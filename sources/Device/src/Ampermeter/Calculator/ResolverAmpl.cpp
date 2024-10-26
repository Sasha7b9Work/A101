// 2024/08/21 15:59:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include <limits>


ResolverAmpl::ResolverAmpl(const Period &period)
{
    int first = period.first.first;
    int last = period.last.first;

    max = CalculateMax(first, last, period.dc.Real());

    min = CalculateMin(first, last, period.dc.Real());
}


REAL ResolverAmpl::CalculateMin(int first, int last, REAL aveValue) const
{
    REAL sum = 0.0;
    int numSums = 0;

    for (int i = first; i < last; i++)
    {
        REAL d = -BufferADC::At(i).Real();

        if (d < aveValue)
        {
            sum += d; //-V127
            numSums++; //-V127
        }
    }

    return sum / numSums;
}


REAL ResolverAmpl::CalculateMax(int first, int last, REAL aveValue) const
{
    REAL sum = 0.0;
    int numSums = 0;

    for (int i = first; i < last; i++)
    {
        REAL d = -BufferADC::At(i).Real();

        if (d > aveValue)
        {
            sum += d; //-V127
            numSums++; //-V127
        }
    }

    return sum / numSums;
}
