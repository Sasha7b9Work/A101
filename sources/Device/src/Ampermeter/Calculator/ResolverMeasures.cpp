// 2024/08/21 15:59:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include <limits>


ResolverMeasures::ResolverMeasures(const Period &period, REAL frequency)
{
//    min = std::numeric_limits<REAL>::max();

    max = -BufferADC::Max().Real();
    min = -BufferADC::Min().Real();

    int num_points = 0;
    int num_periods = 0;
    
    CalculateNumPoints(frequency, &num_points, &num_periods);

    amplitude = CalculateAmplitudeSteady(period.first.first, num_points, num_periods);
}


REAL ResolverMeasures::CalculateAmplitudeSteady(int first, int num_points, int num_periods) const
{
    if (num_points < 10)
    {
        return std::numeric_limits<REAL>::infinity();
    }
    else
    {
        REAL sum_ampl = 0.0;

        if (first + num_points >= BufferADC::SIZE)
        {
            first = 0;
        }

        float points_on_period = (float)num_points / num_periods;

        for (int i = 0; i < num_periods; i++)
        {
            sum_ampl += CalculateAmplitudeSteadyPeriod((int)(first + points_on_period * i + 0.5f), (int)(points_on_period + 0.5f));
        }

        return sum_ampl / num_periods;
    }
}


REAL ResolverMeasures::CalculateAmplitudeSteadyPeriod(int first, int num_points) const
{
    int last = first + num_points;

    REAL res_max = -1e30;
    REAL res_min = 1e30;

    for (int i = first; i < last; i++)
    {
        REAL value = -BufferADC::At(i).Real();

        if (value > res_max)
        {
            res_max = value;
        }

        if (value < res_min)
        {
            res_min = value;
        }
    }

    return res_max - res_min;
}


void ResolverMeasures::CalculateNumPoints(REAL frequency, int *out_num_points, int *out_num_periods) const
{
    int num_periods = 11;

    int num_points = 100000;

    while (num_points > 15000)
    {
        num_periods--;

        num_points = (int)(1e6 * num_periods / SampleRate::TimeUSonPoint() / frequency + 0.5);
    }

    *out_num_points = num_points;
    *out_num_periods = num_periods;
}
