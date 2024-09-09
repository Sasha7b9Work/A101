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
        { 10, 10, ButtonCommon::WIDTH_MENU, ButtonCommon::HEIGHT_MENU },
        [](Item *)
    {
    });


    static Item *items[] =
    {
        &btnSettings,
        nullptr
    };

    static Page pageMenu(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMenu;
}
