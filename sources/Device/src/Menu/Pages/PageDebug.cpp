// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"
#include "Display/Display.h"


namespace PageDebug
{
    static void FuncOnEnter()
    {

    }

    static void FuncDraw()
    {
    }

    static ButtonOld buttonZero("bt1", "debugZero", []()
        {
        });

    static ButtonOld buttonResetCal("bt2", "debugReset",
        []()
        {
            cal.Reset();
        });

    static ButtonCommon *buttons[] =
    {
        &buttonZero,
        &buttonResetCal,
        nullptr
    };

    static Page pageDebug(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageDebug;
}
