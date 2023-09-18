// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageCalibration
{
    static Button button0("", [](Button *) {});

    static Button button1("", [](Button *) {});

    static Button button2("", [](Button *) {});

    static Button button3("", [](Button *) {});

    static Button button4("", [](Button *) {});

    static Button button5("", [](Button *) {});

    static Button *buttons[] =
    {
        &button0,
        &button1,
        &button2,
        &button3,
        &button4,
        &button5,
        nullptr
    };

    static Page pageCalibration(buttons);

    Page *self = &pageCalibration;
}
