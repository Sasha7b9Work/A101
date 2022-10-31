// 2022/10/19 08:58:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/Nextion.h"
#include "Hardware/Timer.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include <cstdio>


namespace Painter
{
    static void WaitResponse(ResponseCode::E);
}


void Line::Draw(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    Nextion::SendCommandFormat("line %d,%d,%d,%d,%s", x1, y1, x2, y2, Color::CurrentValue());
}


void Line::DrawV(int x, int y1, int y2, const Color &color)
{
    Line().Draw(x, y1, x, y2, color);
}


void Painter::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text)
{
    Nextion::SendCommandFormat("xstr %d,%d,%d,%d,%d,%d,%d,0,0,1,\"%s\"", x, y, width, height, font, color.ToRaw(), back_color.ToRaw(), text);
}


void Painter::Button::SetText(pchar name_button, pchar _text)
{
    Nextion::SendCommandFormat("%s.txt=\"%s\"", name_button, _text);
}


void Painter::Button::Highligth(pchar name_button, bool val)
{
    uint color_val = val ? Color::ButtonPress.ToRaw() : Color::Background.ToRaw();

    Nextion::SendCommandFormat("%s.bco=%d", name_button, color_val);
}


void Painter::Button::Eanble(pchar name_button)
{
    Nextion::SendCommandFormat("vis %s,1", name_button);
}


void Painter::Button::Disable(pchar name_button)
{
    Nextion::SendCommandFormat("vis %s,0", name_button);
}


void Painter::WaveInput::Draw(uint8 *points, int num_points)
{
    /*
    * Маленький - в форму 9
    * Большой - в форму 7
    */


    int id = (DiagramFFT::IsEnabled() && DiagramInput::IsEnabled()) ? 9 : 7;

    Nextion::SendCommandFormatWithoutWaiting("addt %d,0,%d", id, num_points);

    WaitResponse(ResponseCode::TransparentDataReady);

    for (int i = 0; i < num_points; i++)
    {
        Nextion::SendByte(*points++);
    }

    WaitResponse(ResponseCode::TransparentDataFinished);
}


void Painter::WaveInput::Enable(int size)
{
    Nextion::SendCommandFormat("vis %s,1", size ? "waveBig" : "waveLeft");
}


void Painter::WaveInput::Disable(int size)
{
    Nextion::SendCommandFormat("vis %s,0", size ? "waveBig" : "waveLeft");
}


void Painter::WaveFFT::Enable(int size)
{
    Nextion::SendCommandFormat("vis %s,1", size ? "waveBig" : "waveRight");
}


void Painter::WaveFFT::Disable(int size)
{
    Nextion::SendCommandFormat("vis %s,0", size ? "waveBig" : "waveRight");
}


void Painter::WaitResponse(ResponseCode::E code)
{
    TimeMeterMS meter;

    while (Nextion::LastCode() != code)
    {
        if (meter.ElapsedTime() > 200)
        {
            LOG_WRITE("Not response");
            break;
        }
    }
}
