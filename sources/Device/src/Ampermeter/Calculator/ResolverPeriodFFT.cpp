// 2022/11/06 17:18:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverPeriodFFT.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <limits>


ResolverPeriodFFT::ResolverPeriodFFT(const ResolverFFT &fft)
{
    CalculateSums();
    
    TimeMeterMS meter;
    
    period = (int)std::numeric_limits<int>::max();

    float min_delta = (float)std::numeric_limits<int>::max();

    const int index = fft.FindIndexFreq();

    for (int per = BufferADC::SIZE / (index + 2) * index; per < BufferADC::SIZE - 10; per++)
    {
        float delta = FindDelta2(per, min_delta);

        if (delta < min_delta)
        {
            min_delta = delta;
            period = per;
        }

        if (min_delta == 0.0f)
        {
            break;
        }
    }

    float freq = 1.0f / ((float)(BufferADC::SIZE * BufferADC::GetSampleRate().TimeUS()) * 1e-6f) * (float)index;

    LOG_WRITE("index %d, period %d, index freq %.1f, freq %f",
        fft.FindIndexFreq(),
        period,
        (double)freq,
        1.0 / ((double)((uint)index * BufferADC::GetSampleRate().TimeUS()) * 1e-6)
    );
}


float ResolverPeriodFFT::FindDelta(int per, float delta_out)
{
    float min = (float)std::numeric_limits<int>::max();
    float max = (float)std::numeric_limits<int>::min();

    for (int start = 0; start < (BufferADC::SIZE - per - 1); start++)
    {
        float integral = FindIntegral(per, start);

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


float ResolverPeriodFFT::FindDelta2(int per, float)
{
    float delta = 0.0f;

    for (int start = 0; start < (BufferADC::SIZE - per - 1); start++)
    {
        float integral = sum[start + per] - sum[start];

        if (Math::Abs<float>(integral) > delta)
        {
            delta = Math::Abs<float>(integral);
        }
    }

    return delta;
}


float ResolverPeriodFFT::FindIntegral(int line, int index_start)
{
    float result = 0.0f;

    for (int i = index_start; i < index_start + line; i++)
    {
        result += BufferADC::At(i).Real();
    }

    return result;
}


void ResolverPeriodFFT::CalculateSums()
{
    sum[0] = BufferADC::At(0).Real();

    for(int i = 1; i < BufferADC::SIZE; i++)
    {
        sum[i] = sum[i - 1] + BufferADC::At(i).Real();
    }
}
