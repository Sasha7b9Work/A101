// 2022/11/06 17:18:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverPeriodFFT.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include <limits>


ResolverPeriodFFT::ResolverPeriodFFT(const BufferADC &buffer, const ResolverFFT &fft)
{
    CalculateSums(buffer);
    
    TimeMeterMS meter;
    
    period = (int)std::numeric_limits<int>::max();

    int min_delta = (int)std::numeric_limits<int>::max();

    const int index = fft.FindIndexFreq();

    for (int per = BufferADC::SIZE / (index + 2) * index; per < BufferADC::SIZE - 10; per++)
    {
        int delta = FindDelta2(buffer, per, min_delta);

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

    float freq = 1.0f / ((float)(BufferADC::SIZE * buffer.GetSampleRate().TimeUS()) * 1e-6f) * (float)index;

    LOG_WRITE("index %d, period %d, index freq %.1f, freq %f",
        fft.FindIndexFreq(),
        period,
        (double)freq,
        1.0 / ((double)((uint)index * buffer.GetSampleRate().TimeUS()) * 1e-6)
    );
}


int ResolverPeriodFFT::FindDelta(const BufferADC &buffer, int per, int delta_out)
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


//int PeriodInt::FindDelta2(const BufferADC &buffer, int per, int delta_out)
//{  
//    for(int start = 0; start < BufferADC::SIZE - per; start++)
//    {
//        int min = (int)std::numeric_limits<int>::max();
//        int max = (int)std::numeric_limits<int>::min();
//
//        
//    }
//}


int ResolverPeriodFFT::FindDelta2(const BufferADC &, int per, int)
{
    int delta = 0;

    for (int start = 0; start < (BufferADC::SIZE - per - 1); start++)
    {
        int integral = sum[start + per] - sum[start];

        if (Math::Abs<int>(integral) > delta)
        {
            delta = Math::Abs<int>(integral);
        }
    }

    return delta;
}


int ResolverPeriodFFT::FindIntegral(const BufferADC &buffer, int line, int index_start)
{
    int result = 0;

    for (int i = index_start; i < index_start + line; i++)
    {
        result += buffer[i].Raw();
    }

    return result;
}


void ResolverPeriodFFT::CalculateSums(const BufferADC &buffer)
{
    sum[0] = buffer[0].Raw();
    
    for(int i = 1; i < BufferADC::SIZE; i++)
    {
        sum[i] = sum[i - 1] + buffer[i].Raw();
    }
}
