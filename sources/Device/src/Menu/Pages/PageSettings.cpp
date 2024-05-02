// 2024/05/02 08:38:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Settings/Settings.h"
#include <cstdio>


namespace PageSettings
{
    static Button btnBack("bt18", "2B0", []()
        {
            PageMain::self->SetAsCurrent();
        }
    );

    static Button *buttons[] =
    {
        &btnBack, nullptr
    };

    static void FuncOnEnter()
    {
        char buffer[32];

        std::sprintf(buffer, "%04u %04u", set.serial_number & 0xFFFF, (set.serial_number >> 16));

        Nextion::Text::SetLabel("NumVersion", buffer);
    }

    static void FuncDraw()
    {
//        Nextion::Text::SetLabel("NumVersion", "7");
    }

    static Page pageSettings(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageSettings;
}
