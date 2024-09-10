// 2024/09/10 16:55:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Nextion/Display.h"


namespace PageInformation
{
    static void FuncDraw()
    {
        for (int i = 0; i < 2; i++)
        {
            Nextion::DrawRect({ i, i, Display::WIDTH - 1 - i * 2, Display::HEIGHT - 1 - i * 2 }, Color::White);
        }
    }

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMenu::self->SetAsCurrent();
        }
    });

    static Item *items[] =
    {
        &btnBack,
        nullptr
    };

    static Page pageInformation(items, nullptr, FuncDraw);

    Page *self = &pageInformation;
}
