// 2022/10/31 19:37:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Nextion.h"
#include "Display/Painter.h"
#include "Screen.h"


void Nextion::DrawRect(int x, int y, int width, int height, const Color &color)
{
    color.SetAsCurrent();

    DrawLineH(y, x, x + width);
    DrawLineV(x + width - 1, y, y + height);
    DrawLineH(y + height - 1, x, x + width);
    DrawLineV(x, y, y + height);
}


void Nextion::FillRect(int x, int y, int width, int height, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->FillRectangle(x, y, width, height, wxColor(Color::Current().ToRaw()));
}
