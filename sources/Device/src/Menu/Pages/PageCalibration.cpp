// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Nextion/Nextion.h"


namespace PageCalibration
{
    static void FuncDraw()
    {

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

    static Button btn0("KB0", [](Button *) {});

    static Button btn1("KB1", [](Button *) {});

    static Button btn2("KB2", [](Button *) {});

    static Button btn3("KB3", [](Button *) {});

    static Button btn4("KB4", [](Button *) {});

    static Button btn5("KB5", [](Button *) {});

    static Button btn6("KB6", [](Button *) {});

    static Button btn7("KB7", [](Button *) {});

    static Button btn8("KB8", [](Button *) {});

    static Button btn9("KB9", [](Button *) {});

    static Button btnDot("KBD", [](Button *) {});

    static Button btnSign("KBS", [](Button *) {});

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

    static Page pageCalibration(buttons, FuncDraw);

    Page *self = &pageCalibration;
}
