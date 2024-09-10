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
    extern ButtonToggle btnZeroAC;
    extern ButtonToggle btnZeroDC;
    extern ButtonPress btnGraphics;
    extern ButtonPress btnMenu;
    extern ButtonPress btnSettings;
    extern ButtonPress btnCalibration;

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

    static ButtonPress btnZero("Уст. 0", "Set 0", Font::_1, { 669, 321, 127, 74 }, [](Item *, bool)
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

        bool is_show = btnMenu.IsPressed();

        btnZeroDC.SetShown(is_show);
        btnZeroAC.SetShown(is_show);
        btnGraphics.SetShown(is_show);
        btnSettings.SetShown(is_show);
        btnCalibration.SetShown(is_show);

        wndMIN.SetShown(set.en_Imin);
        wndMAX.SetShown(set.en_Imax);
        wndAMPL.SetShown(set.en_Iampl);
        wndPEAK.SetShown(set.en_Ipp);
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
    static void FuncOnRange(Item *, int range, bool press)
    {
        if (press)
        {
            if (Range::Current() != range)
            {
                for (int i = 0; i < 6; i++)
                {
                    if (i != range)
                    {
                        PageMain::self->GetItem(i)->ToButtonToggle()->SetToggled(false);
                    }
                }

                Ampermeter::AVP::Disable();
                Range::Set(range);
            }
        }
    }

    static ButtonRange btn2mA("2 мА", "2 mA", 4, 402, [](Item *item, bool press)
    {
        FuncOnRange(item, 0, press);
    });


    static ButtonRange btn20mA("20 мА", "20 mA", 137, 402, [](Item *item, bool press)
    {
        FuncOnRange(item, 1, press);
    });

    static ButtonRange btn200mA("200 мА", "200 mA", 270, 402, [](Item *item, bool press)
    {
        FuncOnRange(item, 2, press);
    });

    static ButtonRange btn2A("2 А", "2 A", 403, 402, [](Item *item, bool press)
    {
        FuncOnRange(item, 3, press);
    });

    static ButtonRange btn20A("20 А", "20 A", 536, 402, [](Item *item, bool press)
    {
        FuncOnRange(item, 4, press);
    });

    static ButtonRange btn50A("50 А", "50 A", 669, 402, [](Item *item, bool press)
    {
        FuncOnRange(item, 5, press);
    });

    static ButtonPress btnAC_DC("AC+DC", "AC + DC", Font::_1, { 6, 4, 127, 74 }, [](Item *, bool)
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC_DC);
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC);
        MeasuresOnDisplay::Set(MeasuresOnDisplay::DC);
    });

    ButtonPress btnCalibration("Калибр.", "Calibr.", Font::_0, { 5, 84, 127, 74 }, [](Item *, bool)
    {
        PageCalibration::self->SetAsCurrent();
    });

    ButtonPress btnSettings("Настройки", "Settings", Font::_0, { 463, 4, 188, 74 }, [](Item *, bool)
    {
        PageSettings::self->SetAsCurrent();
    });

    ButtonToggle btnZeroDC("Ноль DC", "Zero DC", Font::_0, { 660, 84, 136, 74 }, [](Item *item, bool)
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

    ButtonToggle btnZeroAC("Ноль AC", "Zero AC", Font::_0, { 660, 164, 136, 74 }, [](Item *item, bool)
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

    ButtonPress btnGraphics("Графики", "Graphs", Font::_0, { 660, 243, 136, 74}, [](Item *, bool)    // Доступ к графикам
    {
        PageGraph::self->SetAsCurrent();
    });

    ButtonPress btnMenu("Меню", "Menu", Font::_1, { 660, 4, 136, 74 }, [](Item *, bool press)           // Menu
    {
        if (!press)
        {
            PageMenu::self->SetAsCurrent();
        }
    });

    static Item *items[] =
    {
        &btn2mA,    // /
        &btn20mA,   // |
        &btn200mA,  // | Эти кнопки всегда должны быть первыми
        &btn2A,     // |
        &btn20A,    // |
        &btn50A,    // /

        &btnAC_DC,
        &btnCalibration,
        &btnZeroDC,
        &btnZeroAC,
        &btnGraphics,
        &btnSettings,
        &btnZero,
        &wndDC,
        &wndAC,
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

        PageMain::self->GetItem(range)->Press();

        for (int i = 0; i < 6; i++)
        {
            if (i != range)
            {
                PageMain::self->GetItem(i)->Release();
            }
        }
    }


    void HightLightCurrentRange()
    {
        PageMain::self->GetItem(Range::Current())->Press();
    }
}


void PageMain::EnableZero(MeasuresOnDisplay::E meas, bool enable)
{
    if (meas == MeasuresOnDisplay::AC)
    {
        enable ? btnZeroAC.Press() : btnZeroAC.Release();

        PageMain::labelZeroAC.SetShown(enable);
    }
    else if (meas == MeasuresOnDisplay::DC)
    {
        enable ? btnZeroDC.Press() : btnZeroDC.Release();

        PageMain::labelZeroDC.SetShown(enable);
    }
}
