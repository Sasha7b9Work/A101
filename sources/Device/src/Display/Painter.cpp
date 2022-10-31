// 2022/10/19 08:58:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include "Hardware/Timer.h"
#include <cstdio>


namespace Painter
{
    static void WaitResponse(ResponseCode::E);
}


void Rect::Draw(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    DInterface::SendCommandFormat("draw %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Rect::Fill(int x, int y, const Color &color)
{
    color.SetAsCurrent();

    DInterface::SendCommandFormat("fill %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Line::Draw(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    DInterface::SendCommandFormat("line %d,%d,%d,%d,%s", x1, y1, x2, y2, Color::CurrentValue());
}


void Line::DrawV(int x, int y1, int y2, const Color &color)
{
    Line().Draw(x, y1, x, y2, color);
}


void Line::DrawH(int y, int x1, int x2, const Color &color)
{
    Line().Draw(x1, y, x2, y, color);
}


void Painter::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text)
{
    DInterface::SendCommandFormat("xstr %d,%d,%d,%d,%d,%d,%d,0,0,1,\"%s\"", x, y, width, height, font, color.ToRaw(), back_color.ToRaw(), text);
}


void Painter::Button::SetText(pchar name_button, pchar _text)
{
    DInterface::SendCommandFormat("%s.txt=\"%s\"", name_button, _text);
}


void Painter::Button::Highligth(pchar name_button, bool val)
{
    uint color_val = val ? Color::ButtonPress.ToRaw() : Color::Background.ToRaw();

    DInterface::SendCommandFormat("%s.bco=%d", name_button, color_val);
}


void Painter::Button::Eanble(pchar name_button)
{
    DInterface::SendCommandFormat("vis %s,1", name_button);
}


void Painter::Button::Disable(pchar name_button)
{
    DInterface::SendCommandFormat("vis %s,0", name_button);
}


void Painter::WaveInput::Draw(uint8 *points, int num_points)
{
    DInterface::SendCommandFormat("addt 16,0,%d", num_points);

    WaitResponse(ResponseCode::TransparentDataReady);

    for (int i = 0; i < num_points; i++)
    {
        DInterface::SendByte(*points++);
    }

    WaitResponse(ResponseCode::TransparentDataFinished);
}


void Painter::WaveInput::Enable(int size)
{
    DInterface::SendCommandFormat("vis %s,1", size ? "waveBig" : "waveLeft");
}


void Painter::WaveInput::Disable(int size)
{
    DInterface::SendCommandFormat("vis %s,0", size ? "waveBig" : "waveLeft");
}


void Painter::WaveFFT::Enable(int size)
{
    DInterface::SendCommandFormat("vis %s,1", size ? "waveBig" : "waveRight");
}


void Painter::WaveFFT::Disable(int size)
{
    DInterface::SendCommandFormat("vis %s,0", size ? "waveBig" : "waveRight");
}


void Painter::WaitResponse(ResponseCode::E code)
{
    TimeMeterMS meter;

    while (DInterface::LastCode() != code)
    {
        DInterface::Update();

        if (meter.ElapsedTime() > 400)
        {
            break;
        }
    }
}
