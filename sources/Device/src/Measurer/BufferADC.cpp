// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/BufferADC.h"
#include "Display/DInterface.h"
#include "Display/Indicator.h"
#include <limits>


void BufferADC::ConvertToVoltage()
{
    min = std::numeric_limits<float>::max();
    max = std::numeric_limits<float>::min();

    min_raw = 1 << 20;
    max_raw = - min_raw;

    for (int i = 0; i < SIZE; i++)
    {
        int raw_value = raw[i];

        if (raw_value < min_raw)
        {
            min_raw = raw_value;
        }
        if (raw_value > max_raw)
        {
            max_raw = raw_value;
        }

        float value = (float)raw_value;

        const float k = 5.0 / (1 << 17);

        volt[i] = value * k;

        if (volt[i] < min)
        {
            min = volt[i];
        }

        if (volt[i] > max)
        {
            max = volt[i];
        }
    }

    Indicator::SetDeltaADC((int)(max_raw - min_raw));
}

