// 2022/11/01 11:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Ampermeter/AD7691.h"
#include "Utils/Math.h"
#include <cmath>


namespace Generator
{
    static bool enabled = false;
    static double frequency = 1000.0;
    static double picAC = 1.0;
    static double dc = -0.2555;
    static double picNoise = 0.0;
    static double time = 0.0;

    static ValueADC ConvertToValueADC(double);

    static double CalculateNoise();
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


void Generator::SetFrequency(double freq)
{
    frequency = freq;
}


void Generator::SetPicAC(double ac)
{
    picAC = ac;
}


void Generator::SetDC(double _dc)
{
    dc = _dc;
}


ValueADC Generator::ReadValue()
{
    time += SampleRate::Current::Get().Time() * 1e-6;

    double result = dc + picAC * std::sin(2.0 * 3.1415926535897932384626433832795 * frequency * time) + CalculateNoise();

    return ConvertToValueADC(result);
}


bool Generator::IsEanbled()
{
    return enabled;
}


ValueADC Generator::ConvertToValueADC(double value)
{
    Math::Limitation(&value, -5.0, 4.999962);

    int direct = (int)(value * (1 << 17) / 5.0);

    int convert = (direct >= 0) ? direct : (direct + (1 << 18));

    return ValueADC(convert);
}


double Generator::CalculateNoise()
{
    static double value = 0.0;

    value += Math::Random(-picNoise, picNoise);

    Math::Limitation<double>(&value, -picNoise, picNoise);

    return value;
}
