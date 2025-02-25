// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Menu/MenuItems.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Pages.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


namespace Display
{
}


void Display::Init()
{
    Nextion::Page::Enable(0);

    Nextion::WaveInput::Disable(0);
    Nextion::WaveInput::Disable(1);
    Nextion::WaveFFT::Disable(0);

    SetBrightness();

    Nextion::SendCommandFormat("%s", "pic 0,0,0");

    Nextion::DrawString({ 645, 423, 145, 45 }, Font::_0_GB34b, Color::Background, Color::White, "Ver.1.0.1.");

#ifdef DEBUG
    Nextion::DrawString({10, 350, 100, 40}, 0, Color::Black, Color::Background, "Debug");
#endif
}


void Display::SetBrightness()
{
    float max_delta = 100.0f - 10.0f;

    float delta = 100.0f - 10.0f - 20.0f;

    float k = delta / max_delta;

    float bright = (((float)set.brightness - 10.0f) * k) + 10.0f;

    Nextion::SendCommandFormat("dims=%d", (int)bright);
}


void Display::Update()
{
    Page::Current()->Draw();
}


void Display::Clear()
{
    Nextion::FillRect({ 0, 0, Display::WIDTH, Display::HEIGHT }, Color::Background);
}
