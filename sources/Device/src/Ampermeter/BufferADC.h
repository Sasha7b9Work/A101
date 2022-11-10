// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"


struct BufferADC
{
    static const int SIZE = 8 * 1024;

    BufferADC() : pointer(0) { } //-V730

    void Push(ValueADC word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    bool IsFull()     { return (pointer == SIZE); }

    void Clear(SampleRate _rate) { pointer = 0; sampleRate = _rate; }

    int Size() const  { return SIZE; }

    void CalculateLimits();

    ValueADC Min() const { return min; }

    ValueADC Max() const { return max; }

    void LogUART() const;

    SampleRate GetSampleRate() const { return sampleRate; }

    ValueADC operator[](int i) const { return raw[i]; }

private:
    int        pointer;         // Указатель используется при чтении данных (массив raw)
    ValueADC   raw[SIZE];       // Данные, считанные с АЦП
    SampleRate sampleRate;

    ValueADC   min;
    ValueADC   max;
};
