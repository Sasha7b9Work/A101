// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageMain
{
    static void FuncDraw()
    {

    }


    static Button btn2mA("01P", [](Button *) {});

    static Button btn20mA("02P", [](Button *) {});

    static Button btn200mA("03P", [](Button *) {});

    static Button btn2A("04P", [](Button *) {});

    static Button btn20A("05P", [](Button *) {});

    static Button btn50A("06P", [](Button *) {});

    static Button btnDC("0DC", [](Button *) {});

    static Button btnAC("0AC", [](Button *) {});

    static Button btnAC_DC("0AD", [](Button *) {});

    static Button btnCalibration("0C", [](Button *) {});

    static Button btnZeroDC("0DZ", [](Button *) {});

    static Button btnZeroAC("0AZ", [](Button *) {});

    static Button btnSignal("0S", [](Button *) {});

    static Button btnMAX("01I", [](Button *) {});

    static Button btnAMP("04I", [](Button *) {});

    static Button btnMIN("02I", [](Button *) {});

    static Button btnPEAK("03I", [](Button *) {});

    static Button *buttons[] =
    {
        &btn2mA,
        &btn20mA,
        &btn200mA,
        &btn2A,
        &btn20A,
        &btn50A,
        &btnDC,
        &btnAC,
        &btnAC_DC,
        &btnCalibration,
        &btnZeroDC,
        &btnZeroAC,
        &btnSignal,
        &btnMAX,
        &btnAMP,
        &btnMIN,
        &btnPEAK,
        nullptr
    };

    static Page pageMain(buttons, FuncDraw);

    Page *self = &pageMain;
}

