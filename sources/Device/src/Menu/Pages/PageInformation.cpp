// 2024/09/10 16:55:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Nextion/Display.h"
#include "Hardware/Timer.h"


namespace PageInformation
{
    static void FuncDraw()
    {
        for (int i = 0; i < 2; i++)
        {
            Nextion::DrawRect({ i, i, Display::WIDTH - 1 - i * 2, Display::HEIGHT - 1 - i * 2 }, Color::White);
        }

        Nextion::DrawString({ 50, 100, 700, 100 }, Font::_3, Color::White, Color::Background, "О приборе:", true, true);

        Timer::Delay(100);

        Nextion::DrawString({ 50, 200, 700, 200 }, Font::_1, Color::White, Color::Background, "Производитель ОАО МНИПИ", true, true);
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
