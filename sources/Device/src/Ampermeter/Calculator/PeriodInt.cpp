// 2022/11/06 17:18:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/PeriodInt.h"
#include "Utils/Math.h"
#include <limits>


PeriodInt::PeriodInt(const BufferADC &buffer)
{
    sum[0] = Math::Abs<int>(buffer[0].Raw());

    for (int i = 1; i < BufferADC::SIZE; i++)
    {
        sum[i] = sum[i - 1] + Math::Abs<int>(buffer[i].Raw());
    }

    period = std::numeric_limits<int>::max();

    for (int per = BufferADC::SIZE / 2; per < BufferADC::SIZE; per++)
    {
        int delta = FindDelta(per);

        if (delta < period)
        {
            period = per;
        }
    }

    period = period;
}


int PeriodInt::FindDelta(int per)
{
    int delta = 0;

    for (int start = 0; start < (BufferADC::SIZE - per - 1); start++)
    {
        int integral = sum[start + per - 1] - sum[start];

        if (integral > delta)
        {
            delta = integral;
        }
    }

    return delta;
}
