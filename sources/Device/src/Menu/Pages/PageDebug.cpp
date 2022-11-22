// 2022/10/29 21:42:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Settings/Settings.h"


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

    static Button buttonMiddleOf3("button1", "Middle", false,
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

    static Button buttonSmooth("button2", "Smooth", false,
        [](Button *button)
        {
            button->ToggleHighlight();

            set.smooth = button->IsHightlight();
        },
        [](Button *button)
        {
            if (button->IsHightlight())
            {
                if (!set.smooth)
                {
                    button->SetHighlight(false);
                }
            }
            else
            {
                if (set.smooth)
                {
                    button->SetHighlight(true);
                }
            }
        });

    static Button buttonFIR("button3", "LPF", false,
        [](Button *button)
        {
            button->ToggleHighlight();

            set.firLPF = button->IsHightlight();
        },
        [](Button *button)
        {
            button->SetHighlight(set.firLPF);
        });

    static Button button4("button4", "", false, [](Button *) {});

    static Button button5("button5", "", false, [](Button *) {});


    static Page pageDebug(&buttonZero, &buttonMiddleOf3, &buttonSmooth, &buttonFIR, &button4, &button5);

    Page *self = &pageDebug;
}
