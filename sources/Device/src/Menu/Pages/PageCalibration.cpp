// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageCalibration
{
    static Button button0(false, [](Button *) {});

    static Button button1(false, [](Button *) {});

    static Button button2(false, [](Button *) {});

    static Button button3(false, [](Button *) {});

    static Button button4(false, [](Button *) {});

    static Button button5(false, [](Button *) {});

    static Page pageCalibration(&button0, &button1, &button2, &button3, &button4, &button5);

    Page *self = &pageCalibration;
}
