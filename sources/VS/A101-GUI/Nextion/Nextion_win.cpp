// 2022/10/31 19:37:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Nextion.h"
#include "GUI/Screen.h"


void Nextion::DrawSpaceForStar()
{

}


void Nextion::DrawStarForStar()
{

}


void Nextion::DrawRect(const Rect &rect, const Color &color)
{
    color.SetAsCurrent();

    int x = rect.x;
    int y = rect.y;
    int width = rect.width;
    int height = rect.height;

    DrawLineH(y, x, x + width);
    DrawLineV(x + width - 1, y, y + height);
    DrawLineH(y + height - 1, x, x + width);
    DrawLineV(x, y, y + height);
}


void Nextion::FillRect(const Rect &rect, const Color &color)
{
    color.SetAsCurrent();

    if (Screen::self)
    {
        int x = rect.x;
        int y = rect.y;
        int width = rect.width;
        int height = rect.height;

        Screen::self->FillRectangle(x, y, width, height, Color::Current());
    }
}


void Nextion::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    Screen::self->DrawLine(x1, y1, x2, y2, Color::Current());
}


void Nextion::DrawString(const Rect &rect, int font, const Color &color, const Color &back_color, pchar text, bool /*h_align*/, bool /*v_align*/)
{
    FillRect(rect, back_color);

    color.SetAsCurrent();

    if (Screen::self)
    {
        Screen::self->DrawString(rect.x, rect.y, font, Color::Current(), text);
    }
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


void Nextion::DrawLineWhite(int x1, int y1, int x2, int y2)
{
    DrawLine(x1, y1, x2, y2, Color::White);
}


void Nextion::Page::Enable(int)
{

}


void Nextion::SendCommandFormat(const char *, ...)
{

}

