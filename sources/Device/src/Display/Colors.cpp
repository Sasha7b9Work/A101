// 2022/10/19 10:00:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"
#include <cstdio>


Color Color::Black(0);
Color Color::White(1);
Color Color::Background(2);
Color Color::ButtonPress(3);
Color Color::MeasureDC(4);
Color Color::MeasureAC(5);
Color Color::Count(255);

Color Color::current(0);


static uint16 colors[255] =
{
    Color::MakeColor(0.0f, 0.0f, 0.0f),     // Black
    Color::MakeColor(1.0f, 1.0f, 1.0f),     // White
    15319,                                  // Background
    6700,                                   // ButtonPress
    Color::MakeColor(0.5f,  1.0f, 0.1f),    // MeasureDC
    Color::MakeColor(0.25f, 0.75f, 0.05f)   // MeasureAC
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


#ifdef WIN32

uint Color::ToRaw() const
{
    uint16 val16 = colors[value];

    float red = (float)((val16 >> 11) & 0x1F) / 31.0f;
    float green = (float)((val16 >> 5) & 0x3F) / 63.0f;
    float blue = (float)((val16 >> 0) & 0x1F) / 31.0f;

    uint u_r = (uint)(red * 255.0f);
    uint u_g = (uint)(green * 255.0f);
    uint u_b = (uint)(blue * 255.0f);

    uint result = (u_b << 16) | (u_g << 8) | u_r;

    return result;  
}

#else

uint16 Color::ToRaw() const
{
    return colors[value];
}

#endif
