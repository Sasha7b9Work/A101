// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/DiagramInput.h"
#include "Ampermeter/Calculator/Averager.h"
#include <limits>


ResolverFrequency::ResolverFrequency(const Period &period)
{
    static const int SIZE = 7;

    Averager<float, SIZE> averager;

    float sum[BufferADC::SIZE];

    {                                                                       // Заполняем массив, по которому будем считать интегралы
        for (int i = 1; i < BufferADC::SIZE; i++)
        {
            averager.Push((float)BufferADC::At(i).Real());
            sum[i] = averager.Get();
        }
    }

    for (int i = 0; i < SIZE; i++)
    {
        sum[i] = sum[SIZE];
    }

    DiagramInput::InstallData(sum);

    frequency = period.GetFrequency();
}
