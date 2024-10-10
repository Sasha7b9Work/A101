// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include <limits>


ResolverFrequency::ResolverFrequency(const Period &period)
{
    float sum[BufferADC::SIZE];

    const int first = period.first.first;
    const int lenght = period.Lenght();

    {                                                                       // «аполн€ем массив, по которому будем считать интегралы
        sum[0] = (float)BufferADC::At(first);

        for (int i = 1; i < lenght; i++)
        {
            sum[i] = sum[i - 1] + (float)BufferADC::At(i + first);
        }
    }

//    float prev_delta = 0.0f;
    int T = 5;
//    int num_deltas = 0;     // 

    for (; T < 100; T++)                           // Ѕудем считать дельту интеграла при перемещении участка фиксированной длины
    {                                                           // от начала сигнала к концу. Ќачинаем с минимального значени€, чтобы найти минимальный период
        float delta = CalculateMaxDelta(sum, T);

        LOG_WRITE("delta = %f", delta);
    }

    frequency = period.GetFrequency();
}


float ResolverFrequency::CalculateMaxDelta(float *sum, int period)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    for (int start = 0; start < BufferADC::SIZE - 1 - period; start++)              // —мещаем отрезок
    {
        min = std::numeric_limits<float>::max();
        max = std::numeric_limits<float>::min();

        float integral = sum[start + period] - sum[start];

        if (integral < min)
        {
            min = integral;
        }

        if (integral > max)
        {
            max = integral;
        }
    }


    return max - min;
}
