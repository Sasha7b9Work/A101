// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Display.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include <cstdio>
#include <cstring>


namespace PageMain
{
    namespace Star
    {
        static uint time_start = TIME_MS;     // Время перехода на данную страницу

        static bool prev_enabled = false;       // В таком состоянии звёздочка была

        static void Reset();
    }

    extern ButtonPress btnMenu;
    extern ButtonPress btnAC_DC;

    LabelMeasure wndDC{ TypeMeasure::DC, SizeMeasure::Big, 0, 60 };
    LabelMeasure wndAC{ TypeMeasure::AC, SizeMeasure::Big, 0, 220 };

    static const int y_meas = 355;

    static LabelMeasure wndAMPL{ TypeMeasure::Ampl,      SizeMeasure::Small, 0, y_meas };
    static LabelMeasure wndPEAK{ TypeMeasure::Peak,      SizeMeasure::Small, 0, y_meas };
    static LabelMeasure wndMIN{  TypeMeasure::Min,       SizeMeasure::Small, 0, y_meas };
    static LabelMeasure wndMAX{  TypeMeasure::Max,       SizeMeasure::Small, 0, y_meas };
    static LabelMeasure wndFREQ{ TypeMeasure::Frequency, SizeMeasure::Small, 0, y_meas };

    static LabelMeasure *labels[5] =
    {
        &wndAMPL,
        &wndPEAK,
        &wndMIN,
        &wndMAX,
        &wndFREQ
    };

    static void SetTitleButtonAC_DC()
    {
        if (set.meas_on_display.Current() == MeasuresOnDisplay::DC)
        {
            btnAC_DC.SetText("DC", "DC");
        }
        else if (set.meas_on_display.Current() == MeasuresOnDisplay::AC)
        {
            btnAC_DC.SetText("AC", "AC");
        }
        else
        {
            btnAC_DC.SetText("AC+DC", "AC+DC");
        }
    }

    void RedrawAllMeasures()
    {
        {                           // Дополнительные измерения

            static TypeMeasure::E prev[3] = { TypeMeasure::Count, TypeMeasure::Count, TypeMeasure::Count };

            if (std::memcmp(prev, set.en_add_meas, 3 * sizeof(TypeMeasure::E)) != 0)
            {
                std::memcpy(prev, set.en_add_meas, 3 * sizeof(TypeMeasure::E));

                int16 x = 5;

                for (int i = 0; i < 5; i++)
                {
                    labels[i]->SetEnabled(false);
                }

                for (int i = 0; i < 3; i++)
                {
                    if (set.en_add_meas[i] == TypeMeasure::Count)
                    {
                        return;
                    }

                    labels[set.en_add_meas[i] - 2]->SetCoord({ x, y_meas });
                    labels[set.en_add_meas[i] - 2]->SetEnabled(true);

                    x += (750 / 3 + 15);
                }
            }
        }

        {                           // Основные измерения
            static MeasuresOnDisplay::E prev = MeasuresOnDisplay::Count;

            if (set.meas_on_display.Current() != prev)
            {
                prev = set.meas_on_display.Current();

                wndDC.SetEnabled(false);
                wndAC.SetEnabled(false);

                if (set.meas_on_display.IsAC_DC())
                {
                    wndDC.SetCoord({ 0, 60 });
                    wndAC.SetCoord({ 0, 220 });
                    wndDC.SetEnabled(true);
                    wndAC.SetEnabled(true);
                }
                else if (set.meas_on_display.IsAC())
                {
                    wndAC.SetCoord({ 0, 150 });
                    wndAC.SetEnabled(true);
                }
                else
                {
                    wndDC.SetCoord({ 0, 150 });
                    wndDC.SetEnabled(true);
                }
            }
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

    static int WidthHiButton()
    {
        return Display::WIDTH / 6 - 10;
    }

    static int CoordXHiButton(int i)
    {
        if (i == 5)
        {
            return Display::WIDTH - WidthHiButton();
        }

        return i * WidthHiButton();
    }

    static ButtonPress btnZero("Уст. 0", "Set 0", Font::_5_GB30b, { CoordXHiButton(3), 4, WidthHiButton(), 37 }, [](Item *, bool press)
    {
        if (press)
        {
            Ampermeter::ZeroDC::FloatingZero::Process();
        }
    }, 1);

    ButtonToggle btnAVP("АВП", "AVP", Font::_5_GB30b, { CoordXHiButton(3), 4, WidthHiButton(), 37 }, [](Item *, bool press)
    {
        if (press)
        {
            Ampermeter::AVP::Enable();
        }
        else
        {
            Ampermeter::AVP::Disable();
        }
    }, 1);

    ButtonToggle btnZeroDC("Нуль DC", "Zero DC", Font::_5_GB30b, { CoordXHiButton(1), 4, WidthHiButton(), 37 }, [](Item *item, bool)
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

    ButtonToggle btnZeroAC("Нуль AC", "Zero AC", Font::_5_GB30b, { CoordXHiButton(2), 4, WidthHiButton(), 37 }, [](Item *item, bool)
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
        btnAVP.SetShown(new_range < 4);
    }

    static void FuncOnEnter()
    {
        Star::Reset();

        SetTitleButtonAC_DC();

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

        RedrawAllMeasures();

        wndMIN.SetEnabled(TypeMeasure(TypeMeasure::Min).IsShown());
        wndMAX.SetEnabled(TypeMeasure(TypeMeasure::Max).IsShown());
        wndAMPL.SetEnabled(TypeMeasure(TypeMeasure::Ampl).IsShown());
        wndPEAK.SetEnabled(TypeMeasure(TypeMeasure::Peak).IsShown());
        wndFREQ.SetEnabled(TypeMeasure(TypeMeasure::Frequency).IsShown());

        btnAC_DC.SetExtendedHeightPress();
        btnZeroDC.SetExtendedHeightPress();
        btnZeroAC.SetExtendedHeightPress();
        btnAVP.SetExtendedHeightPress();
        btnZero.SetExtendedHeightPress();

        SetMeasuresOnDisplay(set.meas_on_display.Current(), true);
    }

    static void FuncDraw()
    {
//        Star::Draw();

        wndDC.SetMeasure(Calculator::GetMeasureDC(), Range::Current());
        wndAC.SetMeasure(Calculator::GetMeasureAC(), Range::Current());

        wndAMPL.SetMeasure(Calculator::GetMeasureAmplSteady(), Range::Current());
        wndPEAK.SetMeasure(Calculator::GetMeasurePeak(), Range::Current());
        wndMIN.SetMeasure(Calculator::GetMeasureMinSteady(), Range::Current());
        wndMAX.SetMeasure(Calculator::GetMeasureMaxSteady(), Range::Current());

        Measure frequency = Calculator::GetMeasureFrequency();

        pchar message_ru = nullptr;
        pchar message_en = nullptr;

        if (SampleRate::InRange(frequency.value_abs, &message_ru, &message_en))
        {
            wndFREQ.SetMeasure(Calculator::GetMeasureFrequency(), Range::Current());
        }
        else
        {
            wndFREQ.SetMeasure(message_ru, message_en);
        }
    }

    void SetRange(int range)
    {
        PageMain::self->GetItem(range)->Press();

        for (int i = 0; i < 6; i++)
        {
            if (i != range)
            {
                PageMain::self->GetItem(i)->Release();
            }
        }
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
                        PageMain::self->GetItem(i)->ToButtonToggle()->SetToggled(false, false);
                    }
                }

                Ampermeter::AVP::Disable();
                btnAVP.SetToggled(false, false);
                Range::Set(range);
            }
        }
        else if (Range::Current() == range)
        {
            item->ToButtonToggle()->SetToggled(true, false);
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

    void SetMeasuresOnDisplay(MeasuresOnDisplay::E meas, bool force)
    {
        if (force || meas != set.meas_on_display.Current())
        {
            set.meas_on_display.Set(meas);

            SetTitleButtonAC_DC();

            RedrawAllMeasures();

            bool show_AC = (meas == MeasuresOnDisplay::AC || meas == MeasuresOnDisplay::AC_DC);
            bool show_DC = (meas == MeasuresOnDisplay::DC || meas == MeasuresOnDisplay::AC_DC);

            btnZeroAC.SetShown(show_AC);
            btnZeroAC.SetEnabled(show_AC);
            btnZeroDC.SetShown(show_DC);
            btnZeroDC.SetEnabled(show_DC);
        }
    }

    ButtonPress btnAC_DC("AC+DC", "AC+DC", Font::_5_GB30b, { CoordXHiButton(0), 4, WidthHiButton(), 37 }, [](Item *, bool press)
    {
        if (press)
        {
            int meas = set.meas_on_display.Current() + 1;
            if (meas == MeasuresOnDisplay::Count)
            {
                meas = 0;
            }
            SetMeasuresOnDisplay((MeasuresOnDisplay::E)meas);
        }
    }, 1);

    ButtonPress btnMenu("Меню", "Menu", Font::_5_GB30b, { CoordXHiButton(5), 4, WidthHiButton(), 37 }, [](Item *, bool press)           // Menu
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

        &btnAC_DC,  // /
        &btnZeroDC, // |
        &btnZeroAC, // | Кнопки вверху экрана
        &btnAVP,    // |
        &btnZero,   // /

        &wndDC,
        &wndAC,
        &wndAMPL,
        &wndPEAK,
        &wndMIN,
        &wndMAX,
        &wndFREQ,
        &btnMenu,
        nullptr
    };

    static Page pageMain(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMain;

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
    }
    else if (meas == MeasuresOnDisplay::DC)
    {
        enable ? btnZeroDC.Press() : btnZeroDC.Release();
    }
}


void PageMain::Star::Reset()
{
    time_start = TIME_MS;
    prev_enabled = false;
}


void PageMain::Star::Draw()
{
    uint full_time = (TIME_MS - time_start) / 1000;

    bool enabled = (full_time % 2) == 0;

    if (enabled != prev_enabled)
    {
        if (enabled)
        {
            Nextion::DrawStarForStar();
        }
        else
        {
            Nextion::DrawSpaceForStar();
        }

        prev_enabled = enabled;
    }
}
