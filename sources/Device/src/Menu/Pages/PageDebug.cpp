// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"


namespace PageDebug
{
    static Button buttonZero([](Button *)
        {
        });

    static Button button4([](Button *) {});

    static Button buttonResetCal(
        [](Button *)
        {
            cal.Reset();
        });

    static Page pageDebug(&buttonZero, &button4, &button4, &button4, &button4, &buttonResetCal);

    Page *self = &pageDebug;
}
