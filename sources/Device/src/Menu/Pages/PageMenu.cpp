// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Display.h"


namespace PageMenu
{
    extern ButtonMenuToggle btnIndication;
    extern Choice chChangeRange;
    extern Choice chRangeFreq;
    extern ButtonMenuPress btnCOM;
    extern ButtonMenuToggle btnSystem;

    static ButtonMenuToggle btnFrequency("�������", "Frequency", 1, 0, [](Item *item, bool)
    {
        set.en_f = item->ToButtonToggle()->IsPressed();
    });

    static ButtonMenuToggle btnImin("I���", "Imin", 1, 1, [](Item *item, bool)
    {
        set.en_Imin = item->ToButtonToggle()->IsPressed();
    });

    static ButtonMenuToggle btnImax("I����", "Imax", 1, 2, [](Item *item, bool)
    {
        set.en_Imax = item->ToButtonToggle()->IsPressed();
    });

    static ButtonMenuToggle btnIamp("I���", "Iamp", 1, 3, [](Item *item, bool)
    {
        set.en_Iampl = item->ToButtonToggle()->IsPressed();
    });

    static ButtonMenuToggle btnIpeak("I��", "Ipp", 1, 4, [](Item *item, bool)
    {
        set.en_Ipp = item->ToButtonToggle()->IsPressed();
    });

    static ButtonMenuToggle btnSettings("���������", "Settings", 0, 0, [](Item *, bool press)
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

    static ButtonMenuPress btnBrightness("�������", "Brightness", 1, 2);

    static pchar names_lang[] =
    {
        "Russian",    "�������",
        "����������", "English",
        nullptr
    };

    static Choice chLanguage("����", "Language", (uint8 *)&set.lang, names_lang, 1, 3, [](Item *, bool press)
    {
        if (!press)
        {
            Page::Current()->Refresh();
        }
    });

    static ButtonMenuPress btnInformation("����������", "Information", 1, 4);

    ButtonMenuToggle btnSystem("�������", "System", 0, 1, [](Item *, bool press)
    {
        if (press)
        {
            btnSettings.SetToggled(false);
            btnIndication.SetToggled(false);
        }

        btnBrightness.SetShown(press);
        chLanguage.SetShown(press);
        btnInformation.SetShown(press);
    });

    ButtonMenuToggle btnIndication("���������", "Indication", 0, 2, [](Item *, bool press)
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

    static ButtonMenuPress btnCalibration("����������", "Calibration", 0, 3);

    static ButtonMenuPress btnGraphics("�������", "Graphs", 0, 4);

    static ButtonMenuPress btnBack("�����", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
            set.Save();
        }
    });

    static pchar names_change_range[] =
    {
        "������",         "Hand",
        "��������������", "Auto",
        nullptr
    };

    Choice chChangeRange("����� ���������", "Range selection", (uint8 *)&set.change_range, names_change_range, 1, 2);

    static pchar names_range_freq[] =
    {
        "1�� - 4��",   "1Hz - 4Hz",
        "4�� - 40��",  "4Hz - 40Hz",
        "40�� - 5���", "40Hz - 5kHz",
        nullptr
    };

    Choice chRangeFreq("�������� �������", "Range frequency", (uint8 *)&set.range_freq, names_range_freq, 1, 3);

    ButtonMenuPress btnCOM("COM-����", "COM-port", 1, 4, [](Item *, bool press)
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
        &chLanguage,
        &btnInformation,
        nullptr
    };

    static void FuncOnEnter()
    {
        btnFrequency.SetToggled(set.en_f);
        btnImax.SetToggled(set.en_Imax);
        btnImin.SetToggled(set.en_Imin);
        btnIamp.SetToggled(set.en_Iampl);
        btnIpeak.SetToggled(set.en_Ipp);

        btnSettings.SetToggled(false);
        btnIndication.SetToggled(false);
        btnSystem.SetToggled(false);

//        chChangeRange.SetShown(false);
//        chRangeFreq.SetShown(false);
//        btnCOM.SetShown(false);
    }


    static Page pageMenu(items, FuncOnEnter, nullptr);

    Page *self = &pageMenu;
}
