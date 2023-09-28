// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/Calculator/Averager.h"
#include <limits>


int        BufferADC::pointer = 0;                                      // ”казатель используетс€ при чтении данных (массив raw)
ValueADC   BufferADC::raw[SIZE] __attribute__((section("CCM_DATA")));   // ƒанные, считанные с ј÷ѕ
SampleRate BufferADC::sampleRate;

ValueADC   BufferADC::min;
ValueADC   BufferADC::max;


void BufferADC::CalculateLimits()
{
    int64 sum = 0;

    min = ValueADC::MAX;
    max = ValueADC::MIN;

    for (int i = 0; i < SIZE; i++)
    {
        ValueADC value = raw[i];

        sum += value;

        if (value < min) { min = value; }
        if (value > max) { max = value; }
    }
}


void BufferADC::LogUART()
{
    for (int i = 0; i < Size(); i++)
    {
        LOG_WRITE("%d", raw[i].Raw());
    }
}


static int middle_of_3(int a, int b, int c)
{
    if ((a <= b) && (a <= c))
    {
        return (b <= c) ? b : c;
    }
    else if ((b <= a) && (b <= c))
    {
        return (a <= c) ? a : c;
    }
    return (a <= b) ? a : b;
}


void BufferADC::MiddleOf3()
{
    int raw_i_1 = raw[0];   // Ёлемент raw[i - 1]
    int raw_i_0 = 0;        // Ёлемент raw[i]

    for (int i = 1; i < SIZE - 1; i++)
    {
        raw_i_0 = raw[i];

        raw[i] = ValueADC::FromRaw(middle_of_3(raw_i_1, raw[i], raw[i + 1]));

        raw_i_1 = raw_i_0;
    }
}


void BufferADC::SmoothOut()
{
    Averager<int, 3> averager;

    for (int i = 0; i < SIZE; i++)
    {
        averager.Push(raw[i].Raw());
        raw[i] = ValueADC::FromRaw(averager.Get());
    }
}
