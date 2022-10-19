// 2022/10/19 08:58:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include <cstdio>


void Primitives::Rectangle::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();


}


void Primitives::Rectangle::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    char buffer[128];

    std::sprintf(buffer, "fill %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());

    Display::Interface::SendCommand(buffer);
}
