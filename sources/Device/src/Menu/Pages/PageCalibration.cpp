// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"
#include <cstring>


namespace PageCalibration
{
    static const pchar PASSWORD = "123";

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

    static void FuncDraw()
    {
        LabelPassword::Draw();
    }

    // Нажатие кнопки на цифровой клавиатуре
    static void PressDigit(char symbol)
    {
        LabelPassword::Append(symbol);

        if (LabelPassword::PasswordCorrect())
        {
            LabelPassword::Draw();

            SetVisibleExceptButtons(true);

            SetVisibleDigits(false);
        }
    }

    static Button btnBack("bt18", "2B0", [](Button *)
        {
            PageMain::self->SetAsCurrent();
        });

    static Button btnSave("b12", "2SV", [](Button *) {});

    static Button btnOK("b13", "2OK", [](Button *) {});

    static Button btnDot1("bt22", "2D1", [](Button *) {});

    static Button btnDot2("bt21", "2D2", [](Button *) {});

    static Button btn2mA("bt17", "21P", [](Button *) {});

    static Button btn20mA("bt16", "22P", [](Button *) {});

    static Button btn200mA("bt15", "23P", [](Button *) {});

    static Button btn2A("bt14", "24P", [](Button *) {});

    static Button btn20A("bt13", "25P", [](Button *) {});

    static Button btn50A("bt12", "26P", [](Button *) {});

    static Button btn0("b0", "KB0", [](Button *)
        {
            PressDigit('0');
        });

    static Button btn1("b1", "KB1", [](Button *)
        {
            PressDigit('1');
        });

    static Button btn2("b2", "KB2", [](Button *)
        {
            PressDigit('2');
        });

    static Button btn3("b3", "KB3", [](Button *)
        {
            PressDigit('3');
        });

    static Button btn4("b4", "KB4", [](Button *)
        {
            PressDigit('4');
        });

    static Button btn5("b5", "KB5", [](Button *)
        {
            PressDigit('5');
        });

    static Button btn6("b6", "KB6", [](Button *)
        {
            PressDigit('6');
        });

    static Button btn7("b7", "KB7", [](Button *)
        {
            PressDigit('7');
        });

    static Button btn8("b8", "KB8", [](Button *)
        {
            PressDigit('8');
        });

    static Button btn9("b9", "KB9", [](Button *)
        {
            PressDigit('9');
        });

    static Button btnDot("b10", "KBD", [](Button *)
        {
            PressDigit('.');
        });

    static Button btnSign("b11", "KBS", [](Button *)
        {
            PressDigit('-');
        });

    static Button *buttons[] =
    {
        &btnBack,
        &btnSave,
        &btnOK,
        &btnDot1,
        &btnDot2,
        &btn2mA,
        &btn20mA,
        &btn200mA,
        &btn2A,
        &btn20A,
        &btn50A,
        &btn0,
        &btn1,
        &btn2,
        &btn3,
        &btn4,
        &btn5,
        &btn6,
        &btn7,
        &btn8,
        &btn9,
        &btnDot,
        &btnSign,
        nullptr
    };

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
        btnDot1.ChangeVisible(visible);
        btnDot2.ChangeVisible(visible);
        btnBack.ChangeVisible(visible);
        btnSave.ChangeVisible(visible);
        btnOK.ChangeVisible(visible);
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

    static void FuncOnEnable(bool)
    {
        LabelPassword::Reset();

        btnDot1.SetText("Min");
        btnDot2.SetText("Max");
        btnOK.SetText("Calib");

        SetVisibleExceptButtons(false);
    }

    static Page pageCalibration(buttons, FuncOnEnable, FuncDraw);

    Page *self = &pageCalibration;
}
