// 2022/10/29 21:41:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/PageCalibration.h"


namespace PageCalibration
{
    static Button buttonZero("button0", "Zero", false, [](Button *button)
        {
            button->ToggleHighlight();

            if (button->IsHightlight())
            {

            }
            else
            {

            }
        });


    static Button button0A("button1", "0 A", false, [](Button *) {});

    static Button button20mA("button2", "20 mA", false, [](Button *) {});

    static Button button3("button3", "", false, [](Button *) {});

    static Button button4("button4", "", false, [](Button *) {});

    static Button button5("button5", "", false, [](Button *) {});

    static Page pageCalibration(&buttonZero, &button0A, &button20mA, &button3, &button4, &button5);

    Page *self = &pageCalibration;
}
