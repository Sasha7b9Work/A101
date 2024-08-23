// 2024/08/21 15:59:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverAmpl.h"
#include <limits>


ResolverAmpl::ResolverAmpl(const Period &period, REAL min, REAL max)
{
    int first = period.first.first;
    int last = period.last.first;

    REAL _max = CalculateMax(first, last, period.dc.Real(), min, max);

    REAL _min = CalculateMin(first, last, period.dc.Real(), min, max);

    _ampl = _max - _min;
}


REAL ResolverAmpl::CalculateMin(int first, int last, REAL aveValue, REAL min, REAL max) const
{
    REAL result = std::numeric_limits<REAL>::max();

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

    result = sum / numSums;

    int numDeleted = 0;

    int numMin = numSums;

    REAL value = (max - min) / 9.0F;

    REAL _min = result;

    for (int i = first; i < last; i++)
    {
        REAL d = -BufferADC::At(i).Real();

        if (d < aveValue)
        {
            if (d < _min)
            {
                if (_min - d > value)
                {
                    sum -= d;
                    --numSums;
                    ++numDeleted; //-V127
                }
            }
            else if (d - _min > value) //-V2516
            {
                sum -= d;
                --numSums;
                ++numDeleted; //-V127
            }
        }
    }

    result = (numDeleted > numMin / 2.0F) ? min : (sum / numSums);

    return result;
}


REAL ResolverAmpl::CalculateMax(int first, int last, REAL aveValue, REAL min, REAL max) const
{
    REAL result = std::numeric_limits<REAL>::min();

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

    result = sum / numSums;

    int numDeleted = 0;

    int numMax = numSums;

    REAL value = (max - min) / 9.0;

    REAL _max = result;

    for (int i = first; i < last; i++)
    {
        REAL d = -BufferADC::At(i).Real();

        if (d > aveValue)
        {
            if (d > _max)
            {
                if (d - _max > value)
                {
                    sum -= d;
                    numSums--;
                    numDeleted++; //-V127
                }
            }
            else if (_max - d > value) //-V2516
            {
                sum -= d;
                numSums--;
                numDeleted++; //-V127
            }
        }
    }

    result = (numDeleted > numMax / 2) ? max : (sum / numSums);

    return result;
}
