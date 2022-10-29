// 2022/10/29 09:37:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Screen.h"


void Rect::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->FillRectangle(x, y, width, height, wxColor(Color::Current().ToRaw()));
}


void Rect::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    Line().Draw(x, y, x + width, y);
    Line().Draw(x + width - 1, y, x + width - 1, y + height);
    Line().Draw(x, y + height - 1, x + width, y + height - 1);
    Line().Draw(x, y, x, y + height);
}


void Line::Draw(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->DrawLine(x1, y1, x2, y2, wxColor(Color::Current().ToRaw()));
}


void Painter::DrawWave(uint8 *, int)
{

}


void Painter::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text)
{
    Rect(width, height).Fill(x, y, back_color);

    color.SetAsCurrent();

    Screen::self->DrawString(x, y, font, wxColor(Color::Current().ToRaw()), text);
}



void Painter::SetTextButton(pchar name_button, pchar text)
{
    Screen::self->SetTextButton(name_button, text);
}


void Painter::SetValButton(pchar name_button, int val)
{
    Screen::self->SetValButton(name_button, val);
}
