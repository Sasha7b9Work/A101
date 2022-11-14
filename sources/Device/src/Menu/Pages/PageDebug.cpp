// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageDebug
{
    static Button buttonZero("button0", "Zero", false, [](Button *button)
        {
            button->ToggleHighlight();

            if (button->IsHightlight())
            {
                InputRelays::EnableZero();
            }
            else
            {
                InputRelays::DisableZero();
            }
        });

    static Button button1("button1", "", false, [](Button *) {});

    static Button button2("button2", "", false, [](Button *) {});

    static Button button3("button3", "", false, [](Button *) {});

    static Button button4("button4", "", false, [](Button *) {});

    static Button button5("button5", "", false, [](Button *) {});


    static Page pageDebug(&buttonZero, &button1, &button2, &button3, &button4, &button5);

    Page *self = &pageDebug;
}
