// 2022/11/06 17:18:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/PeriodInt.h"
#include "Utils/Math.h"
#include <limits>


PeriodInt::PeriodInt(const BufferADC &buffer, const FFT &fft)
{
    period = (int)std::numeric_limits<int>::max();

    int min_delta = (int)std::numeric_limits<int>::max();

    for (int per = BufferADC::SIZE / 2; per < BufferADC::SIZE - 10; per++)
    {
        int delta = FindDelta(buffer, per, min_delta);

        if (delta < min_delta)
        {
            min_delta = delta;
            period = per;
        }

        if (min_delta == 0)
        {
            break;
        }
    }

    LOG_WRITE("index %d", fft.FindIndexFreq());

    LOG_WRITE("period %d", period);
}


int PeriodInt::FindDelta(const BufferADC &buffer, int per, int delta_out)
{
    int min = (int)std::numeric_limits<int>::max();
    int max = (int)std::numeric_limits<int>::min();

    for (int start = 0; start < (BufferADC::SIZE - per - 1); start++)
    {
        int integral = FindIntegral(buffer, per, start);

        if (integral < min)
        {
            min = integral;
        }

        if (integral > max)
        {
            max = integral;
        }

        if (max - min >= delta_out)
        {
            break;
        }
    }

    return max - min;
}


int PeriodInt::FindIntegral(const BufferADC &buffer, int line, int index_start)
{
    int result = 0;

    for (int i = index_start; i < index_start + line; i++)
    {
        result += buffer[i].Raw();
    }

    return result;
}
