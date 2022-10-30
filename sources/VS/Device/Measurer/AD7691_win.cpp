// 2022/10/29 10:28:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/AD7691.h"
#include <cstdlib>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>


namespace AD7691
{
    static const double max = (double)(0x1FFFF);
}


void AD7691::Init()
{

}


int AD7691::ReadValue()
{
    const double step = 2.0 * M_PI / 1024.0;

    static double angle = 0.0;

    double value = std::cos(angle / (2.0f * M_PI)) * max;

    angle += step;

    int result = (int)value;

    if (result < 0)
    {
        result = -result;

        _SET_BIT(result, 17);
    }

    return result;
}
