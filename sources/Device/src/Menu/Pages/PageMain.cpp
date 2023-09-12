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

    static Button button2mA(false, [](Button *btn) { });

    static Button button20mA(false, [](Button *btn) { });

    static Button button200mA(false, [](Button *btn) { });

    static Button button2A(true, [](Button *btn) { });

    static Button button20A(false, [](Button *btn) { });

    static Button button50A(false, [](Button *btn) { });


    static Page pageMain(&button2mA, &button20mA, &button200mA, &button2A, &button20A, &button50A);

    Page *self = &pageMain;
}

