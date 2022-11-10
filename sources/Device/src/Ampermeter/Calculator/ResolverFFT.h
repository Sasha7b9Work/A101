// 2022/11/01 15:04:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


class ResolverFFT
{
public:
    static const int NUM_POINTS = 1024;
    ResolverFFT();
    // Находит индекс массива частоты
    int FindIndexFreq() const;
private:
    static const int SIZE = NUM_POINTS / 2;
    uint8 data[SIZE];

    void CalculateFFT(float dataR[NUM_POINTS], float result[NUM_POINTS]);
    void Normalize(float *data, int);
};
