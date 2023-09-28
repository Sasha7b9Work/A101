// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"


namespace PageDebug
{
    static void FuncOnEnable()
    {

    }

    static void FuncDraw()
    {

    }

    static Button buttonZero("bt1", "debugZero", []()
        {
        });

    static Button buttonResetCal("bt2", "debugReset",
        []()
        {
            cal.Reset();
        });

    static Button *buttons[] =
    {
        &buttonZero,
        &buttonResetCal,
        nullptr
    };

    static Page pageDebug(buttons, FuncOnEnable, FuncDraw);

    Page *self = &pageDebug;
}
