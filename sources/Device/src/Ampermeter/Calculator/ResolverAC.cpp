// 2022/11/10 10:47:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include "Ampermeter/Calculator/Averager.h"
#include <cmath>


ResolverAC::ResolverAC(const Period &period)
{
    REAL min_value = 1.0;

    REAL sum = 0.0;
    REAL dc = period.dc.Real();

    for (int i = period.first.first; i < period.last.first; i++)
    {
        REAL value = BufferADC::At(i).Real() - dc;

        if (std::fabs(value) < min_value)
        {
            min_value = std::fabs(value);
        }

        sum += value * value;
    }

    Period per = period;
    per.last.first = period.first.first;

    result = std::sqrt(sum / (per.last.first - per.first.first));
}
