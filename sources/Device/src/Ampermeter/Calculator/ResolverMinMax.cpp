// 2024/08/19 11:30:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include <limits>


ResolverMinMax::ResolverMinMax(const Period &period)
{
    min = std::numeric_limits<REAL>::max();
    max = std::numeric_limits<REAL>::min();

    for (int i = period.first.first; i < period.last.first; i++)
    {
        REAL value = -BufferADC::At(i).Real();

        if (value < min)
        {
            min = value;
        }

        if (value > max)
        {
            max = value;
        }
    }
}
