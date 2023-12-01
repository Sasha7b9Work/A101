// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"


namespace BufferADC
{
    static const int SIZE = 16 * 1024;

    void Push(ValueADC word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    int NumElements() { return pointer; }

    bool IsFull()     { return (pointer == SIZE); }

    void Clear(SampleRate _rate) { pointer = 0; sampleRate = _rate; }

    int Size() { return SIZE; }

    void CalculateLimits();

    ValueADC Min() { return min; }

    ValueADC Max() { return max; }

    void LogUART();

    SampleRate GetSampleRate() { return sampleRate; }

    ValueADC At(int i) { return raw[i]; }

    // Прменить медианный фильтр по трём
    void MiddleOf3();

    // Сгладить сигнал
    void SmoothOut();
};
