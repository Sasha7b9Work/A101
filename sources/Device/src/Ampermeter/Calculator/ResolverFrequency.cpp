// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/DiagramInput.h"
#include <limits>


ResolverFrequency::ResolverFrequency(const Period &period)
{
    int sum[BufferADC::SIZE];

    {                                                                       // Заполняем массив, по которому будем считать интегралы
        sum[0] = BufferADC::At(0).Raw();

        for (int i = 1; i < BufferADC::SIZE; i++)
        {
            sum[i] = sum[i - 1] + BufferADC::At(i).Raw();
        }
    }

    float data[1100];

    for (int i = 30; i < 1025; i++)
    {
        data[i] = CalculateMaxDelta(sum, i);
    }

    DiagramInput::InstallData(data + 30);

    frequency = period.GetFrequency();
}


float ResolverFrequency::CalculateMaxDelta(int *_sum, int period)
{
    int min = std::numeric_limits<int>::max();
    int max = std::numeric_limits<int>::min();

    for (int start = 0; start < BufferADC::SIZE - period - 10; start++)              // Смещаем отрезок
    {
        int integral = _sum[start + period] - _sum[start];

        if (integral < min)
        {
            min = integral;
        }

        if (integral > max)
        {
            max = integral;
        }
    }


    return (float)(max - min);
}
