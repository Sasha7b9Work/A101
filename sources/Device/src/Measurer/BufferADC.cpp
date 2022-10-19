// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/BufferADC.h"


void BufferADC::ConvertToVoltage()
{
    for (int i = 0; i < SIZE; i++)
    {
        uint value = raw[i];

        value &= 0x1FFFF;

        static const float k = 5.0f / (float)0x3FFFF;

        volt[i] = (float)value * k;

        if (value != raw[i])
        {
            volt[i] *= -1.0f;
        }
    }
}
