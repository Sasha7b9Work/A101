// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/DiagramInput.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Nextion/Nextion.h"
#include "Nextion/Controls.h"
#include <limits>
#include <cstdio>


#define DELTA(o)    (sum[(o) + 1] - sum[(o)])


ResolverFrequency::ResolverFrequency(const Period &period)
{
    float sum[BufferADC::SIZE];

    Averager<float, 10> averager;

    sum[0] = averager.Push((float)BufferADC::At(0).Real());

    for (int i = 1; i < BufferADC::SIZE; i++)                       // –ассчитываем массив сумм касательных
    {
        sum[i] = averager.Push(sum[i - 1] + (float)BufferADC::At(i).Real());
    }

    // «десь у нас график, представл€ющий гнутую синусоиду.  оличество верхних вершин соотвествует количеству периодов в исходной последовательности

    int first = period.first.first;
    int last = period.last.first;

    int counter = CalculateCounter(sum, first, last, 5);

    frequency = (float)counter * 1e6f / (last - first) / SampleRate::TimeUSonPoint();

//    char buffer[65];
//
//    sprintf(buffer, "%d:%d %d:%d %d:%d",
//        counter_2, (int)(counter_2 * 1e6f / (last - first) / SampleRate::TimeUSonPoint()),
//        counter_5, (int)(counter_5 * 1e6f / (last - first) / SampleRate::TimeUSonPoint()),
//        counter_10, (int)(counter_10 * 1e6f / (last - first) / SampleRate::TimeUSonPoint()),
//        counter_20, (int)(counter_20 * 1e6f / (last - first) / SampleRate::TimeUSonPoint())
//    );
//
//    Nextion::DrawString({ 0, 20, 700, 40 }, Font::_1_GB42b, Color::White, Color::Background, buffer);

    if (SampleRate::Get() == SampleRate::_100us)
    {
        frequency = 40.0 + (frequency - 44.0) * 450.0 / 443.0;
    }
    else if (SampleRate::Get() == SampleRate::_1000us)
    {
        frequency = 40.0 + (frequency - 42.5) * 10.5 / 10.0;
    }
}


int ResolverFrequency::CalculateCounter(float sum[BufferADC::SIZE], int first, int last, int step)
{
    int counter = 0;

    for (int i = first; i < last; i++)
    {
        if (DELTA(i) > 0.0f && DELTA(i + 1) <= 0.0f)
        {
            i += step;
            counter++;
            continue;
        }
    }

    return counter;
}
