// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include <limits>


ResolverFrequency::ResolverFrequency(const Period &period)
{
    static float sum[BufferADC::SIZE] __attribute__((section("ram2")));

    {                                                                       // Заполняем массив, по которому будем считать интегралы
        sum[0] = (float)BufferADC::At(0);

        for (int i = 1; i < BufferADC::SIZE; i++)
        {
            sum[i] = sum[i - 1] + (float)BufferADC::At(i).Real();
        }
    }

//    float prev_delta = 0.0f;
//    int T = 5;
//    int num_deltas = 0;     // 

    for (int T = 25; T < 35; T++)                           // Будем считать дельту интеграла при перемещении участка фиксированной длины
    {                                                           // от начала сигнала к концу. Начинаем с минимального значения, чтобы найти минимальный период
        float delta = CalculateMaxDelta(sum, T);

        LOG_WRITE("delta %d = %f", T, delta);
    }

    for (int T = 60; T < 65; T++)                           // Будем считать дельту интеграла при перемещении участка фиксированной длины
    {                                                           // от начала сигнала к концу. Начинаем с минимального значения, чтобы найти минимальный период
        float delta = CalculateMaxDelta(sum, T);

        LOG_WRITE("delta %d = %f", T, delta);
    }

    for (int T = 90; T < 97; T++)                           // Будем считать дельту интеграла при перемещении участка фиксированной длины
    {                                                           // от начала сигнала к концу. Начинаем с минимального значения, чтобы найти минимальный период
        float delta = CalculateMaxDelta(sum, T);

        LOG_WRITE("delta %d = %f", T, delta);
    }


    LOG_WRITE(" ");

    frequency = period.GetFrequency();
}


float ResolverFrequency::CalculateMaxDelta(float *_sum, int period)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    for (int start = 0; start < BufferADC::SIZE - 1 - period; start++)              // Смещаем отрезок
    {
        float integral = _sum[start + period] - _sum[start];

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
