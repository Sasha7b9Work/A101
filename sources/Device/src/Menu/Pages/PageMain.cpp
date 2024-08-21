// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/Ampermeter.h"
#include "Display/Display.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include <cstdio>


namespace PageMain
{
    WindowMeasure wndDC(TypeMeasure::DC, "titleDC", "f_sign", "valueDC", "unitsDC", "DC:");
    WindowMeasure wndAC(TypeMeasure::AC, "titleAC", "", "valueAC", "unitsAC", "AC:");

    static WindowMeasure wndAMPL(TypeMeasure::Ampl, "tIamp", "", "tIampValue", "tIampUnits", "");
    static WindowMeasure wndPEAK(TypeMeasure::Peak, "tIpp",  "", "tIppValue",  "tIppUnits", "");
    static WindowMeasure wndMIN(TypeMeasure::Min,   "tImin", "", "tIminValue", "tIminUnits", "");
    static WindowMeasure wndMAX(TypeMeasure::Max,   "tImax", "", "tImaxValue", "tImaxUnits", "");

    static void ResetAllMeasures()
    {
        wndDC.Reset();
        wndAC.Reset();
        wndAMPL.Reset();
        wndPEAK.Reset();
        wndMIN.Reset();
        wndMAX.Reset();
    }

    void Init()
    {
    }

    void OnEventChangeRange()
    {
        ResetAllMeasures();
    }

    static void FuncOnEnter()
    {
        ResetAllMeasures();

        int range = Range::Current();

        if (range > 0)
        {
            Range::Set(range - 1);
            Range::Set(range);
        }
        else
        {
            Range::Set(range + 1);
            Range::Set(range);
        }

        for (int i = 0; i < 6; i++)
        {
            char name[32];
            std::sprintf(name, "bt%d", i);
            Nextion::SetValue(name, Range::Current() == i ? 1 : 0);
        }
    }

    static void FuncDraw()
    {
        Display::LabelStar::Update();

        wndDC.Draw(Ampermeter::GetDC(), Range::Current());

        wndAC.Draw(Ampermeter::GetAC(), Range::Current());

        wndAMPL.Draw(Ampermeter::GetAmpl(), Range::Current());
        wndPEAK.Draw(Ampermeter::GetPeak(), Range::Current());
        wndMIN.Draw(Ampermeter::GetMin(), Range::Current());
        wndMAX.Draw(Ampermeter::GetMax(), Range::Current());
    }


    static Button btn2mA("btn2mA", "01P", []()
    {
        Ampermeter::AVP::Disable();
        Range::Set(0);
    });

    static Button btn2maAVP("tAVP2mA", "01A", []()
    {
        Ampermeter::AVP::Enable();
        Nextion::SetVisible("tAVP2mA", true);
    });

    static Button btn20mA("btn20mA", "02P", []()
    {
        Ampermeter::AVP::Disable();
        Range::Set(1);
    });

    static Button btn20maAVP("tAVP20mA", "02A", []()
    {
        Ampermeter::AVP::Enable();
    });

    static Button btn200mA("btn200mA", "03P", []()
    {
        Ampermeter::AVP::Disable();
        Range::Set(2);
    });

    static Button btn200maAVP("tAVP200mA", "03A", []()
    {
        Ampermeter::AVP::Enable();
    });

    static Button btn2A("btn2A", "04P", []()
    {
        Ampermeter::AVP::Disable();
        Range::Set(3);
    });

    static Button btn2aAVP("tAVP2A", "04A", []()
    {
        Ampermeter::AVP::Enable();
    });

    static Button btn20A("btn20A", "05P", []()
    {
        Ampermeter::AVP::Disable();
        Range::Set(4);
    });

    static Button btn50A("btn50A", "06P", []()
    {
        Ampermeter::AVP::Disable();
        Range::Set(5);
    });

    static Button btnAC_DC("btnACDC", "0AD", []()            // Переход в AC+DC
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC_DC);
    });

    static Button btnAC("btnACDC", "0AC", []()              // Переход в AC
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC);
    });

    static Button btnDC("btnACDC", "0DC", []()              // Переход в DC
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::DC);
    });

    static Button btnCalibration("btnCalibr", "0C", []()
    {
        PageCalibration::self->SetAsCurrent();
    });

    static Button btnSettings("btnSettings", "0T", []()
    {
        PageSettings::self->SetAsCurrent();
    });

    static Button btnZeroDC_EN("btnZeroDC", "0DZ1", []()         // Включение режима "Zero DC"
    {
        Ampermeter::ZeroDC::Enable();
    });

    static Button btnZeroDC_DIS("btnZeroDC", "0DZ0", []()        // Выключение режима "Zero DC"
    {
        Ampermeter::ZeroDC::Disable();
    });

    static Button btnZeroAC_EN("btnZeroAC", "0AZ1", []()         // Включение режима "Zero AC"
    {
        Ampermeter::ZeroAC::Enable();
    });

    static Button btnZeroAC_DIS("btnZeroAC", "0AZ0", []()        // Выключение режима "Zero AC"
    {
        Ampermeter::ZeroAC::Disable();
    });

    static Button btnGraphics("btnGraphics", "0S", []()            // Доступ к графикам
    {
        PageGraph::self->SetAsCurrent();
    });

    static Button btnMAX("btnImax", "01I", []() {});           // Imax

    static Button btnAMP("btnIamp", "04I", []() {});           // Iamp

    static Button btnMIN("btnImin", "02I", []() {});           // Imin

    static Button btnPEAK("btnIpp", "03I", []() {});          // Ipp

    static Button btnZero("btnZero", "0ZE", []()
    {
        Ampermeter::ZeroDC::FloatingZero::Process();
    });

    static Button *buttons[] =
    {
        &btn2mA,
        &btn20mA,
        &btn200mA,
        &btn2A,
        &btn20A,
        &btn50A,
        &btnAC_DC,
        &btnAC,
        &btnDC,
        &btnCalibration,
        &btnZeroDC_EN,
        &btnZeroDC_DIS,
        &btnZeroAC_EN,
        &btnZeroAC_DIS,
        &btnGraphics,
        &btnMAX,
        &btnAMP,
        &btnMIN,
        &btnPEAK,
        &btnSettings,
        &btn2maAVP,
        &btn20maAVP,
        &btn200maAVP,
        &btn2aAVP,
        &btnZero,
        nullptr
    };

    static Page pageMain(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageMain;

    void SetRange(MeasuresOnDisplay::E meas, int range)
    {
        MeasuresOnDisplay::Set(meas);

        if (MeasuresOnDisplay::IsAC_DC())
        {
            btnAC_DC.SetText("AC+DC");
        }
        else if (MeasuresOnDisplay::IsAC())
        {
            btnAC_DC.SetText("AC");
        }
        else if (MeasuresOnDisplay::IsDC())
        {
            btnAC_DC.SetText("DC");
        }

        wndAC.SetVisible(MeasuresOnDisplay::IsAC_DC() || MeasuresOnDisplay::IsAC());
        wndDC.SetVisible(MeasuresOnDisplay::IsAC_DC() || MeasuresOnDisplay::IsDC());

        for (int i = 0; i < 6; i++)
        {
            PageMain::self->GetButton(i)->SetValue(i == range ? 1 : 0);
        }

        PageMain::self->GetButton(range)->Press();
    }


    void HightLightCurrentRange()
    {
        PageMain::self->GetButton(Range::Current())->SetValue(1);
    }
}


void PageMain::EnableZero(MeasuresOnDisplay::E meas, bool enable)
{
    if (meas == MeasuresOnDisplay::AC)
    {
        btnZeroAC_EN.SetValue(enable ? 1 : 0);

        enable ? btnZeroAC_EN.Press() : btnZeroAC_DIS.Press();

        Nextion::Text::SetVisible("tzAC", enable);
    }
    else if (meas == MeasuresOnDisplay::DC)
    {
        btnZeroDC_EN.SetValue(enable ? 1 : 0);

        enable ? btnZeroDC_EN.Press() : btnZeroDC_DIS.Press();

        Nextion::Text::SetVisible("tzDC", enable);
    }
}
