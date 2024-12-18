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

    for (int i = 1; i < BufferADC::SIZE; i++)                       // ������������ ������ ���� �����������
    {
        sum[i] = averager.Push(sum[i - 1] + (float)BufferADC::At(i).Real());
    }

    // ����� � ��� ������, �������������� ������ ���������. ���������� ������� ������ ������������ ���������� �������� � �������� ������������������

    int first = period.first.first;
    int last = period.last.first;

    int counter = CalculateCounter(sum, first, last, 5);

    frequency = (float)counter * 1e6f / (last - first) / SampleRate::TimeUSonPoint();
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
