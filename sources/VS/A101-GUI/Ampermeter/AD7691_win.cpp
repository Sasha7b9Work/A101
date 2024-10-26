// 2023/11/09 12:21:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"
#include <cstdlib>


namespace AD7691
{
    static int counter = 0;
}


SampleRate SampleRate::current(10);


ValueADC::ValueADC(int reading)
{
    value = reading - cal.zero[Range::Current()].GetFull();
}


void AD7691::Init()
{

}


ValueADC AD7691::ReadValue()
{
    return ValueADC(ReadValueRAW());
}


void AD7691::ResetValue()
{

}


static float GetSample(float freq, int num_sample)
{
    float T = 1.0f / freq;

    float samples_in_T = T / 10e-6f;

    float radians_in_T = 2.0f * M_PI;

    float radians_in_sample = radians_in_T / samples_in_T;

    return std::sinf(radians_in_sample * num_sample);
}


int AD7691::ReadValueRAW()
{
    float amplitude = 0.5f;

    float value = amplitude * GetSample(4997.5f, counter);

    value += (float)std::rand() / (float)RAND_MAX / 20.0f;

    value *= (1 << 16);

    if (value < 0)
    {
        value += (1 << 18);
    }

    counter++;

    return (int)(value / 10);
}


int AD7691::GetAverageValue()
{
    return 0;
}
