// 2022/10/18 16:18:26 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/Calculator/Averager.h"
#include "Utils/String.h"
#include <limits>


namespace BufferADC
{
    static int        pointer;                                      // ”казатель используетс€ при чтении данных (массив raw)
    static ValueADC   raw[SIZE]  __attribute__((section("ram2")));  // ƒанные, считанные с ј÷ѕ

    static ValueADC   min;
    static ValueADC   max;
}


void BufferADC::Push(ValueADC word)
{
    if (pointer < SIZE)
    {
        raw[pointer++] = word;
    }
}


int BufferADC::NumElements()
{
    return pointer;
}


bool BufferADC::IsFull()
{
    return pointer == SIZE;
}


void BufferADC::Clear()
{
    pointer = 0;
}


double BufferADC::Min()
{
    return min.Real();
}


double BufferADC::Max()
{
    return max.Real();
}


ValueADC BufferADC::At(int i)
{
    return (i < SIZE) ? raw[i] : ValueADC(0);
}


void BufferADC::CalculateLimits()
{
    min = ValueADC::FromRaw(1 << 17);
    max = ValueADC::FromRaw(-(1 << 17));

    for (int i = 0; i < SIZE; i++)
    {
        ValueADC value = raw[i];

        if (value < min) { min = value; }
        if (value > max) { max = value; }
    }
}


void BufferADC::LogUART()
{
    for (int i = 0; i < SIZE; i++)
    {
        LOG_WRITE("%d", raw[i].Raw());
    }
}


void BufferADC::MiddleOf3()
{
    int raw_i_1 = raw[0];   // Ёлемент raw[i - 1]
    int raw_i_0 = 0;        // Ёлемент raw[i]

    for (int i = 1; i < SIZE - 1; i++)
    {
        raw_i_0 = raw[i];

        raw[i] = ValueADC::FromRaw(Math::middle_of_3<int>(raw_i_1, raw[i], raw[i + 1]));

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
