// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Display.h"


namespace PageMenu
{
    extern ButtonMenuToggle btnIndication;
    extern Choice chChangeRange;
    extern Choice chRange;
    extern ButtonMenuPress btnCOM;

    static void FuncDraw()
    {

    }


    static ButtonMenuToggle btnFrequency("Частота", "Frequency", 1, 0, [](Item *item, bool press)
    {
        if (press)
        {
            set.en_f = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonMenuToggle btnImin("Iмин", "Imin", 1, 1, [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Imin = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonMenuToggle btnImax("Iмакс", "Imax", 1, 2, [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Imax = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonMenuToggle btnIamp("Iамп", "Iamp", 1, 3, [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Iampl = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonMenuToggle btnIpeak("Iпп", "Ipp", 1, 4, [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Ipp = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonMenuToggle btnSettings("Настройки", "Settings", 0, 0, [](Item *, bool press)
    {
        if (press)
        {
            btnIndication.SetToggled(false);
        }

        chChangeRange.SetShown(press);
        chRange.SetShown(press);
        btnCOM.SetShown(press);
    });

    static ButtonMenuPress btnSystem("Система", "System", 0, 1, [](Item *, bool)
    {
    });

    ButtonMenuToggle btnIndication("Индикация", "Indication", 0, 2, [](Item *, bool press)
    {
        if (press)
        {
            btnSettings.SetToggled(false);
        }

        btnFrequency.SetShown(press);
        btnImin.SetShown(press);
        btnImax.SetShown(press);
        btnIamp.SetShown(press);
        btnIpeak.SetShown(press);
    });

    static ButtonMenuPress btnCalibration("Калибровка", "Calibration", 0, 3, [](Item *, bool)
    {
    });

    static ButtonMenuPress btnGraphics("Графики", "Graphs", 0, 4, [](Item *, bool)
    {
    });

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
        }
    });

    static pchar choices_change_range[] =
    {
        "Ручной",         "Hand",
        "Автоматический", "Auto",
        nullptr
    };

    Choice chChangeRange("Выбор диапазона", "Range selection", choices_change_range, 1, 2);

    static pchar choices_range[] =
    {
        "1Гц - 4Гц",   "1Hz - 4Hz",
        "4Гц - 40Гц",  "4Hz - 40Hz",
        "40Гц - 5кГц", "40Hz - 5kHz",
        nullptr
    };

    Choice chRange("Диапазон", "Range", choices_range, 1, 3);

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
        &chRange,
        &btnCOM,
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

        chChangeRange.SetShown(false);
        chRange.SetShown(false);
        btnCOM.SetShown(false);
    }


    static Page pageMenu(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMenu;
}
