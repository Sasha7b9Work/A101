// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Nextion/Display.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cstring>
#include <cstdlib>


namespace PageCalibration
{
    extern ButtonPress btnSave;
    extern ButtonPress btnCalib;
    extern ButtonPress btn0;
    extern ButtonPress btn2;

    static void FuncDraw();

    static LabelMeasure wndCurrent{ TypeMeasure::DC, SizeMeasure::Calibration, 210, 150 };     // Текущее значение напряжения
    static LabelMeasure wndGiven  { TypeMeasure::DC, SizeMeasure::Calibration, 210, 250 };     // Заданное значение напряжения

    void OnEventChangeRange()
    {
        wndCurrent.Reset();
    }

    static void ChooseRange(int);

    static const pchar PASSWORD = "1";

    namespace LabelPassword
    {
        static const int SIZE_BUFFER = 32;
        static int num_symbols = 0;                     // Столько символов нажато
        static char buffer[SIZE_BUFFER] = { '\0' };
        static bool is_changed = true;

        static void Reset()
        {
            num_symbols = 0;
            buffer[0] = '\0';
            is_changed = true;
        }

        static void Append(char symbol)
        {
            if (num_symbols < SIZE_BUFFER - 1)
            {
                buffer[num_symbols++] = symbol;
                buffer[num_symbols] = '\0';
                is_changed = true;
            }
        }

        static bool IsChanged()
        {
            return is_changed;
        }

        static bool PasswordCorrect()
        {
            return std::strcmp(LabelPassword::buffer, PASSWORD) == 0;
        }

        static void Draw()
        {
            if (IsChanged())
            {
                Color fill = Color::White;
                Color back = Color::Black;

                if (PasswordCorrect())
                {
                    fill = Color::Background;
                    back = Color::Background;
                }

                Nextion::DrawString({ 50, 100, 500, 90 }, 4, fill, back, LabelPassword::buffer);

                is_changed = false;

                Nextion::DrawString({ 300, 20, 150, 40 }, 0, fill, back, String<>("%08X", cal.crc32).c_str());
            }
        }

        static void Backspace()
        {
            if (num_symbols > 0)
            {
                buffer[--num_symbols] = '\0';
                is_changed = true;
            }
        }
    }

    // Установить видимость для всех элементов кроме кнопок
    static void SetVisibleExceptButtons(bool visible);

    // Установить видимость для цифровых кнопок
    static void SetVisibleDigits(bool visible);

    const int y = 183;

    static ButtonToggle btnMin("Мин", "Min", Font::_1_GB42b, { 640, y, 150, 73 }, [](Item *, bool)
    {
    });

    static ButtonToggle btnMax("Макс", "Max", Font::_1_GB42b, { 640, y - 79, 150, 73 }, [](Item *, bool)
    {
    });

    ButtonPress btnSave("Сохр.", "Save", Font::_1_GB42b, { 236, 6, 130, 73 }, [](Item *item, bool)
        {
            Calibrator::PressButtonSave();
            item->SetShown(false);
        });

    // Нажатие кнопки на цифровой клавиатуре
    static void PressDigit(char symbol, bool press)
    {
        if (!press)
        {
            return;
        }

        if (symbol == ' ')                              // Backspace
        {
            LabelPassword::Backspace();
        }
        else if (symbol == '0' && !btn2.IsShown())
        {
            cal.Reset();
        }
        else
        {
            LabelPassword::Append(symbol);

            if (LabelPassword::PasswordCorrect())
            {
                LabelPassword::Draw();

                ChooseRange(5);

                SetVisibleExceptButtons(true);

                SetVisibleDigits(false);

                btnSave.SetShown(false);
            }
            else
            {
                // Первый символ - точка, последний - '-', между ними - серийный номер - две группы по четыре цифры, между которыми пробел
                if (LabelPassword::buffer[0] == '.' && LabelPassword::buffer[LabelPassword::num_symbols - 1] == '-')
                {
                    LabelPassword::buffer[LabelPassword::num_symbols - 1] = '\0';
                    LabelPassword::buffer[5] = '\0';

                    uint low = (uint)std::atoi(LabelPassword::buffer + 1);
                    uint hi = (uint)std::atoi(LabelPassword::buffer + 6);

                    set.serial_number = (hi << 16) + low;

                    set.Save();

                    PageInformation::self->SetAsCurrent();
                }
            }
        }
    }

    ButtonPress btnCalib("Старт", "Start", Font::_1_GB42b, { 372, Item::GetCoordY(0), 130, 73 }, [](Item *, bool)
        {
            btnSave.SetShown(false);
            btnCalib.Release();
            btnCalib.SetShown(false);
            if (Calibrator::Run(Range::Current(), (!btnMax.IsPressed()) ? Calibrator::Type::DC : Calibrator::Type::AC, FuncDraw))
            {
                btnSave.SetShown(true);
            }
            btnCalib.SetShown(true);
        });

    static const int BUTTON_X = 0;

    static ButtonToggle btn2mA("2 мА", "2 mA", Font::_1_GB42b, { BUTTON_X, 6, 150, 73 }, [](Item *, bool)
    {
        ChooseRange(0);
    });

    static ButtonToggle btn20mA("20 мА", "20 mA", Font::_1_GB42b, { BUTTON_X, 85, 150, 73 }, [](Item *, bool)
    {
        ChooseRange(1);
    });

    static ButtonToggle btn200mA("200 мА", "200 mA", Font::_1_GB42b, { BUTTON_X, 164, 150, 73 }, [](Item *, bool)
    {
        ChooseRange(2);
    });

    static ButtonToggle btn2A("2 А", "2 A", Font::_1_GB42b, { BUTTON_X, 243, 150, 73 }, [](Item *, bool)
    {
        ChooseRange(3);
    });

    static ButtonToggle btn20A("20 А", "20 A", Font::_1_GB42b, { BUTTON_X, 322, 150, 73 }, [](Item *, bool)
    {
        ChooseRange(4);
    });

    static ButtonToggle btn50A("50 А", "50 A", Font::_1_GB42b, { BUTTON_X, 401, 150, 73 }, [](Item *, bool)
    {
        ChooseRange(5);
    });

#define SIZE_DIGIT 96, 96

    ButtonPress btn0("0", "0", Font::_1_GB42b, { 12, 276, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('0', press);
    });

    static ButtonPress btn1("1", "1", Font::_1_GB42b, { 113, 276, SIZE_DIGIT },  [](Item *, bool press)
    {
        PressDigit('1', press);
    });

    ButtonPress btn2("2", "2", Font::_1_GB42b, { 214, 276, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('2', press);
    });

    static ButtonPress btn3("3", "3", Font::_1_GB42b, { 315, 276, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('3', press);
    });

    static ButtonPress btn4("4", "4", Font::_1_GB42b, { 416, 276, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('4', press);
    });

    static ButtonPress btn5("5", "5", Font::_1_GB42b, { 517, 276, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('5', press);
    });

    static ButtonPress btn6("6", "6", Font::_1_GB42b, { 113, 377, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('6', press);
    });

    static ButtonPress btn7("7", "7", Font::_1_GB42b, { 217, 377, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('7', press);
    });

    static ButtonPress btn8("8", "8", Font::_1_GB42b, { 315, 377, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('8', press);
    });

    static ButtonPress btn9("9", "9", Font::_1_GB42b, { 416, 377, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('9', press);
    });

    static ButtonPress btnDot(".", ".", Font::_1_GB42b, { 12, 377, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('.', press);
    });

    static ButtonPress btnSign("+-", "+-", Font::_1_GB42b, { 517, 377, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit('-', press);
    });

    static ButtonPress btnBackspace("<-", "<-", Font::_1_GB42b, { 624, 334, SIZE_DIGIT }, [](Item *, bool press)
    {
        PressDigit(' ', press);
    });

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMenu::self->SetAsCurrent();
        }
    });

    namespace ButtonsRange
    {
        static ButtonToggle *buttons[6] =
        {
            &btn2mA,
            &btn20mA,
            &btn200mA,
            &btn2A,
            &btn20A,
            &btn50A
        };

        static void SetAllValue(bool value)
        {
            for (int i = 0; i < 6; i++)
            {
                buttons[i]->SetToggled(value, false);
            }
        }

        static void SetRange(int range)
        {
            SetAllValue(0);

            buttons[range]->SetToggled(true, false);
        }

        static int GetRange()
        {
            for (int i = 0; i < 6; i++)
            {
                if (((ButtonPress *)buttons[i])->IsPressed())
                {
                    return i;
                }
            }

            return -1;
        }
    }

    static void ChooseRange(int range)
    {
        ButtonsRange::SetRange(range);
        Range::Set(range);

        Timer::Delay(400);

        HAL_PIO::Write(PIN_ZERO, false);

        btnSave.SetShown(false);
    }

    void SetVisibleDigits(bool visible)
    {
        btn0.SetShown(visible);
        btn0.Refresh();
        btn1.SetShown(visible);
        btn2.SetShown(visible);
        btn3.SetShown(visible);
        btn4.SetShown(visible);
        btn5.SetShown(visible);
        btn6.SetShown(visible);
        btn7.SetShown(visible);
        btn8.SetShown(visible);
        btn9.SetShown(visible);
        btnSign.SetShown(visible);
        btnDot.SetShown(visible);
        btnBackspace.SetShown(visible);
    }

    void SetVisibleExceptButtons(bool visible)
    {
        btnMin.SetShown(visible);
        btnMax.SetShown(visible);
        btnSave.SetShown(visible);
        btnCalib.SetShown(visible);
        btn2mA.SetShown(visible);
        btn20mA.SetShown(visible);
        btn200mA.SetShown(visible);
        btn2A.SetShown(visible);
        btn20A.SetShown(visible);
        btn50A.SetShown(visible);

        wndCurrent.SetEnabled(visible);
        wndGiven.SetEnabled(visible);
    }

    static void FuncOnEnter()
    {
        btnMin.SetText("Мин", "Min");
        btnMax.SetText("Макс", "Max");
        btnCalib.SetText("Калиб.", "Calib.");

        wndCurrent.Flash();
        wndGiven.Flash();

        SetVisibleExceptButtons(false);

        SetVisibleDigits(true);

        LabelPassword::Reset();
    }

    static void FuncDraw()
    {
        wndCurrent.SetMeasure(Ampermeter::GetDC(), Range::Current());

        LabelPassword::Draw();

        int range = ButtonsRange::GetRange();

        if (range >= 0)
        {
            if (btnMin.IsPressed())
            {
                wndGiven.SetMeasure({ 0.0, false, true }, range);
            }
            else
            {
                static const REAL values[] =
                {
                    2.0,
                    20.0,
                    200.0,
                    2000.0,
                    20000.0,
                    50000.0
                };

                wndGiven.SetMeasure({ values[range], false, true }, range);
            }
        }
    }

    static Item *items[] =
    {
        &btnBack,    &btnSave, &btnCalib, &btnMin, &btnMax,  &btn2mA, &btn20mA, &btn200mA,
        &btn2A,      &btn20A,  &btn50A,   &btn0,   &btn1,    &btn2,   &btn3,    &btn4,
        &btn5,       &btn6,    &btn7,     &btn8,   &btn9,    &btnDot, &btnSign, &btnBackspace,
        &wndCurrent, &wndGiven, nullptr
    };

    static Page pageCalibration(items, FuncOnEnter, FuncDraw);

    Page *self = &pageCalibration;
}
