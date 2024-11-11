// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Display.h"
#include "Utils/String.h"


namespace PageMenu
{
    extern ButtonMenuToggle btnIndication;
    extern Choice chRangeFreq;
    extern ButtonMenuPress btnCOM;
    extern ButtonMenuPress btnCalibration;
    extern ButtonMenuPress btnGraphics;

    static void AppendRemoveMeasure(TypeMeasure::E meas, bool insert);


    static ButtonMenuToggle btnFrequency("F", "F", 1, 0, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Frequency, press);
    });

    static ButtonMenuToggle btnImin("I мин", "I min", 1, 1, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Min, press);
    });

    static ButtonMenuToggle btnImax("I макс", "I max", 1, 2, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Max, press);
    });

    static ButtonMenuToggle btnIamp("I амп", "I amp", 1, 3, [](Item *, bool press)
    {
        AppendRemoveMeasure(TypeMeasure::Ampl, press);
    });

    static ButtonMenuToggle btnIpeak("I пп", "I pp", 1, 4, [](Item *, bool press)
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

            if (!TypeMeasure(meas).IsShown())
            {
                set.en_add_meas[0] = set.en_add_meas[1];
                set.en_add_meas[1] = set.en_add_meas[2];
                set.en_add_meas[2] = meas;
            }
        }
        else
        {
            for (int i = 0; i < 3; i++)
            {
                if (set.en_add_meas[i] == meas)
                {
                    if (i == 0)
                    {
                        std::memmove(set.en_add_meas, set.en_add_meas + 1, 2 * sizeof(TypeMeasure::E));
                    }
                    else if (i == 1)
                    {
                        set.en_add_meas[1] = set.en_add_meas[2];
                    }

                    set.en_add_meas[2] = TypeMeasure::Count;

                    break;
                }
            }
        }

        ToggleButtonsMeasures();

        PageMain::RedrawAllMeasures();
    }


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
        "English",    "јнглийский",
        nullptr,      nullptr
    };

    static Choice chLanguage((Lang::IsRU() ? "Language" : "язык"), (Lang::IsRU() ? "язык" : "Language"),
        (uint8 *)&set.lang,
        names_lang,
        1, 3, [](Item *item, bool press)
    {
        if (!press)
        {
            ((ButtonCommon *)item)->SetText((Lang::IsRU() ? "Language" : "язык"), (Lang::IsRU() ? "язык" : "Language"));
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

    static ButtonMenuToggle btnSettings("Ќастройки", "Settings", 0, 0, [](Item *, bool press)
    {
        if (press)
        {
            btnIndication.SetToggled(false);
            btnIndication.SetInactiveColor();
            btnCalibration.SetInactiveColor();
            btnGraphics.SetInactiveColor();
        }
        else
        {
            btnIndication.SetActiveColor();
            btnCalibration.SetActiveColor();
            btnGraphics.SetActiveColor();
        }

        chRangeFreq.SetShown(press);
        btnCOM.SetShown(press);
        btnBrightness.SetShown(press);
        btnLess.SetShown(press);
        btnMore.SetShown(press);
        chLanguage.SetShown(press);
        btnInformation.SetShown(press);
    });

    ButtonMenuToggle btnIndication("ƒоп. измер.", "Add. dim.", 0, 1, [](Item *, bool press)
    {
        if (press)
        {
            btnSettings.SetToggled(false);
            btnSettings.SetInactiveColor();
            btnCalibration.SetInactiveColor();
            btnGraphics.SetInactiveColor();
        }
        else
        {
            btnSettings.SetActiveColor();
            btnCalibration.SetActiveColor();
            btnGraphics.SetActiveColor();
        }

        press ? btnSettings.SetInactiveColor() : btnSettings.SetActiveColor();

        btnFrequency.SetShown(press);
        btnImin.SetShown(press);
        btnImax.SetShown(press);
        btnIamp.SetShown(press);
        btnIpeak.SetShown(press);
    });

    ButtonMenuPress btnCalibration(" алибровка", "Calibration", 0, 2, [](Item *, bool press)
    {
        if (!press)
        {
            PageCalibration::self->SetAsCurrent();
        }
    });

    ButtonMenuPress btnGraphics("√рафики", "Graphs", 0, 3, [](Item *, bool press)
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

    static pchar names_range_freq[] =
    {
        "40 √ц - 5 к√ц",  "40 Hz - 5 kHz",
        "4 √ц - 500 √ц",  "4 Hz - 500 Hz",
        "0.4 √ц - 50 √ц", "0.4 Hz - 50 Hz",
        nullptr,          nullptr
    };

    static uint8 sample_rate = SampleRate::_10us;

    Choice chRangeFreq("ƒиапазон", "Range", &sample_rate, names_range_freq, 1, 1, [](Item *, bool press)
    {
        if (!press)
        {
            SampleRate::Set((SampleRate::E)sample_rate);
        }
    });

    ButtonMenuPress btnCOM("COM-порт", "COM-port", 1, 0, [](Item *, bool press)
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
        &btnIndication,
        &btnCalibration,
        &btnGraphics,
        &btnBack,
        &btnCOM,
        &chRangeFreq,
        &btnBrightness,
        &chLanguage,
        &btnInformation,
        &btnLess,
        &btnMore,
        nullptr
    };

    static void FuncOnEnter()
    {
        ToggleButtonsMeasures();

        btnSettings.SetToggled(false);
        btnIndication.SetToggled(false);

        btnBrightness.SetText(name_bright[set.brightness / 10 - 1][Lang::RU], name_bright[set.brightness / 10 - 1][Lang::EN]);
    }


    static Page pageMenu(items, FuncOnEnter, nullptr);

    Page *self = &pageMenu;
}
