// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageMain
{
    static Button button2mA([](Button *) { });

    static Button button20mA([](Button *) { });

    static Button button200mA([](Button *) { });

    static Button button2A([](Button *) { });

    static Button button20A([](Button *) { });

    static Button button50A([](Button *) { });


    static Page pageMain(&button2mA, &button20mA, &button200mA, &button2A, &button20A, &button50A);

    Page *self = &pageMain;
}

