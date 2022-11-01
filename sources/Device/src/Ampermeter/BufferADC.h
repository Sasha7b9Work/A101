// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"


struct BufferADC
{
    static const int SIZE = 1024;

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

    float At(int pos) { return volt[pos]; }

    void ConvertToVoltage();

    float MinReal() const { return min; }

    float MaxReal() const { return max; }

    void LogUART() const;

private:
    int        pointer;         // Указатель используется при чтении данных (массив raw)
    ValueADC   raw[SIZE];       // Данные, считанные с АЦП
    float      volt[SIZE];      // Реальные значения с АЦП
    int        min_raw;
    int        max_raw;
    float      min;
    float      max;
    SampleRate sampleRate;
};
