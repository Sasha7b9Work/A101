// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"


struct BufferADC
{
    static const int SIZE = 16 * 1024;

    static void Push(ValueADC word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    static int NumElements() { return pointer; }

    static bool IsFull()     { return (pointer == SIZE); }

    static void Clear(SampleRate _rate) { pointer = 0; sampleRate = _rate; }

    static int Size() { return SIZE; }

    static void CalculateLimits();

    static ValueADC Min() { return min; }

    static ValueADC Max() { return max; }

    static void LogUART();

    static SampleRate GetSampleRate() { return sampleRate; }

    static ValueADC At(int i) { return raw[i]; }

private:
    static int        pointer;         // Указатель используется при чтении данных (массив raw)
    static ValueADC   raw[SIZE];       // Данные, считанные с АЦП
    static SampleRate sampleRate;

    static ValueADC   min;
    static ValueADC   max;
};
