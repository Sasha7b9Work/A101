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


void Nextion::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->DrawLine(x1, y1, x2, y2, wxColor(Color::Current().ToRaw()));
}


void Nextion::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text)
{
    FillRect(x, y, width, height, back_color);

    color.SetAsCurrent();

    Screen::self->DrawString(x, y, font, wxColor(Color::Current().ToRaw()), text);
}

