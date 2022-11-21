// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"


namespace PageMain
{
    static int GetIndex(const Button *button)
    {
        for (int i = 0; i < 6; i++)
        {
            if (PageMain::self->GetButton(i) == button)
            {
                return i;
            }
        }

        LOG_WRITE("Invalid index page");

        return 0;
    }

    static void Function(const Button *button)
    {
        int index = GetIndex(button);

        Range::Set(index);

        for (int i = 5; i >= 0; i--)
        {
            PageMain::self->GetButton(i)->SetHighlight(index == i);
        }
    }

    static Button button2mA("button0", "2 mA", false, [](Button *btn) {Function(btn); });

    static Button button20mA("button1", "20 mA", false, [](Button *btn) {Function(btn); });

    static Button button200mA("button2", "200 mA", false, [](Button *btn) {Function(btn); });

    static Button button2A("button3", "2 A", true, [](Button *btn) {Function(btn); });

    static Button button20A("button4", "20 A", false, [](Button *btn) {Function(btn); });

    static Button button50A("button5", "50 A", false, [](Button *btn) {Function(btn); });


    static Page pageMain(&button2mA, &button20mA, &button200mA, &button2A, &button20A, &button50A);

    Page *self = &pageMain;
}

