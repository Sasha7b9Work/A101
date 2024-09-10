// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"
#include "Nextion/Display.h"


namespace PageDebug
{
    static void FuncOnEnter()
    {

    }

    static void FuncDraw()
    {
    }

    static ButtonPress btnBack("Назад", "Back", Font::_1, { 19, 17, 150, 73 }, [](Item *, bool)
        {
        });

    static Item *items[] =
    {
        &btnBack,
        nullptr
    };

    static Page pageDebug(items, FuncOnEnter, FuncDraw);

    Page *self = &pageDebug;
}
