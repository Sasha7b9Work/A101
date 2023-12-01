// 2022/11/10 10:47:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Ampermeter/Calculator/Averager.h"
#include <cmath>


ResolverAC::ResolverAC(const Period &period)
{
    float min_value = 1.0f;
    int num_min = 0;

    float sum = 0.0f;
    float dc = period.dc.Real();

    for (int i = period.first.first; i < period.last.first; i++)
    {
        float value = bufADC.At(i).Real() - dc;

        if (std::fabs(value) < min_value)
        {
            min_value = std::fabs(value);
        }

        if (std::fabs(value) < 1e-4f)
        {
            num_min++;
        }

        sum += value * value;
    }

    result = (float)std::sqrt(sum / (period.last.first - period.first.first));
}
