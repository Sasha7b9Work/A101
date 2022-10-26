// 2022/10/19 10:00:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include <cstdio>


Color Color::Black(0);
Color Color::White(1);
Color Color::Count(255);

Color Color::current(0);


static uint16 colors[255] =
{
    Color::MakeColor(0.0f, 0.0f, 0.0f),
    Color::MakeColor(1.0f, 1.0f, 1.0f)
};


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
