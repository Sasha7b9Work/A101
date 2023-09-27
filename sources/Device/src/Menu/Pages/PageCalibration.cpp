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

        static void Draw()
        {
            Nextion::DrawString(50, 100, 300, 50, 4, Color::White, Color::Black, LabelPassword::buffer);

            is_changed = false;
        }
    }

    // Установить видимость для всех элементов кроме кнопок
    static void SetVisibleExceptButtons(bool visible);


    static void FuncOnEnable(bool)
    {
        LabelPassword::Reset();

        SetVisibleExceptButtons(false);
    }

    static void FuncDraw()
    {
        if (LabelPassword::IsChanged())
        {
            LabelPassword::Draw();
        }
    }

    // Нажатие кнопки на цифровой клавиатуре
    static void PressDigit(char symbol)
    {
        LabelPassword::Append(symbol);

        if (std::strcmp(LabelPassword::buffer, PASSWORD) == 0)
        {
            SetVisibleExceptButtons(true);
        }
    }


    void SetVisibleExceptButtons(bool visible)
    {
        Nextion::Visible("b12", visible);
        Nextion::Visible("b13", visible);
        Nextion::Visible("t2", visible);

        Nextion::Visible("bt17", visible);
        Nextion::Visible("bt16", visible);
        Nextion::Visible("bt15", visible);
        Nextion::Visible("bt14", visible);
        Nextion::Visible("bt13", visible);
        Nextion::Visible("bt12", visible);

        Nextion::Visible("bt22", visible);
        Nextion::Visible("bt21", visible);

        Nextion::Visible("t5", visible);
        Nextion::Visible("t0", visible);
        Nextion::Visible("t3", visible);
        Nextion::Visible("t6", visible);
        Nextion::Visible("t1", visible);
        Nextion::Visible("t4", visible);
    }


    static Button btnBack("2B0", [](Button *)
        {
            PageMain::self->SetAsCurrent();
        });

    static Button btnSave("2SV", [](Button *) {});

    static Button btnOK("2OK", [](Button *) {});

    static Button btnDot1("2D1", [](Button *) {});

    static Button btnDot2("2D2", [](Button *) {});

    static Button btn2mA("21P", [](Button *) {});

    static Button btn20mA("22P", [](Button *) {});

    static Button btn200mA("23P", [](Button *) {});

    static Button btn2A("24P", [](Button *) {});

    static Button btn20A("25P", [](Button *) {});

    static Button btn50A("26P", [](Button *) {});

    static Button btn0("KB0", [](Button *)
        {
            PressDigit('0');
        });

    static Button btn1("KB1", [](Button *)
        {
            PressDigit('1');
        });

    static Button btn2("KB2", [](Button *)
        {
            PressDigit('2');
        });

    static Button btn3("KB3", [](Button *)
        {
            PressDigit('3');
        });

    static Button btn4("KB4", [](Button *)
        {
            PressDigit('4');
        });

    static Button btn5("KB5", [](Button *)
        {
            PressDigit('5');
        });

    static Button btn6("KB6", [](Button *)
        {
            PressDigit('6');
        });

    static Button btn7("KB7", [](Button *)
        {
            PressDigit('7');
        });

    static Button btn8("KB8", [](Button *)
        {
            PressDigit('8');
        });

    static Button btn9("KB9", [](Button *)
        {
            PressDigit('9');
        });

    static Button btnDot("KBD", [](Button *)
        {
            PressDigit('.');
        });

    static Button btnSign("KBS", [](Button *)
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

    static Page pageCalibration(buttons, FuncOnEnable, FuncDraw);

    Page *self = &pageCalibration;
}
