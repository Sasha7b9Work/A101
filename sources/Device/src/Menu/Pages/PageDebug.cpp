// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Settings.h"


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

    static Button buttonMiddleOf3("button1", "Медиан", false,
        [](Button *button)
        {
            button->ToggleHighlight();

            set.middle_of_3 = button->IsHightlight();
        },
        [](Button *button)
        {
            if (button->IsHightlight())
            {
                if (!set.middle_of_3)
                {
                    button->SetHighlight(false);
                }
            }
            else
            {
                if (set.middle_of_3)
                {
                    button->SetHighlight(true);
                }
            }
        });

    static Button buttonSmooth("button2", "Сглаж", false, [](Button *) {});

    static Button button3("button3", "", false, [](Button *) {});

    static Button button4("button4", "", false, [](Button *) {});

    static Button button5("button5", "", false, [](Button *) {});


    static Page pageDebug(&buttonZero, &buttonMiddleOf3, &buttonSmooth, &button3, &button4, &button5);

    Page *self = &pageDebug;
}
