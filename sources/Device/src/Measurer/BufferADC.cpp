// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/BufferADC.h"


void BufferADC::ConvertToVoltage()
{
    for (int i = 0; i < SIZE; i++)
    {
        uint raw_value = raw[i];

        float value = (float)raw_value;

        if (raw_value > (1 << 17))
        {
            value = 
        }

        float value = (raw_value > (1 << 17)) ? (float)(raw_value - (1 << 18)) : (float)raw_value;

        const float k = 5.0f / (1 << 17);

        volt[i] = (float)value * k;
    }
}
