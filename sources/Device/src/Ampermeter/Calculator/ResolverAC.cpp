// 2022/11/10 10:47:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Ampermeter/Calculator/Averager.h"
#include <cmath>


ResolverAC::ResolverAC(const Period &period)
{
    double min_value = 1.0;
    int num_min = 0;

    double sum = 0.0;
    double dc = period.dc.Real();

    for (int i = period.first.first; i < period.last.first; i++)
    {
        double value = BufferADC::At(i).Real() - dc;

        if (std::fabs(value) < min_value)
        {
            min_value = std::fabs(value);
        }

        if (std::fabs(value) < 1e-4)
        {
            num_min++;
        }

        sum += value * value;
    }

    result = (double)std::sqrt(sum / (period.last.first - period.first.first));
}
