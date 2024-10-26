// 2022/10/19 10:00:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#ifdef WIN32
#include "../../Device/src/Nextion/Colors.h"
#else
#include "Nextion/Colors.h"
#endif
#include <cstdio>


Color Color::Black(0);
Color Color::White(1);
Color Color::Background(2);
Color Color::ButtonPress(3);
Color Color::MeasureDC(4);
Color Color::MeasureAC(5);
Color Color::Gray50(6);
Color Color::Gray75(7);
Color Color::Count(255);

Color Color::current(0);


static pchar colors[255] =
{
    "0",        // Black
    "65535",    // White
    "6964",     // Background
    "1024",     // ButtonPress
    "65504",    // MeasureDC
    "65504",    // MeasureAC
    "33808",    // Серый 50%
    "48631"     // Серый 75%
};


void Color::SetAsCurrent() const
{
    if (value != Count.value)
    {
        current = value;
    }
}


pchar Color::GetValue() const
{
    return colors[value];
}


uint16 Color::MakeColor(float r, float g, float b)
{
    int red = (int)(r * 0x3F);
    int green = (int)(g * 0x1F);
    int blue = (int)(b * 0x3F);

    return (uint16)((red << 11) | (green << 6) | (blue));
}


uint16 Color::_ToRaw() const
{   // Red   Green  Blue
    // 11111 111111 11111
    // b15 ........... b0

    return (uint16)std::atoi(colors[value]);
}
