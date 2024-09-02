// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Menu/MenuItems.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


namespace Display
{
    namespace LabelStar
    {
        static uint time_show = 0;      // Время зажигания звёздочки. Нужно, чтобы знать, когда потушить.
        static uint time_hide = 0;      // Время, когда была потушена звёздочка
        static bool is_shown = false;
    }
}


void Display::Init()
{
    Nextion::Page::Enable(0);

    Nextion::WaveInput::Disable(0);
    Nextion::WaveInput::Disable(1);
    Nextion::WaveFFT::Disable(0);

#ifdef DEBUG
    Nextion::DrawString({10, 350, 100, 40}, 0, Color::Black, Color::Background, "Debug");
#endif
}


void Display::Update()
{
    Page::Current()->Draw();
}


void Display::LabelStar::Show()
{
    if (is_shown || (TIME_MS - time_hide < 500))
    {
        return;
    }

    is_shown = true;

    time_show = TIME_MS;

    Nextion::SetVisible("t_star", true);
}


void Display::LabelStar::Update()
{
    if (is_shown && (TIME_MS - time_show > 500))
    {
        is_shown = false;

        time_hide = TIME_MS;

        Nextion::SetVisible("t_star", false);
    }
}
