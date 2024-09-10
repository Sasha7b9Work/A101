// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Display.h"


namespace PageMenu
{
    extern ButtonToggle btnIndication;
    extern Choice chChangeRange;
    extern Choice chRange;

    static void FuncDraw()
    {

    }


    static ButtonToggle btnFrequency("�������", "Frequency", Font::_1,
        {ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU},
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_f = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnImin("I���", "Imin", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(1), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Imin = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnImax("I����", "Imax", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(2), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Imax = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnIamp("I���", "Iamp", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(3), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Iampl = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnIpeak("I��", "Ipp", Font::_1,
        { ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(4), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *item, bool press)
    {
        if (press)
        {
            set.en_Ipp = item->ToButtonToggle()->IsPressed();
        }
    });

    static ButtonToggle btnSettings("���������", "Settings", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU},
        [](Item *, bool press)
    {
        if (press)
        {
            btnIndication.SetToggled(false);
        }

        chChangeRange.SetShown(press);
        chRange.SetShown(press);
    });

    static ButtonPress btnSystem("�������", "System", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(1), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool)
    {
    });

    ButtonToggle btnIndication("���������", "Indication", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(2), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool press)
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

    static ButtonPress btnCalibration("����������", "Calibration", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(3), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool)
    {
    });

    static ButtonPress btnGraphics("�������", "Graphs", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(4), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool)
    {
    });

    static ButtonPress btnBack("�����", "Back", Font::_1,
        { ButtonCommon::GetCoordX(2), ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
        }
    });

    static pchar choices_change_range[3][Lang::Count] =
    {
        { "������", "Hand" },
        { "��������������","Auto" },
        { nullptr, nullptr }
    };

    Choice chChangeRange("����� ���������", "Range selection", choices_change_range,
        ButtonCommon::GetCoordX(1), ButtonCommon::GetCoordY(2), [](Item *, bool)
    {
    });

    static pchar choices_range[4][Lang::Count] =
    {
        { "1�� - 4��",   "1Hz - 4Hz" },
        { "4�� - 40��",  "4Hz - 40Hz" },
        { "40�� - 5���", "40Hz - 5kHz" },
        { nullptr,       nullptr }
    };

    Choice chRange("��������", "Range", choices_range,
        Item::GetCoordX(1), Item::GetCoordY(3), [](Item *, bool)
    {

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
    }


    static Page pageMenu(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMenu;
}
