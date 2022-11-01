// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/Nextion.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include <limits>


void BufferADC::CalculateLimits()
{
    min = ValueADC::MAX;
    max = ValueADC::MIN;

    for (int i = 0; i < SIZE; i++)
    {
        ValueADC value = raw[i];

        if (value < ValueADC::MIN) { min = value; }
        if (value > ValueADC::MAX) { max = value; }
    }

    Indicator::SetDeltaADC((int)(max.Raw() - min.Raw()));

//    LOG_WRITE("min %d, max %d, delta %d", min_raw, max_raw, (int)(max_raw - min_raw));
}


void BufferADC::LogUART() const
{
    for (int i = 0; i < Size(); i++)
    {
        LOG_WRITE("%d", raw[i].Raw());
    }
}
