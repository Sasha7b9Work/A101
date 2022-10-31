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


void Nextion::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text)
{
    SendCommandFormat("xstr %d,%d,%d,%d,%d,%d,%d,0,0,1,\"%s\"", x, y, width, height, font, color.ToRaw(), back_color.ToRaw(), text);
}




