// 2022/10/31 19:37:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Nextion.h"


void Nextion::DrawRect(int x, int y, int width, int height, const Color &color)
{
    color.SetAsCurrent();

    SendCommandFormat("draw %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Nextion::FillRect(int x, int y, int width, int height, const Color & color)
{
    color.SetAsCurrent();

    SendCommandFormat("fill %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Nextion::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    SendCommandFormat("line %d,%d,%d,%d,%s", x1, y1, x2, y2, Color::CurrentValue());
}


