// 2022/10/31 19:37:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Nextion.h"
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

    if (Screen::self)
    {
        Screen::self->FillRectangle(x, y, width, height, wxColor(Color::Current().ToRaw()));
    }
}


void Nextion::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->DrawLine(x1, y1, x2, y2, wxColor(Color::Current().ToRaw()));
}


void Nextion::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text, int)
{
    FillRect(x, y, width, height, back_color);

    color.SetAsCurrent();

    if (Screen::self)
    {
        Screen::self->DrawString(x, y, font, wxColor(Color::Current().ToRaw()), text);
    }
}


void Nextion::WaveInput::Draw(uint8 *points, int num_points)
{
    Screen::WaveInput::Draw(points, num_points);
}


void Nextion::WaveInput::Enable(int size)
{
    Screen::WaveInput::Enable(size);
}


void Nextion::WaveInput::Disable(int size)
{
    Screen::WaveInput::Disable(size);
}


void Nextion::WaveFFT::Enable(int size)
{
    Screen::WaveFFT::Enable(size);
}


void Nextion::WaveFFT::Disable(int size)
{
    Screen::WaveFFT::Disable(size);
}


void Nextion::Button::SetText(pchar _name_button, pchar _text)
{
    Screen::self->Button::SetText(_name_button, _text);
}


void Nextion::Button::Highligth(pchar _name_button, bool _highlight)
{
    Screen::self->Button::Highlight(_name_button, _highlight);
}


void Nextion::Button::Enable(pchar _name_button)
{
    Screen::Button::Enable(_name_button);
}


void Nextion::Button::Disable(pchar _name_button)
{
    Screen::Button::Disable(_name_button);
}


void Nextion::Page::Enable(int)
{

}
