// 2022/10/29 09:37:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Screen.h"


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


void Painter::WaveFFT::Enable(int size)
{
    Screen::WaveFFT::Enable(size);
}


void Painter::WaveFFT::Disable(int size)
{
    Screen::WaveFFT::Disable(size);
}
