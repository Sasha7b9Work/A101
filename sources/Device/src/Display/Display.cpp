// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Display/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Menu/MenuItems.h"
#include "Hardware/Timer.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


void Display::Init()
{
    Nextion::Page::Enable(0);

    Nextion::WaveInput::Disable(0);
    Nextion::WaveInput::Disable(1);
    Nextion::WaveFFT::Disable(0);

#ifdef DEBUG
    Nextion::DrawString(10, 350, 100, 40, 2, Color::Black, Color::Background, "Debug");
#endif
}


void Display::Update()
{
    Page::Current()->Draw();
}


void Display::DrawLabelStar()
{
    uint secs = TIME_MS / 1000;

    static bool is_enabled = false;

    bool enabled = (secs % 2) != 0;

    if (enabled != is_enabled)
    {
        Nextion::SetVisible("t_star", enabled);

        is_enabled = enabled;
    }
}
