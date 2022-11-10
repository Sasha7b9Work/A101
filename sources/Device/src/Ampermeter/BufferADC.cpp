// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/Nextion.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include <limits>


int        BufferADC::pointer = 0;                                      // Указатель используется при чтении данных (массив raw)
ValueADC   BufferADC::raw[SIZE] __attribute__((section("CCM_DATA")));   // Данные, считанные с АЦП
SampleRate BufferADC::sampleRate;

ValueADC   BufferADC::min;
ValueADC   BufferADC::max;


void BufferADC::CalculateLimits()
{
    min = ValueADC::MAX;
    max = ValueADC::MIN;

    for (int i = 0; i < SIZE; i++)
    {
        ValueADC value = raw[i];

        if (value < min) { min = value; }
        if (value > max) { max = value; }
    }

    Indicator::SetDeltaADC((int)(max.Raw() - min.Raw()));
}


void BufferADC::LogUART() const
{
    for (int i = 0; i < Size(); i++)
    {
        LOG_WRITE("%d", raw[i].Raw());
    }
}
