// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include <cstring>


namespace PageCalibration
{
    static void FuncDraw();

    static WindowMeasure wndCurrent(TypeMeasure::DC, "", "t5", "t0", "t3", "");     // Текущее значение напряжения
    static WindowMeasure wndGiven(TypeMeasure::DC, "", "t6", "t1", "t4", "");       // Заданное значение напряжения

    void OnEventChangeRange()
    {
        wndCurrent.Reset();
    }

    static void DrawLabelStar()
    {
        uint secs = TIME_MS / 1000;

        static bool is_enabled = false;

        bool enabled = (secs % 2) != 0;

        if (enabled != is_enabled)
        {
            Nextion::Visible("t2", enabled);

            is_enabled = enabled;
        }
    }

    // Выбор точки калибровки
    // 0 - min, 1 - max
    static void ChooseDot(int);

    static void ChooseRange(int);

    static const pchar PASSWORD = "1";

    namespace LabelPassword
    {
        static const int SIZE_BUFFER = 32;
        static int size_password = 0;
        static char buffer[SIZE_BUFFER] = { '\0' };
        static bool is_changed = true;

        static void Reset()
        {
            size_password = 0;
            buffer[0] = '\0';
            is_changed = true;
        }

        static void Append(char symbol)
        {
            if (size_password < SIZE_BUFFER - 1)
            {
                buffer[size_password++] = symbol;
                buffer[size_password] = '\0';
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

                Nextion::DrawString(50, 100, 300, 50, 4, fill, back, LabelPassword::buffer);

                is_changed = false;
            }
        }
    }

    // Установить видимость для всех элементов кроме кнопок
    static void SetVisibleExceptButtons(bool visible);

    // Установить видимость для цифровых кнопок
    static void SetVisibleDigits(bool visible);

    // Нажатие кнопки на цифровой клавиатуре
    static void PressDigit(char symbol)
    {
        LabelPassword::Append(symbol);

        if (LabelPassword::PasswordCorrect())
        {
            LabelPassword::Draw();

            ChooseDot(0);
            ChooseRange(5);

            SetVisibleExceptButtons(true);

            SetVisibleDigits(false);
        }
    }

    static Button btnBack("bt18", "2B0", []()
        {
            PageMain::self->SetAsCurrent();
        });

    static Button btnZero("bt22", "2D1", []() { ChooseDot(0); });

    static Button btnMax("bt21", "2D2", []() { ChooseDot(1); });

    static Button btnSave("b12", "2SV", []()
        {
        });

    static Button btnCalib("b13", "2OK", []()
        {
            btnSave.ChangeVisible(false);
            Calibrator::Run(Range::Current(), btnMax.GetValue(), FuncDraw);
            btnSave.ChangeVisible(true);
        });

    static Button btn2mA("bt17", "21P", []() { ChooseRange(0); });

    static Button btn20mA("bt16", "22P", []() { ChooseRange(1); });

    static Button btn200mA("bt15", "23P", []() { ChooseRange(2); });

    static Button btn2A("bt14", "24P", []() { ChooseRange(3); });

    static Button btn20A("bt13", "25P", []() { ChooseRange(4); });

    static Button btn50A("bt12", "26P", []() { ChooseRange(5); });

    static Button btn0("b0", "KB0", []() { PressDigit('0'); });

    static Button btn1("b1", "KB1", []() { PressDigit('1'); });

    static Button btn2("b2", "KB2", []() { PressDigit('2'); });

    static Button btn3("b3", "KB3", []() { PressDigit('3'); });

    static Button btn4("b4", "KB4", []() { PressDigit('4'); });

    static Button btn5("b5", "KB5", []() { PressDigit('5'); });

    static Button btn6("b6", "KB6", []() { PressDigit('6'); });

    static Button btn7("b7", "KB7", []() { PressDigit('7'); });

    static Button btn8("b8", "KB8", []() { PressDigit('8'); });

    static Button btn9("b9", "KB9", []() { PressDigit('9'); });

    static Button btnDot("b10", "KBD", []() { PressDigit('.'); });

    static Button btnSign("b11", "KBS", []() { PressDigit('-'); });

    static Button *buttons[] =
    {
        &btnBack, &btnSave, &btnCalib, &btnZero, &btnMax,  &btn2mA, &btn20mA, &btn200mA,
        &btn2A,   &btn20A,  &btn50A,   &btn0,    &btn1,    &btn2,   &btn3,    &btn4,
        &btn5,    &btn6,    &btn7,     &btn8,    &btn9,    &btnDot, &btnSign, nullptr
    };

    namespace ButtonsRange
    {
        static Button *buttons[6] =
        {
            &btn2mA,
            &btn20mA,
            &btn200mA,
            &btn2A,
            &btn20A,
            &btn50A
        };

        static void SetAllValue(int value)
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
                if (buttons[i]->GetValue())
                {
                    return i;
                }
            }

            return -1;
        }
    }


    static void ChooseDot(int dot)
    {
        static Button *btns[2] =
        {
            &btnZero,
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
    }

    void SetVisibleDigits(bool visible)
    {
        btn0.ChangeVisible(visible);
        btn1.ChangeVisible(visible);
        btn2.ChangeVisible(visible);
        btn3.ChangeVisible(visible);
        btn4.ChangeVisible(visible);
        btn5.ChangeVisible(visible);
        btn6.ChangeVisible(visible);
        btn7.ChangeVisible(visible);
        btn8.ChangeVisible(visible);
        btn9.ChangeVisible(visible);
        btnSign.ChangeVisible(visible);
        btnDot.ChangeVisible(visible);
    }

    void SetVisibleExceptButtons(bool visible)
    {
        btnZero.ChangeVisible(visible);
        btnMax.ChangeVisible(visible);
        btnSave.ChangeVisible(visible);
        btnCalib.ChangeVisible(visible);
        btn2mA.ChangeVisible(visible);
        btn20mA.ChangeVisible(visible);
        btn200mA.ChangeVisible(visible);
        btn2A.ChangeVisible(visible);
        btn20A.ChangeVisible(visible);
        btn50A.ChangeVisible(visible);

        Nextion::Visible("t5", visible);
        Nextion::Visible("t0", visible);
        Nextion::Visible("t3", visible);
        Nextion::Visible("t6", visible);
        Nextion::Visible("t1", visible);
        Nextion::Visible("t4", visible);
    }

    static void FuncOnEnter()
    {
        wndCurrent.Flash();
        wndGiven.Flash();

        SetVisibleExceptButtons(false);

        SetVisibleDigits(true);

        LabelPassword::Reset();

        btnZero.SetText("Min");
        btnMax.SetText("Max");
        btnCalib.SetText("Calib");
    }

    static void FuncDraw()
    {
        wndCurrent.Draw(Ampermeter::GetDC(), Range::Current());

        LabelPassword::Draw();

        DrawLabelStar();

        int range = ButtonsRange::GetRange();

        if (range >= 0)
        {
            if (btnZero.GetValue() == 1)
            {
                wndGiven.Draw({ 0.0f, false, true }, range);
            }
            else
            {
                static const float values[] =
                {
                    2.0f,
                    20.0f,
                    200.0f,
                    2.0f,
                    20.0f,
                    50.0f
                };

                wndGiven.Draw({ values[range], false, true }, range);
            }
        }
    }

    static Page pageCalibration(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageCalibration;
}
