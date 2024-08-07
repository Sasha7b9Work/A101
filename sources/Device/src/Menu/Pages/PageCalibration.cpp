// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Display/Display.h"
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"
#include "Utils/String.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <cstring>
#include <cstdlib>


namespace PageCalibration
{
    extern Button btnSave;
    extern Button btnCalib;
    extern Button btn0;
    extern Button btn2;

    static void FuncDraw();

    static WindowMeasure wndCurrent(TypeMeasure::DC, "", "t5", "t0", "t3", "");     // ������� �������� ����������
    static WindowMeasure wndGiven(TypeMeasure::DC, "", "t6", "t1", "t4", "");       // �������� �������� ����������

    void OnEventChangeRange()
    {
        wndCurrent.Reset();
    }

    // ����� ����� ����������
    // 0 - min, 1 - max
    static void ChooseDot(int);

    static void ChooseRange(int);

    static const pchar PASSWORD = "1";

    namespace LabelPassword
    {
        static const int SIZE_BUFFER = 32;
        static int num_symbols = 0;                     // ������� �������� ������
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

                Nextion::DrawString(50, 100, 500, 90, 4, fill, back, LabelPassword::buffer);

                is_changed = false;

                Nextion::DrawString(300, 20, 150, 40, 0, fill, back, String<>("%08X", cal.crc32).c_str());
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

    // ���������� ��������� ��� ���� ��������� ����� ������
    static void SetVisibleExceptButtons(bool visible);

    // ���������� ��������� ��� �������� ������
    static void SetVisibleDigits(bool visible);

    static Button btnBack("bt18", "2B0", []()
        {
            PageMain::self->SetAsCurrent();
        });

    static Button btnMin("bt22", "2D1", []() { ChooseDot(0); });

    static Button btnMax("bt21", "2D2", []() { ChooseDot(1); });

    Button btnSave("b12", "2SV", []()
        {
            Calibrator::PressButtonSave();
            btnSave.SetVisible(false);
        });

    // ������� ������ �� �������� ����������
    static void PressDigit(char symbol)
    {
        if (symbol == ' ')                              // Backspace
        {
            LabelPassword::Backspace();
        }
        else if (symbol == '0' && !btn2.IsVisible())
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

                btnSave.SetVisible(false);

                btn0.SetText("Res");
                btn0.SetVisible(true);
            }
            else
            {
                // ������ ������ - �����, ��������� - '-', ����� ���� - �������� �����
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

    Button btnCalib("b13", "2OK", []()
        {
            btnSave.SetVisible(false);
            btnCalib.SetValue(0);
            btnCalib.SetVisible(false);
            if (Calibrator::Run(Range::Current(), (btnMax.GetValue() == 0) ? Calibrator::Type::DC : Calibrator::Type::AC, FuncDraw))
            {
                btnSave.SetVisible(true);
            }
            btnCalib.SetVisible(true);
        });

    static Button btn2mA("bt17", "21P", []() { ChooseRange(0); });

    static Button btn20mA("bt16", "22P", []() { ChooseRange(1); });

    static Button btn200mA("bt15", "23P", []() { ChooseRange(2); });

    static Button btn2A("bt14", "24P", []() { ChooseRange(3); });

    static Button btn20A("bt13", "25P", []() { ChooseRange(4); });

    static Button btn50A("bt12", "26P", []() { ChooseRange(5); });

    Button btn0("b0", "KB0", []() { PressDigit('0'); });

    static Button btn1("b1", "KB1", []() { PressDigit('1'); });

    Button btn2("b2", "KB2", []() { PressDigit('2'); });

    static Button btn3("b3", "KB3", []() { PressDigit('3'); });

    static Button btn4("b4", "KB4", []() { PressDigit('4'); });

    static Button btn5("b5", "KB5", []() { PressDigit('5'); });

    static Button btn6("b6", "KB6", []() { PressDigit('6'); });

    static Button btn7("b7", "KB7", []() { PressDigit('7'); });

    static Button btn8("b8", "KB8", []() { PressDigit('8'); });

    static Button btn9("b9", "KB9", []() { PressDigit('9'); });

    static Button btnDot("b10", "KBD", []() { PressDigit('.'); });

    static Button btnSign("b11", "KBS", []() { PressDigit('-'); });

    static Button btnBackspace("b14", "KBBACK", []() { PressDigit(' '); });

//    static Button btnDebugPage("Debug", "", []() {}, 10, 300);

    static Button *buttons[] =
    {
        &btnBack, &btnSave, &btnCalib, &btnMin, &btnMax,  &btn2mA, &btn20mA, &btn200mA,
        &btn2A,   &btn20A,  &btn50A,   &btn0,   &btn1,    &btn2,   &btn3,    &btn4,
        &btn5,    &btn6,    &btn7,     &btn8,   &btn9,    &btnDot, &btnSign, &btnBackspace, /* &btnDebugPage, */ nullptr
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

        btnSave.SetVisible(false);
    }

    void SetVisibleDigits(bool visible)
    {
        btn0.SetVisible(visible);
        btn1.SetVisible(visible);
        btn2.SetVisible(visible);
        btn3.SetVisible(visible);
        btn4.SetVisible(visible);
        btn5.SetVisible(visible);
        btn6.SetVisible(visible);
        btn7.SetVisible(visible);
        btn8.SetVisible(visible);
        btn9.SetVisible(visible);
        btnSign.SetVisible(visible);
        btnDot.SetVisible(visible);
        btnBackspace.SetVisible(visible);
    }

    void SetVisibleExceptButtons(bool visible)
    {
        btnMin.SetVisible(visible);
        btnMax.SetVisible(visible);
        btnSave.SetVisible(visible);
        btnCalib.SetVisible(visible);
        btn2mA.SetVisible(visible);
        btn20mA.SetVisible(visible);
        btn200mA.SetVisible(visible);
        btn2A.SetVisible(visible);
        btn20A.SetVisible(visible);
        btn50A.SetVisible(visible);

        Nextion::SetVisible("t5", visible);
        Nextion::SetVisible("t0", visible);
        Nextion::SetVisible("t3", visible);
        Nextion::SetVisible("t6", visible);
        Nextion::SetVisible("t1", visible);
        Nextion::SetVisible("t4", visible);
    }

    static void FuncOnEnter()
    {
        btnMin.SetText("Min");
        btnMax.SetText("Max");
        btnCalib.SetText("Calib");

        wndCurrent.Flash();
        wndGiven.Flash();

        SetVisibleExceptButtons(false);

        SetVisibleDigits(true);

        LabelPassword::Reset();

        ChooseDot(0);
    }

    static void FuncDraw()
    {
        wndCurrent.Draw(Ampermeter::GetDC(), Range::Current());

        LabelPassword::Draw();

        int range = ButtonsRange::GetRange();

        if (range >= 0)
        {
            if (btnMin.GetValue() == 1)
            {
                wndGiven.Draw({ 0.0, false, true }, range);
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

                wndGiven.Draw({ values[range], false, true }, range);
            }
        }
    }

    static Page pageCalibration(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageCalibration;
}
