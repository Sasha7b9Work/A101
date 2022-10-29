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

    Line().DrawH(y, x, x + width);
    Line().DrawV(x + width - 1, y, y + height);
    Line().DrawH(y + height - 1, x, x + width);
    Line().DrawV(x, y, y + height);
}


void Line::Draw(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->DrawLine(x1, y1, x2, y2, wxColor(Color::Current().ToRaw()));
}


void Line::DrawV(int x, int y1, int y2, const Color &color)
{
    color.SetAsCurrent();

    Draw(x, y1, x, y2);
}


void Line::DrawH(int y, int x1, int x2, const Color &color)
{
    color.SetAsCurrent();

    Draw(x1, y, x2, y);
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


void Painter::Button::SetText(pchar name_button, pchar text)
{
    Screen::self->Button::SetText(name_button, text);
}


void Painter::Button::Highligth(pchar name_button, bool highlight)
{
    Screen::self->Button::Highlight(name_button, highlight);
}


void Painter::Button::Eanble(pchar name_button)
{
    Screen::Button::Enable(name_button);
}


void Painter::Button::Disable(pchar name_button)
{
    Screen::Button::Disable(name_button);
}
