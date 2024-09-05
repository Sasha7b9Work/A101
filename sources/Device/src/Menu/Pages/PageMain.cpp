// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Display.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include <cstdio>


namespace PageMain
{
    Label labelZeroDC{ true, "ноль", "zero", { 23, 170, 90, 50 }, Font::_1 };
    Label labelZeroAC{ true, "ноль", "zero", { 23, 260, 90, 50 }, Font::_1 };

    LabelMeasure wndDC{ TypeMeasure::DC, SizeMeasure::Big, 134, 143 };
    LabelMeasure wndAC{ TypeMeasure::AC, SizeMeasure::Big, 134, 233 };

    static LabelMeasure wndAMPL{ TypeMeasure::Ampl, SizeMeasure::Small, 433, 27 };
    static LabelMeasure wndPEAK{ TypeMeasure::Peak, SizeMeasure::Small, 433, 70 };
    static LabelMeasure wndMIN{ TypeMeasure::Min,   SizeMeasure::Small, 171, 70 };
    static LabelMeasure wndMAX{ TypeMeasure::Max,   SizeMeasure::Small, 171, 27 };

    static void ResetAllMeasures()
    {
        wndDC.Reset();
        wndAC.Reset();
        wndAMPL.Reset();
        wndPEAK.Reset();
        wndMIN.Reset();
        wndMAX.Reset();
    }

    static ButtonPress btnZero("Уст. 0", "Set 0", Font::_1, { 669, 321, 127, 74 }, [](Item *)
    {
        Ampermeter::ZeroDC::FloatingZero::Process();
    });

    void OnEventChangeRange(int new_range)
    {
        ResetAllMeasures();

        btnZero.SetShown(new_range >= 4);
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
    }

    static void FuncDraw()
    {
        Display::LabelStar::Update();

        wndDC.SetMeasure(Ampermeter::GetDC(), Range::Current());

        wndAC.SetMeasure(Ampermeter::GetAC(), Range::Current());

        Measure ampl = Ampermeter::GetAmpl();
        wndAMPL.SetMeasure(ampl, Range::Current());

        wndPEAK.SetMeasure(Ampermeter::GetPeak(), Range::Current());
        wndMIN.SetMeasure(Ampermeter::GetMin(), Range::Current());
        wndMAX.SetMeasure(Ampermeter::GetMax(), Range::Current());
    }

    // Вызывается при нажатии кнопки
    static void FuncOnRange(int range)
    {
        if (Range::Current() != range)
        {
            for (int i = 0; i < 6; i++)
            {
                if (i != range)
                {
                    PageMain::self->GetItem(i)->ToButtonRange()->SetValue(0);
                }
            }

            Ampermeter::AVP::Disable();
            Range::Set(range);
        }
    }

    static ButtonRange btn2mA("2 мА", "2 mA", 4, 402, [](Item *)
    {
        FuncOnRange(0);
    });


    static ButtonRange btn20mA("20 мА", "20 mA", 137, 402, [](Item *)
    {
        FuncOnRange(1);
    });

    static ButtonRange btn200mA("200 мА", "200 mA", 270, 402, [](Item *)
    {
        FuncOnRange(2);
    });

    static ButtonRange btn2A("2 А", "2 A", 403, 402, [](Item *)
    {
        FuncOnRange(3);
    });

    static ButtonRange btn20A("20 А", "20 A", 536, 402, [](Item *)
    {
        FuncOnRange(4);
    });

    static ButtonRange btn50A("50 А", "50 A", 669, 402, [](Item *)
    {
        FuncOnRange(5);
    });

    static ButtonPress btnAC_DC("AC + DC", "AC + DC", Font::_1, { 6, 4, 127, 74 }, [](Item *)
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC_DC);
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC);
        MeasuresOnDisplay::Set(MeasuresOnDisplay::DC);
    });

    static ButtonPress btnCalibration("Калибр.", "Calibr.", Font::_0, { 5, 84, 127, 74 }, [](Item *)
    {
        PageCalibration::self->SetAsCurrent();
    });

    static ButtonPress btnSettings("Настройки", "Settings", Font::_0, { 463, 4, 188, 74 }, [](Item *)
    {
        PageSettings::self->SetAsCurrent();
    });

    static ButtonToggle btnZeroDC("Ноль DC", "Zero DC", Font::_0, { 660, 84, 136, 74 }, [](Item *item)
    {
        ButtonToggle *btn = (ButtonToggle *)item;

        if (btn->IsPressed())
        {
            Ampermeter::ZeroDC::Enable();
        }
        else
        {
            Ampermeter::ZeroDC::Disable();
        }
    });

    static ButtonToggle btnZeroAC("Ноль AC", "Zero AC", Font::_0, { 660, 164, 136, 74 }, [](Item *item)
    {
        ButtonToggle *btn = (ButtonToggle *)item;

        if (btn->IsPressed())
        {
            Ampermeter::ZeroAC::Enable();
        }
        else
        {
            Ampermeter::ZeroAC::Disable();
        }
    });

    static ButtonPress btnGraphics("Графики", "Graphs", Font::_0, { 660, 243, 136, 74}, [](Item *)    // Доступ к графикам
    {
        PageGraph::self->SetAsCurrent();
    });

    static ButtonToggle btnMeasures("Измерения", "Measures", Font::_5, { 463, 84, 188, 74 }, [](Item *)
    {

    });

    static ButtonToggle btnMAX("Iмакс", "Imax", Font::_0, { 105, 165, 133, 74 }, [](Item *) {});        // Imax

    static ButtonToggle btnAMP("Iамп", "Iamp", Font::_0, { 381, 165, 133, 74 }, [](Item *) {});         // Iampl

    static ButtonToggle btnMIN("Iмин", "Imin", Font::_0, { 243, 165, 133, 74 }, [](Item *) {});         // Imin

    static ButtonToggle btnPEAK("Iпп", "Ipp", Font::_0, { 519, 165, 133, 74},  [](Item *) {});          // Ipp

    static ButtonPress btnMenu("Меню", "Menu", Font::_1, { 660, 4, 136, 74 }, [](Item *)                // Menu
    {

    });

    static Item *items[] =
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
        &btnGraphics,
        &btnMAX,
        &btnAMP,
        &btnMIN,
        &btnPEAK,
        &btnSettings,
        &btnZero,
        &wndDC,
        &wndAC,
        &btnMeasures,
        &wndAMPL,
        &wndPEAK,
        &wndMIN,
        &wndMAX,
        &labelZeroAC,
        &labelZeroDC,
        &btnMenu,
        nullptr
    };

    static Page pageMain(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMain;

    void SetRange(MeasuresOnDisplay::E meas, int range)
    {
        MeasuresOnDisplay::Set(meas);

        if (MeasuresOnDisplay::IsAC_DC())
        {
            btnAC_DC.SetText("AC+DC", "AC+DC");
        }
        else if (MeasuresOnDisplay::IsAC())
        {
            btnAC_DC.SetText("AC", "AC");
        }
        else if (MeasuresOnDisplay::IsDC())
        {
            btnAC_DC.SetText("DC", "DC");
        }

        wndAC.SetShown(MeasuresOnDisplay::IsAC_DC() || MeasuresOnDisplay::IsAC());
        wndDC.SetShown(MeasuresOnDisplay::IsAC_DC() || MeasuresOnDisplay::IsDC());

        for (int i = 0; i < 6; i++)
        {
            PageMain::self->GetItem(i)->ToButtonRange()->SetValue(i == range ? 1 : 0);
        }

        PageMain::self->GetItem(range)->ToButtonRange()->Press();
    }


    void HightLightCurrentRange()
    {
        PageMain::self->GetItem(Range::Current())->ToButtonRange()->SetValue(1);
    }
}


void PageMain::EnableZero(MeasuresOnDisplay::E meas, bool enable)
{
    if (meas == MeasuresOnDisplay::AC)
    {
        btnZeroAC.SetValue(enable ? 1 : 0);

        PageMain::labelZeroAC.SetShown(enable);
    }
    else if (meas == MeasuresOnDisplay::DC)
    {
        btnZeroDC.SetValue(enable ? 1 : 0);

        PageMain::labelZeroDC.SetShown(enable);
    }
}


void PageMain::Init()
{
    btn2A.SetValue(1);
}
