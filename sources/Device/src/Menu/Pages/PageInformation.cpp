// 2024/09/10 16:55:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Nextion/Nextion.h"
#include "Nextion/Display.h"
#include "Hardware/Timer.h"
#include <cstdio>


namespace PageInformation
{
    static void DrawString(int x, int y, pchar text)
    {
        Nextion::DrawString({ x, y, 300, 60 }, Font::_1, Color::White, Color::Background, text, false, true);
    }

    static void FuncDraw()
    {
        for (int i = 0; i < 2; i++)
        {
            Nextion::DrawRect({ i, i, Display::WIDTH - 1 - i * 2, Display::HEIGHT - 1 - i * 2 }, Color::White);
        }

        Nextion::DrawString({ 50, 100, 700, 100 }, Font::_3, Color::White, Color::Background, "О приборе:", true, true);

        static const pchar strings[3][2] =
        {
            { "Производитель :",  "ОАО МНИПИ" },
            { "Версия ПО :",      "1.0.1" },
            { "Номер прибора : ", "" }
        };

        const int y = 220;
        const int x1 = 80;
        const int x2 = 400;

        for (int i = 0; i < 3; i++)
        {
            DrawString(x1, y + i * 70, strings[i][0]);
            DrawString(x2, y + i * 70, strings[i][1]);
        }

        char buffer[32] = { '\0' };

        std::sprintf(buffer, "%u", set.serial_number);

        DrawString(x2, y + 2 * 70, buffer);
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
