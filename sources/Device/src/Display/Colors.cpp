// 2022/10/19 10:00:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include <cstdio>


Color Color::Black(0);
Color Color::White(1);
Color Color::Background(2);
Color Color::Count(255);

Color Color::current(0);


#ifdef WIN32
    static uint colors[255] =
    {
        0x00000000,
        0x00FFFFFF,
        0x00F23352
    };
#else
    static uint16 colors[255] =
    {
        Color::MakeColor(0.0f, 0.0f, 0.0f),
        Color::MakeColor(1.0f, 1.0f, 1.0f),
        15319
    };
#endif


void Color::SetAsCurrent() const
{
    if (value != Count.value)
    {
        current = value;
    }
}


pchar Color::CurrentValue()
{
    static char buffer[32];

    std::sprintf(buffer, "%d", colors[Current().value]);

    return buffer;
}


uint16 Color::MakeColor(float r, float g, float b)
{
    int red = (int)(r * 0x3F);
    int green = (int)(g * 0x1F);
    int blue = (int)(b * 0x3F);

    return (uint16)((red << 11) | (green << 6) | (blue));
}


#ifdef WIN32
    uint Color::ToRaw() const
#else
    uint16 Color::ToRaw() const
#endif
{
    return colors[value];
}
