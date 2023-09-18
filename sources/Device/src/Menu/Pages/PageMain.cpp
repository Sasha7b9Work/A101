// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageMain
{
    static Button button2mA("01P", [](Button *) {});

    static Button button20mA("02P", [](Button *) {});

    static Button button200mA("03P", [](Button *) {});

    static Button button2A("04P", [](Button *) {});

    static Button button20A("05P", [](Button *) {});

    static Button button50A("06P", [](Button *) {});


    static Page pageMain(&button2mA, &button20mA, &button200mA, &button2A, &button20A, &button50A);

    Page *self = &pageMain;
}

