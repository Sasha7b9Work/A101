// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageDebug.h"


namespace PageDebug
{
    static Button button0("button0", "", false, [](Button *) {});

    static Button button1("button1", "", false, [](Button *) {});

    static Button button2("button2", "", false, [](Button *) {});

    static Button button3("button3", "", false, [](Button *) {});

    static Button button4("button4", "", false, [](Button *) {});

    static Button button5("button5", "", false, [](Button *) {});


    static Page pageDebug(&button0, &button1, &button2, &button3, &button4, &button5);

    Page *self = &pageDebug;
}
