// 2022/10/19 10:00:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Colors.h"


Color Color::None(255);
Color Color::Black(0);
Color Color::White(1);


Color Color::current(0);


void Color::SetAsCurrent() const
{
    if (value != None.value)
    {
        current = value;
    }
};

