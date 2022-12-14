// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Display/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Display/Indicator.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


void Display::Init()
{
    Nextion::Page::Enable(0);

    Nextion::WaveInput::Disable(0);
    Nextion::WaveInput::Disable(1);
    Nextion::WaveFFT::Disable(0);

    Indicator::Init();

    Indicator::AutoSize();

#ifdef DEBUG
    Nextion::DrawString(600, 5, 100, 40, 2, Color::Black, Color::Background, "Debug");
#endif
}


void Display::Update()
{
    DiagramInput::Draw();

    Indicator::Update();
}
