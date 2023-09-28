// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/Ampermeter.h"


namespace PageMain
{
    static char measureDC[TextString::MAX_LEN] = { '\0' };

    static WindowMeasure wndDC("t2", "t19", "t0", "t18", "DC:");

    static void DrawLabelStart()
    {
        uint secs = TIME_MS / 1000;

        static bool is_enabled = false;

        bool enabled = (secs % 2) != 0;

        if (enabled != is_enabled)
        {
            Nextion::Visible("t4", enabled);

            is_enabled = enabled;
        }
    }

    static void FuncOnEnable()
    {

    }

    static void FuncDraw()
    {
        Indicator::Update();

        DrawLabelStart();

        int range = Range::Current();

        static const int after[6] = { 4, 3, 2, 4, 3, 3 };
        const pchar suffix = (range < 3) ? "mA" : "A";

        Indicator::ConvertDoubleToText(Ampermeter::GetDC(), measureDC, after[range], suffix);

        if (Ampermeter::OutOfRange())
        {
            for (int i = 0; (i < TextString::MAX_LEN) && (measureDC[i] != '\0'); i++)
            {
                if (measureDC[i] != '.') { measureDC[i] = '^'; }
            }
        }
        else
        {
            wndDC.SetMeasure(measureDC);
        }
    }


    static Button btn2mA("bt0", "01P", []()
        {
            Range::Set(0);
        });

    static Button btn20mA("bt1", "02P", []()
        {
            Range::Set(1);
        });

    static Button btn200mA("bt2", "03P", []()
        {
            Range::Set(2);
        });

    static Button btn2A("bt3", "04P", []()
        {
            Range::Set(3);
        });

    static Button btn20A("bt4", "05P", []()
        {
            Range::Set(4);
        });

    static Button btn50A("bt5", "06P", []()
        {
            Range::Set(5);
        });

    static Button btnAC_DC("b0", "0AD", []() {});

    static Button btnCalibration("bt13", "0C", []()
        {
            PageCalibration::self->SetAsCurrent();
        });

    static Button btnZeroDC("bt11", "0DZ", []() {});

    static Button btnZeroAC("bt10", "0AZ", []() {});

    static Button btnSignal("bt12", "0S", []() {});

    static Button btnMAX("bt15", "01I", []() {});

    static Button btnAMP("bt19", "04I", []() {});

    static Button btnMIN("bt18", "02I", []() {});

    static Button btnPEAK("bt17", "03I", []() {});

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

