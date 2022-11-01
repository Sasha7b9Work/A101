// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Measurer/Measurer.h"


struct BufferADC
{
    static const int SIZE = 1024;

    BufferADC() : pointer(0) { } //-V730

    void Push(int word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    bool IsFull()     { return (pointer == SIZE); }

    void Clear(SampleRate _rate) { sampleRate = _rate; }

    int Size() const  { return SIZE; }

    float At(int pos) { return volt[pos]; }

    void ConvertToVoltage();

    float MinReal() const { return min; }

    float MaxReal() const { return max; }

    void LogUART() const;

private:
    int        pointer;         // ��������� ������������ ��� ������ ������ (������ raw)
    int        raw[SIZE];       // ������, ��������� � ���
    float      volt[SIZE];      // �������� �������� � ���
    int        min_raw;
    int        max_raw;
    float      min;
    float      max;
    SampleRate sampleRate;
};
