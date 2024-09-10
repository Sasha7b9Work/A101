// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Display.h"


namespace PageMenu
{
    static void FuncDraw()
    {

    }


    static ButtonToggle btnFrequency("Частота", "Frequency", Font::_1,
        {ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU},
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_f = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnImin("Iмин", "Imin", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(1), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Imin = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnImax("Iмакс", "Imax", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(2), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Imax = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnIamp("Iамп", "Iamp", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(3), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Iampl = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnIpeak("Iпп", "Ipp", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(4), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Ipp = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonPress btnSettings("Настройки", "Settings", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU},
        [](Item *, bool)
    {
    });

    static ButtonPress btnSystem("Система", "System", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(1), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool)
    {
    });

    static ButtonToggle btnIndication("Индикация", "Indication", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(2), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            bool is_shown = item->ToButtonToggle()->IsPressed();

            btnFrequency.SetShown(is_shown);
            btnImin.SetShown(is_shown);
            btnImax.SetShown(is_shown);
            btnIamp.SetShown(is_shown);
            btnIpeak.SetShown(is_shown);
        }
    });

    static ButtonPress btnCalibration("Калибровка", "Calibration", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(3), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool)
    {
    });

    static ButtonPress btnGraphics("Графики", "Graphs", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(4), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool)
    {
    });

    static ButtonPress btnBack("Назад", "Back", Font::_1,
        { Display::WIDTH - ButtonCommon::WIDTH_MENU - 10, ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
        }
    });

//    static ButtonPress btnChangeRange("Ручной", "Hand", Font::_1,
//        { })


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
        nullptr
    };

    static void FuncOnEnter()
    {
        set.en_f ? btnFrequency.Press() : btnFrequency.Release();
        set.en_Imax ? btnImax.Press() : btnImax.Release();
        set.en_Imin ? btnImin.Press() :btnImin.Release();
        set.en_Iampl ? btnIamp.Press() : btnIamp.Release();
        set.en_Ipp ? btnIpeak.Press() : btnIpeak.Release();

        btnIndication.Release();

        btnFrequency.SetShown(false);
        btnImax.SetShown(false);
        btnImin.SetShown(false);
        btnIamp.SetShown(false);
        btnIpeak.SetShown(false);
    }


    static Page pageMenu(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMenu;
}
