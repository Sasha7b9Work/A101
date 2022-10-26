// 2022/10/19 08:58:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include <cstdio>


void Primitives::Rectangle::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    DInterface::SendCommandFormat("draw %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Primitives::Rectangle::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    DInterface::SendCommandFormat("fill %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Primitives::Line::Draw(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    DInterface::SendCommandFormat("line %d,%d,%d,%d,%s", x1, y1, x2, y2, Color::CurrentValue());
}


void Primitives::Line::DrawV(int x, int y1, int y2, const Color &color)
{
    Line().Draw(x, y1, x, y2, color);
}


void Primitives::Line::DrawH(int y, int x1, int x2, const Color &color)
{
    Line().Draw(x1, y, x2, y, color);
}
