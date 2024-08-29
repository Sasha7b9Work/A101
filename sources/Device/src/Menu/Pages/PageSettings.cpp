// 2024/05/02 08:38:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Settings/Settings.h"
#include <cstdio>


namespace PageSettings
{
    static ButtonOld btnBack("bt18", "2B0", []()
        {
            PageMain::self->SetAsCurrent();
        }
    );

    static ButtonOld btnLanguageRU("r0", "RBRU", [] {});

    static ButtonOld btnLanguageEN("r1", "RBEN", [] {});

    static Item *items[] =
    {
        &btnBack, &btnLanguageRU, &btnLanguageEN, nullptr
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

    static Page pageSettings(items, FuncOnEnter, FuncDraw);

    Page *self = &pageSettings;
}
