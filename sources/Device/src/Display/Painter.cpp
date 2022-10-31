// 2022/10/19 08:58:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Painter.h"
#include "Display/Nextion.h"
#include "Hardware/Timer.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include <cstdio>


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

