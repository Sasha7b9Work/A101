// 2024/09/09 13:58:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"


namespace PageMenu
{
    static void FuncOnEnter()
    {

    }


    static void FuncDraw()
    {

    }


    static ButtonPress btnSettings("Настройки", "Settings", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(0), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU},
        [](Item *)
    {
    });

    static ButtonPress btnSystem("Система", "System", Font::_1,
        { ButtonCommon::GetCoordX(0), ButtonCommon::GetCoordY(1), ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *)
    {
    });


    static Item *items[] =
    {
        &btnSettings,
        &btnSystem,
        nullptr
    };

    static Page pageMenu(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMenu;
}
