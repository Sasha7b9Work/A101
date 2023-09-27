// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"


namespace PageMain
{
    static void FuncOnEnable(bool)
    {

    }

    static void FuncDraw()
    {
        Indicator::Update();

        uint secs = TIME_MS / 1000;

        static bool is_enabled = false;

        bool enabled = (secs % 2) != 0;

        if (enabled != is_enabled)
        {
            Nextion::Visible("t4", enabled);

            is_enabled = enabled;
        }
    }


    static Button btn2mA("bt0", "01P", [](Button *)
        {
            Range::Set(0);
        });

    static Button btn20mA("bt1", "02P", [](Button *)
        {
            Range::Set(1);
        });

    static Button btn200mA("bt2", "03P", [](Button *)
        {
            Range::Set(2);
        });

    static Button btn2A("bt3", "04P", [](Button *)
        {
            Range::Set(3);
        });

    static Button btn20A("bt4", "05P", [](Button *)
        {
            Range::Set(4);
        });

    static Button btn50A("bt5", "06P", [](Button *)
        {
            Range::Set(5);
        });

    static Button btnAC_DC("b0", "0AD", [](Button *) {});

    static Button btnCalibration("bt13", "0C", [](Button *)
        {
            PageCalibration::self->SetAsCurrent();
        });

    static Button btnZeroDC("bt11", "0DZ", [](Button *) {});

    static Button btnZeroAC("bt10", "0AZ", [](Button *) {});

    static Button btnSignal("bt12", "0S", [](Button *) {});

    static Button btnMAX("bt15", "01I", [](Button *) {});

    static Button btnAMP("bt19", "04I", [](Button *) {});

    static Button btnMIN("bt18", "02I", [](Button *) {});

    static Button btnPEAK("bt17", "03I", [](Button *) {});

    static Button *buttons[] =
    {
        &btn2mA,
        &btn20mA,
        &btn200mA,
        &btn2A,
        &btn20A,
        &btn50A,
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

    static Page pageMain(buttons, FuncOnEnable, FuncDraw);

    Page *self = &pageMain;
}

