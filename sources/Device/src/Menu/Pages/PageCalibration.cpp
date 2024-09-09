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

    static LabelMeasure wndCurrent{ TypeMeasure::DC, SizeMeasure::Calibration, 221, 98 };     // Текущее значение напряжения
    static LabelMeasure wndGiven  { TypeMeasure::DC, SizeMeasure::Calibration, 221, 183 };     // Заданное значение напряжения

    void OnEventChangeRange()
    {
        wndCurrent.Reset();
    }

    // Выбор точки калибровки
    // 0 - min, 1 - max
    static void ChooseDot(int);

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
                Color fill = PasswordCorrect() ? Color::Background : Color::White;
                Color back = PasswordCorrect() ? Color::Background : Color::Black;

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

    static ButtonPress btnBack("Назад", "Back", Font::_1, { 11, 6, 150, 73 }, [](Item *)
        {
            PageMain::self->SetAsCurrent();
        });

    static ButtonPress btnMin("Мин", "Min", Font::_1, { 11, 183, 150, 73 }, [](Item *)
    {
        ChooseDot(0);
    });

    static ButtonPress btnMax("Макс", "Max", Font::_1, { 11, 104, 150, 73 }, [](Item *)
    {
        ChooseDot(1);
    });

    ButtonPress btnSave("Сохр.", "Save", Font::_1, { 236, 6, 130, 73 }, [](Item *item)
        {
            Calibrator::PressButtonSave();
            item->ToButtonPress()->SetShown(false);
        });

    // Нажатие кнопки на цифровой клавиатуре
    static void PressDigit(char symbol)
    {
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

                ChooseDot(0);
                ChooseRange(5);

                SetVisibleExceptButtons(true);

                SetVisibleDigits(false);

                btnSave.SetShown(false);

                btn0.SetText("Сброс", "Reset");
                btn0.SetShown(true);
            }
            else
            {
                // Первый символ - точка, последний - '-', между ними - серийный номер
                if (LabelPassword::buffer[0] == '.' && LabelPassword::buffer[LabelPassword::num_symbols - 1] == '-')
                {
                    LabelPassword::buffer[LabelPassword::num_symbols - 1] = '\0';
                    LabelPassword::buffer[5] = '\0';

                    uint low = (uint)std::atoi(LabelPassword::buffer + 1);
                    uint hi = (uint)std::atoi(LabelPassword::buffer + 6);

                    set.serial_number = (hi << 16) + low;

                    set.Save();
                }
            }
        }
    }

    ButtonPress btnCalib("Старт", "Start", Font::_1, { 372, 6, 130, 73 }, [](Item *)
        {
            btnSave.SetShown(false);
            btnCalib.SetValue(0);
            btnCalib.SetShown(false);
            if (Calibrator::Run(Range::Current(), (!btnMax.IsPressed()) ? Calibrator::Type::DC : Calibrator::Type::AC, FuncDraw))
            {
                btnSave.SetShown(true);
            }
            btnCalib.SetShown(true);
        });

    static ButtonPress btn2mA("2 мА", "2 mA", Font::_1, { 640, 6, 150, 73 }, [](Item *)
    {
        ChooseRange(0);
    });

    static ButtonPress btn20mA("20 мА", "20 mA", Font::_1, { 640, 85, 150, 73 }, [](Item *)
    {
        ChooseRange(1);
    });

    static ButtonPress btn200mA("200 мА", "200 mA", Font::_1, { 640, 164, 150, 73 }, [](Item *)
    {
        ChooseRange(2);
    });

    static ButtonPress btn2A("2 А", "2 A", Font::_1, { 640, 243, 150, 73 }, [](Item *)
    {
        ChooseRange(3);
    });

    static ButtonPress btn20A("20 А", "20 A", Font::_1, { 640, 322, 150, 73 }, [](Item *)
    {
        ChooseRange(4);
    });

    static ButtonPress btn50A("50 А", "50 A", Font::_1, { 640, 401, 150, 73 }, [](Item *)
    {
        ChooseRange(5);
    });

#define SIZE_DIGIT 96, 96

    ButtonPress btn0("0", "0", Font::_1, { 12, 276, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('0');
    });

    static ButtonPress btn1("1", "1", Font::_1, { 113, 276, SIZE_DIGIT },  [](Item *)
    {
        PressDigit('1');
    });

    ButtonPress btn2("2", "2", Font::_1, { 214, 276, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('2');
    });

    static ButtonPress btn3("3", "3", Font::_1, { 315, 276, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('3');
    });

    static ButtonPress btn4("4", "4", Font::_1, { 416, 276, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('4');
    });

    static ButtonPress btn5("5", "5", Font::_1, { 517, 276, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('5');
    });

    static ButtonPress btn6("6", "6", Font::_1, { 113, 377, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('6');
    });

    static ButtonPress btn7("7", "7", Font::_1, { 217, 377, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('7');
    });

    static ButtonPress btn8("8", "8", Font::_1, { 315, 377, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('8');
    });

    static ButtonPress btn9("9", "9", Font::_1, { 416, 377, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('9');
    });

    static ButtonPress btnDot(".", ".", Font::_1, { 12, 377, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('.');
    });

    static ButtonPress btnSign("+-", "+-", Font::_1, { 517, 377, SIZE_DIGIT }, [](Item *)
    {
        PressDigit('-');
    });

    static ButtonPress btnBackspace("<-", "<-", Font::_1, { 624, 334, SIZE_DIGIT }, [](Item *)
    {
        PressDigit(' ');
    });

    static Item *items[] =
    {
        &btnBack, &btnSave, &btnCalib, &btnMin, &btnMax,  &btn2mA, &btn20mA, &btn200mA,
        &btn2A,   &btn20A,  &btn50A,   &btn0,   &btn1,    &btn2,   &btn3,    &btn4,
        &btn5,    &btn6,    &btn7,     &btn8,   &btn9,    &btnDot, &btnSign, &btnBackspace, nullptr
    };

    namespace ButtonsRange
    {
        static ButtonPress *buttons[6] =
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
                buttons[i]->SetValue(value);
            }
        }

        static void SetRange(int range)
        {
            SetAllValue(0);

            buttons[range]->SetValue(1);
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


    static void ChooseDot(int dot)
    {
        static ButtonPress *btns[2] =
        {
            &btnMin,
            &btnMax
        };

        for (int i = 0; i < 2; i++)
        {
            btns[i]->SetValue(0);
        }

        btns[dot]->SetValue(1);
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

        ChooseDot(0);
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

    static Page pageCalibration(items, FuncOnEnter, FuncDraw);

    Page *self = &pageCalibration;
}
