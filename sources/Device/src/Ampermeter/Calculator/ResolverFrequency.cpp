// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/DiagramInput.h"
#include "Ampermeter/Calculator/Averager.h"
#include <limits>


#define DELTA(o)    (sum[(o) + 1] - sum[(o)])


ResolverFrequency::ResolverFrequency(const Period &period)
{
    float sum[BufferADC::SIZE];

    sum[0] = (float)BufferADC::At(0).Real();

    for (int i = 1; i < BufferADC::SIZE; i++)                       // –ассчитываем массив сумм касательных
    {
        sum[i] = sum[i - 1] + (float)BufferADC::At(i).Real();
    }

    // «десь у нас график, представл€ющий гнутую синусоиду.  оличество верхних вершин соотвествует количеству периодов в исходной последовательности

    int first = period.first.first;
    int last = period.last.first;

    int counter = 0;

    for (int i = first; i < last; i++)
    {
        if (DELTA(i) >= 0.0f && DELTA(i + 1) < 0.0f)
        {
            i += 2;

            counter++;

            continue;
        }

        if (DELTA(i) > 0.0f && DELTA(i + 1) <= 0.0f)
        {
            i += 2;

            counter++;

            continue;
        }
    }

    DiagramInput::InstallData(sum);

    frequency = 1.0f / ((last - first) / counter * (float)SampleRate::Current().Get().TimeUS());
}
