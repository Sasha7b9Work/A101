// 2023/11/09 12:21:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"
#include "Settings/CalibrationSettings.h"
#include "Ampermeter/InputRelays.h"
#include "Utils/Math.h"
#include <cstdlib>
#include <cmath>


SampleRate::E SampleRate::value = SampleRate::_10us;


uint SampleRate::TimeUSonPoint()
{
    static const uint times[Count] =
    {
        10,
        100,
        1000
    };

    return times[value];
}


bool SampleRate::InRange(REAL freq, pchar *title_ru, pchar *title_en)
{
    static const pchar meas_ru_less[Count] = { "< 40 √ц", "< 4 √ц", "< 0.4 √ц" };
    static const pchar meas_en_less[Count] = { "< 40 Hz", "< 4 Hz", "< 0.4 Hz" };

    static const REAL value_min[Count] = { 40.0, 4.0, 0.4 };

    static const pchar meas_ru_abov[Count] = { "> 5 к√ц", "> 400 √ц", "> 40 √ц" };
    static const pchar meas_en_abov[Count] = { "> 5 kHz", "> 400 Hz", "> 40 Hz" };

    static const REAL value_max[Count] = { 5e3, 400.0, 40.0 };

    if (freq < value_min[value])
    {
        *title_ru = meas_ru_less[value];
        *title_en = meas_en_less[value];

        return false;
    }
    else if (freq > value_max[value])
    {
        *title_ru = meas_ru_abov[value];
        *title_en = meas_en_abov[value];

        return false;
    }

    return true;
}


ValueADC::ValueADC(int reading)
{
    value = reading - cal.zero[Range::Current()]._GetFull();
}


void AD7691::Init()
{

}


ValueADC AD7691::ReadValue()
{
    return ValueADC(ReadValueRAW());
}


static float GetSample(float freq, int /*num_sample*/)
{
    float T = 1.0f / freq;

    float samples_in_T = T / SampleRate::TimeUSonPoint() / 1e-6f;

    float radians_in_T = 2.0f * M_PI;

    float radians_in_sample = radians_in_T / samples_in_T;

    static float value = 0.0f;

    value += radians_in_sample;

    return std::sinf(value);
}


int AD7691::ReadValueRAW()
{
    float amplitude = 0.5f;

    static int counter = 0;

    float value = amplitude * GetSample(45.0f, counter++);

//    value += (float)std::rand() / (float)RAND_MAX / 100.0f;

//    value += 0.1f;

    value *= 0x1FFFF;           // 17 бит в единичке - максимальное значение

    return (int)value;
}


int AD7691::GetAverageValue()
{
    return 0;
}
