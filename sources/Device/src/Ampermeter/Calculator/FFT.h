// 2022/11/01 15:04:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


class FFT
{
public:
    FFT(const BufferADC &);
private:
    static const int SIZE = 512;
    uint8 data[SIZE];

    void CalculateFFT(float dataR[1024], float result[2024]);
    void Normalize(float *data, int);

    // Находит индекс массива частоты
    int FindFreq();
};
