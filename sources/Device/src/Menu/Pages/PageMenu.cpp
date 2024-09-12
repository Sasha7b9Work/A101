// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Display.h"
#include "Utils/String.h"


namespace PageMenu
{
    extern ButtonMenuToggle btnIndication;
    extern Choice chChangeRange;
    extern Choice chRangeFreq;
    extern ButtonMenuPress btnCOM;
    extern ButtonMenuToggle btnSystem;

    static void AppendRemoveMeasure(TypeMeasure::E meas, bool insert);


    static ButtonMenuToggle btnFrequency("„астота", "Frequency", 1, 0, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Frequency, press);
    });

    static ButtonMenuToggle btnImin("Iмин", "Imin", 1, 1, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Min, press);
    });

    static ButtonMenuToggle btnImax("Iмакс", "Imax", 1, 2, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Max, press);
    });

    static ButtonMenuToggle btnIamp("Iамп", "Iamp", 1, 3, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Ampl, press);
    });

    static ButtonMenuToggle btnIpeak("Iпп", "Ipp", 1, 4, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Peak, press);
    });

    static void ToggleButtonsMeasures()
    {
        btnFrequency.SetToggled(TypeMeasure(TypeMeasure::Frequency).IsShown(), false);
        btnImax.SetToggled(TypeMeasure(TypeMeasure::Max).IsShown(), false);
        btnImin.SetToggled(TypeMeasure(TypeMeasure::Min).IsShown(), false);
        btnIamp.SetToggled(TypeMeasure(TypeMeasure::Ampl).IsShown(), false);
        btnIpeak.SetToggled(TypeMeasure(TypeMeasure::Peak).IsShown(), false);
    }

    static void AppendRemoveMeasure(TypeMeasure::E meas, bool insert)
    {
        if (insert)
        {
            for (int i = 0; i < 3; i++)
            {
                if (set.en_add_meas[i] == TypeMeasure::Count ||
                    set.en_add_meas[i] == meas)
                {
                    set.en_add_meas[i] = meas;
                    break;
                }
            }

            set.en_add_meas[0] = set.en_add_meas[1];
            set.en_add_meas[1] = set.en_add_meas[2];
            set.en_add_meas[2] = meas;
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                if (set.en_add_meas[i] == meas)
                {
                    set.en_add_meas[i] = TypeMeasure::Count;

                    if (i == 0)
                    {
                        std::memmove(set.en_add_meas, set.en_add_meas + 1, 2 * sizeof(TypeMeasure::E));
                    }
                    else if (i == 1)
                    {
                        set.en_add_meas[1] = set.en_add_meas[2];
                    }

                    set.en_add_meas[2] = TypeMeasure::Count;
                }
            }
        }

        ToggleButtonsMeasures();

        PageMain::RedrawAdditionMeasures();
    }


    static ButtonMenuToggle btnSettings("Ќастройки", "Settings", 0, 0, [](Item *, bool press)
    {
        if (press)
        {
            btnIndication.SetToggled(false);
            btnSystem.SetToggled(false);
        }

        chChangeRange.SetShown(press);
        chRangeFreq.SetShown(press);
        btnCOM.SetShown(press);
    });

    static const pchar name_bright[10][Lang::Count] =
    {
        {"яркость 10",  "Bright 10"},
        {"яркость 20",  "Bright 20"},
        {"яркость 30",  "Bright 30"},
        {"яркость 40",  "Bright 40"},
        {"яркость 50",  "Bright 50"},
        {"яркость 60",  "Bright 60"},
        {"яркость 70",  "Bright 70"},
        {"яркость 80",  "Bright 80"},
        {"яркость 90",  "Bright 90"},
        {"яркость 100", "Bright 100"}
    };


    static ButtonMenuPress btnBrightness("", "", 1, 2, [](Item *item, bool press)
    {
        if (press)
        {
            item->ToButtonPress()->SetText(name_bright[set.brightness / 10 - 1][Lang::RU], name_bright[set.brightness / 10 - 1][Lang::EN]);
            item->ToButtonPress()->Release();
            Display::SetBrightness();
        }
    });

    static ButtonPress btnLess("-", "-", Font::_1_GB42b, { Item::GetCoordX(2), Item::GetCoordY(2), Item::HEIGHT_MENU, Item::HEIGHT_MENU }, [](Item *, bool press)
    {
        if (press)
        {
            if (set.brightness > 10)
            {
                set.brightness -= 10;
                btnBrightness.Press();
            }
        }
    });

    static ButtonPress btnMore("+", "+", Font::_1_GB42b, { Item::GetCoordX(2) + 10 + Item::HEIGHT_MENU, Item::GetCoordY(2), Item::HEIGHT_MENU, Item::HEIGHT_MENU }, [](Item *, bool press)
    {
        if (press)
        {
            if (set.brightness < 100)
            {
                set.brightness += 10;
                btnBrightness.Press();
            }
        }
    });

    static pchar names_lang[] =
    {
        "Russian",    "–усский",
        "јнглийский", "English",
        nullptr
    };

    static Choice chLanguage("язык", "Language", (uint8 *)&set.lang, names_lang, 1, 3, [](Item *, bool press)
    {
        if (!press)
        {
            Page::Current()->Refresh();
        }
    });

    static ButtonMenuPress btnInformation("»нформаци€", "Information", 1, 4, [](Item *, bool press)
    {
        if (!press)
        {
            PageInformation::self->SetAsCurrent();
        }
    });

    ButtonMenuToggle btnSystem("—истема", "System", 0, 1, [](Item *, bool press)
    {
        if (press)
        {
            btnSettings.SetToggled(false);
            btnIndication.SetToggled(false);
        }

        btnBrightness.SetShown(press);
        btnLess.SetShown(press);
        btnMore.SetShown(press);
        chLanguage.SetShown(press);
        btnInformation.SetShown(press);
    });

    ButtonMenuToggle btnIndication("»ндикаци€", "Indication", 0, 2, [](Item *, bool press)
    {
        if (press)
        {
            btnSettings.SetToggled(false);
            btnSystem.SetToggled(false);
        }

        btnFrequency.SetShown(press);
        btnImin.SetShown(press);
        btnImax.SetShown(press);
        btnIamp.SetShown(press);
        btnIpeak.SetShown(press);
    });

    static ButtonMenuPress btnCalibration(" алибровка", "Calibration", 0, 3, [](Item *, bool press)
    {
        if (!press)
        {
            PageCalibration::self->SetAsCurrent();
        }
    });

    static ButtonMenuPress btnGraphics("√рафики", "Graphs", 0, 4, [](Item *, bool press)
    {
        if (!press)
        {
            PageGraph::self->SetAsCurrent();
        }
    });

    static ButtonMenuPress btnBack("Ќазад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
        }
    });

    static pchar names_change_range[] =
    {
        "–учной",         "Hand",
        "јвтоматический", "Auto",
        nullptr
    };

    Choice chChangeRange("¬ыбор диапазона", "Range selection", (uint8 *)&set.change_range, names_change_range, 1, 2);

    static pchar names_range_freq[] =
    {
        "1√ц - 4√ц",   "1Hz - 4Hz",
        "4√ц - 40√ц",  "4Hz - 40Hz",
        "40√ц - 5к√ц", "40Hz - 5kHz",
        nullptr
    };

    Choice chRangeFreq("ƒиапазон частоты", "Range frequency", (uint8 *)&set.range_freq, names_range_freq, 1, 3);

    ButtonMenuPress btnCOM("COM-порт", "COM-port", 1, 4, [](Item *, bool press)
    {
        if (!press)
        {
            PageCOM::self->SetAsCurrent();
        }
    });


    static Item *items[] =
    {
        &btnFrequency,
        &btnImin,
        &btnImax,
        &btnIamp,
        &btnIpeak,
        &btnSettings,
        &btnSystem,
        &btnIndication,
        &btnCalibration,
        &btnGraphics,
        &btnBack,
        &chChangeRange,
        &chRangeFreq,
        &btnCOM,
        &btnBrightness,
        &btnLess,
        &btnMore,
        &chLanguage,
        &btnInformation,
        nullptr
    };

    static void FuncOnEnter()
    {
        ToggleButtonsMeasures();

        btnSettings.SetToggled(false);
        btnIndication.SetToggled(false);
        btnSystem.SetToggled(false);

        btnBrightness.SetText(name_bright[set.brightness / 10 - 1][Lang::RU], name_bright[set.brightness / 10 - 1][Lang::EN]);
    }


    static Page pageMenu(items, FuncOnEnter, nullptr);

    Page *self = &pageMenu;
}
