// 2022/11/01 11:12:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Generator/Generator.h"
#include "Ampermeter/AD7691.h"
#include "Utils/Math.h"
#include <cmath>


namespace Generator
{
    static bool enabled = false;
    static double frequency = 100.0;
    static double picAC = 1.0;
    static double dc = 0.0;
    static double picNoise = 0.2;
//    static uint64 timeUS = 0;

    static ValueADC ConvertToValueADC(double);

    double CalculateNoise();
}


void Generator::Enable()
{
    enabled = true;

    AD7691::OnEvent::GeneratorChanged();
}


void Generator::Disable()
{
    enabled = false;

    AD7691::OnEvent::GeneratorChanged();
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
    static int num_sample = 0;

    double value = 1.0;

    num_sample++;

    if (num_sample > 200)
    {
        num_sample = 0;

        if (value > 0.0)
        {
            value = -1.0;
        }
        else
        {
            value = 1.0;
        }
    }

    return ConvertToValueADC(value);

    /*
    timeUS += SampleRate::Current::Get().TimeUS();

    double time = (double)timeUS * 10e-6;

    double result = dc + picAC * std::sin(2.0 * 3.1415926535897932384626433832795 * frequency * time) + CalculateNoise();

    return ConvertToValueADC(result);
    */
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
