// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/Nextion.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include <limits>


void BufferADC::CalculateLimits()
{
    min = std::numeric_limits<float>::max();
    max = std::numeric_limits<float>::min();

    min_raw = 1 << 20;
    max_raw = - min_raw;

    for (int i = 0; i < SIZE; i++)
    {
        int raw_value = raw[i].Raw();

        if (raw_value < min_raw)
        {
            min_raw = raw_value;
        }
        if (raw_value > max_raw)
        {
            max_raw = raw_value;
        }

        if (raw[i].Real() < min)
        {
            min = raw[i].Real();
        }

        if (raw[i].Real() > max)
        {
            max = raw[i].Real();
        }
    }

    Indicator::SetDeltaADC((int)(max_raw - min_raw));

//    LOG_WRITE("min %d, max %d, delta %d", min_raw, max_raw, (int)(max_raw - min_raw));
}


void BufferADC::LogUART() const
{
    for (int i = 0; i < Size(); i++)
    {
        LOG_WRITE("%d", raw[i].Raw());
    }
}
