// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/BufferADC.h"
#include <limits>


void BufferADC::ConvertToVoltage()
{
    for (int i = 0; i < SIZE; i++)
    {
        uint raw_value = raw[i];

        float value = (float)raw_value;

        if (raw_value > (1 << 17))
        {
            value -= (float)(1 << 18);
        }

        const float k = 5.0f / (1 << 17);

        volt[i] = (float)value * k;
    }
}


float BufferADC::MinReal() const
{
    float result = std::numeric_limits<float>::max();

    for (int i = 0; i < SIZE; i++)
    {
        if (volt[i] < result)
        {
            result = volt[i];
        }
    }

    return result;
}


float BufferADC::MaxReal() const
{
    float result = std::numeric_limits<float>::min();

    for (int i = 0; i < SIZE; i++)
    {
        if (volt[i] > result)
        {
            result = volt[i];
        }
    }

    return result;
}
