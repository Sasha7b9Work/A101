// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/Nextion.h"
#include "Display/Indicator.h"
#include "Hardware/HAL/HAL.h"
#include <limits>


int        BufferADC::pointer = 0;                                      // ”казатель используетс€ при чтении данных (массив raw)
ValueADC   BufferADC::raw[SIZE] __attribute__((section("CCM_DATA")));   // ƒанные, считанные с ј÷ѕ
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

    Indicator::SetDeltaADC(max.Real() - min.Real());
}


void BufferADC::LogUART()
{
    for (int i = 0; i < Size(); i++)
    {
        LOG_WRITE("%f", (double)raw[i].Real());
    }
}


static float middle_of_3(float a, float b, float c)
{
    if ((a <= b) && (a <= c))
    {
        return (b <= c) ? b : c;
    }
    else if ((b <= a) && (b <= c))
    {
        return (a <= c) ? a : c;
    }
    return (a <= b) ? a : b;
}


void BufferADC::MiddleOf3()
{
    float raw_i_1 = raw[0].Real();   // Ёлемент raw[i - 1]
    float raw_i_0 = 0;              // Ёлемент raw[i]

    for (int i = 1; i < SIZE - 1; i++)
    {
        raw_i_0 = raw[i].Real();

        raw[i] = ValueADC::FromReal(middle_of_3(raw_i_1, raw[i].Real(), raw[i + 1].Real()));

        raw_i_1 = raw_i_0;
    }
}
