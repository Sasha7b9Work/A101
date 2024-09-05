// 2024/05/02 08:38:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Settings/Settings.h"
#include <cstdio>


namespace PageSettings
{
    static ButtonPress btnBack("Назад", "Back", Font::_1, { 4, 5, 150, 73 }, [](Item *)
        {
            PageMain::self->SetAsCurrent();
        }
    );

    static ButtonToggle btnLanguageRU("Русский", "Russian", Font::_1, { 300, 300, 100, 50 }, [](Item *)
    {
    });

    static ButtonToggle btnLanguageEN("Английский", "English", Font::_1, { 300, 400, 100, 50 }, [](Item *)
    {
    });

    static Item *items[] =
    {
        &btnBack,
        &btnLanguageRU,
        &btnLanguageEN,
        nullptr
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
