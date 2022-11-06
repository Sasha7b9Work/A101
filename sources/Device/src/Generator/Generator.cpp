// 2022/11/01 11:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Ampermeter/AD7691.h"
#include "Utils/Math.h"
#include <cmath>


namespace Generator
{
    static bool enabled = false;
    static float frequency = 1e3f;
    static float picAC = 1.0f;
    static float dc = 0.0f;
    static float picNoise = 0.05f;
    static float time = 0.0f;

    static ValueADC ConvertToValueADC(float);

    static float CalculateNoise();
}


void Generator::Enable()
{
    enabled = true;

    AD7691::GeneratorChangedEvent();
}


void Generator::Disable()
{
    enabled = false;

    AD7691::GeneratorChangedEvent();
}


void Generator::SetFrequency(float freq)
{
    frequency = freq;
}


void Generator::SetPicAC(float ac)
{
    picAC = ac;
}


void Generator::SetDC(float _dc)
{
    dc = _dc;
}


ValueADC Generator::ReadValue()
{
    time += (float)(SampleRate::Current::Get().Time() * 1e-6);

    float result = picAC * std::sinf(2.0f * 3.1415926f * frequency * time) + CalculateNoise();

    return ConvertToValueADC(result);
}


bool Generator::IsEanbled()
{
    return enabled;
}


ValueADC Generator::ConvertToValueADC(float value)
{
    Math::Limitation(&value, -5.0f, 4.999962f);

    int direct = (int)(value * (1 << 17) / 5.0f);

    int convert = (direct >= 0) ? direct : (direct + (1 << 18));

    return ValueADC(convert);
}


float Generator::CalculateNoise()
{
    static float value = 0.0f;

    value += Math::Random(-picNoise, picNoise);

    Math::Limitation<float>(&value, -picNoise, picNoise);

    return value;
}
