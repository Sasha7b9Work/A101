// 2024/08/21 15:59:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include <limits>


ResolverMeasures::ResolverMeasures(const Period &period, REAL frequency)
{
//    min = std::numeric_limits<REAL>::max();

    max = BufferADC::Max().Real();
    min = BufferADC::Min().Real();

    int num_points = CalculateNumPoints(frequency);

    amplitude = CalculateAmplitude(period.first.first, num_points);
}


REAL ResolverMeasures::CalculateAmplitude(int first, int num_points) const
{
    REAL max_steady = 0.0;
    REAL min_steady = 0.0;

    CalculateMinMaxSteady(first, num_points, &min_steady, &max_steady);

    return max_steady - min_steady;
}


void ResolverMeasures::CalculateMinMaxSteady(int first, int num_points, REAL *out_min, REAL *out_max) const
{
    if (first + num_points >= BufferADC::SIZE)
    {
        first = 0;
    }

    int last = first + num_points;

    REAL res_max = -1e30;
    REAL res_min = 1e30;

    for (int i = first; i < last; i++)
    {
        REAL value = BufferADC::At(i).Real();

        if (value > res_max)
        {
            res_max = value;
        }

        if (value < res_min)
        {
            res_min = value;
        }
    }

    *out_min = res_min;
    *out_max = res_max;
}


int ResolverMeasures::CalculateNumPoints(REAL frequency) const
{
    REAL num_periods = 11.0;

    int num_points = 100000;

    while (num_points > 15000)
    {
        num_periods--;

        num_points = (int)(1e6 * num_periods / SampleRate::TimeUSonPoint() / frequency + 0.5);
    }

    return num_points;
}
