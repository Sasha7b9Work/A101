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
    extern ButtonPress btnMenu;

//    Label labelZeroDC{ true, "ноль", "zero", { 23, 170, 90, 50 }, Font::_1_GB42b };
//    Label labelZeroAC{ true, "ноль", "zero", { 23, 260, 90, 50 }, Font::_1_GB42b };

    LabelMeasure wndDC{ TypeMeasure::DC, SizeMeasure::Big, 0, 60 };
    LabelMeasure wndAC{ TypeMeasure::AC, SizeMeasure::Big, 0, 220 };

    static LabelMeasure wndAMPL{ TypeMeasure::Ampl, SizeMeasure::Small, 0, 400 };
    static LabelMeasure wndPEAK{ TypeMeasure::Peak, SizeMeasure::Small, 0, 400 };
    static LabelMeasure wndMIN{ TypeMeasure::Min,   SizeMeasure::Small, 0, 400 };
    static LabelMeasure wndMAX{ TypeMeasure::Max,   SizeMeasure::Small, 0, 400 };
    static LabelMeasure wndFREQ{ TypeMeasure::Frequency, SizeMeasure::Small, 0, 400 };

    static LabelMeasure *labels[5] =
    {
        &wndAMPL,
        &wndPEAK,
        &wndMIN,
        &wndMAX,
        &wndFREQ
    };

    void RedrawAdditionMeasures()
    {
        int16 x = 25;

        for (int i = 0; i < 3; i++)
        {
            if (set.en_add_meas[i] == TypeMeasure::Count)
            {
                return;
            }

            labels[set.en_add_meas[i] - 2]->SetCoord({ x, 350 });

            x += (750 / 3);
        }
    }

    static void ResetAllMeasures()
    {
        wndDC.Reset();
        wndAC.Reset();
        wndAMPL.Reset();
        wndPEAK.Reset();
        wndMIN.Reset();
        wndMAX.Reset();
    }

    static ButtonPress btnZero("Уст. 0", "Set 0", Font::_5_GB30b, { 200, 4, 120, 37 }, [](Item *, bool press)
    {
        if (press)
        {
            Ampermeter::ZeroDC::FloatingZero::Process();
        }
    }, 1);

    ButtonToggle btnZeroDC("Нуль DC", "Zero DC", Font::_5_GB30b, { 350, 4, 120, 37 }, [](Item *item, bool)
    {
        ButtonToggle *btn = item->ToButtonToggle();

        if (btn->IsPressed())
        {
            Ampermeter::ZeroDC::Enable();
        }
        else
        {
            Ampermeter::ZeroDC::Disable();
        }
    }, 1);

    ButtonToggle btnZeroAC("Нуль AC", "Zero AC", Font::_5_GB30b, { 500, 4, 120, 37 }, [](Item *item, bool)
    {
        ButtonToggle *btn = item->ToButtonToggle();

        if (btn->IsPressed())
        {
            Ampermeter::ZeroAC::Enable();
        }
        else
        {
            Ampermeter::ZeroAC::Disable();
        }

    }, 1);

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

        RedrawAdditionMeasures();

        wndMIN.SetShown(TypeMeasure(TypeMeasure::Min).IsShown());
        wndMAX.SetShown(TypeMeasure(TypeMeasure::Max).IsShown());
        wndAMPL.SetShown(TypeMeasure(TypeMeasure::Ampl).IsShown());
        wndPEAK.SetShown(TypeMeasure(TypeMeasure::Peak).IsShown());
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
    static void FuncOnRange(Item *item, int range, bool press)
    {
        if (press)
        {
            item->Refresh();

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

    static ButtonPress btnAC_DC("AC+DC", "AC + DC", Font::_5_GB30b, { 6, 4, 100, 37 }, [](Item *, bool)
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC_DC);
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC);
        MeasuresOnDisplay::Set(MeasuresOnDisplay::DC);
    }, 1);

    ButtonPress btnMenu("Меню", "Menu", Font::_5_GB30b, { 696, 4, 100, 37 }, [](Item *, bool press)           // Menu
    {
        if (!press)
        {
            PageMenu::self->SetAsCurrent();
        }
    }, 1);

    static Item *items[] =
    {
        &btn2mA,    // /
        &btn20mA,   // |
        &btn200mA,  // | Эти кнопки всегда должны быть первыми
        &btn2A,     // |
        &btn20A,    // |
        &btn50A,    // /

        &btnAC_DC,
        &btnZeroDC,
        &btnZeroAC,
        &wndDC,
        &wndAC,
        &wndAMPL,
        &wndPEAK,
        &wndMIN,
        &wndMAX,
//        &labelZeroAC,
//        &labelZeroDC,
        &btnMenu,
        &btnZero,
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

//        PageMain::labelZeroAC.SetShown(enable);
    }
    else if (meas == MeasuresOnDisplay::DC)
    {
        enable ? btnZeroDC.Press() : btnZeroDC.Release();

//        PageMain::labelZeroDC.SetShown(enable);
    }
}
