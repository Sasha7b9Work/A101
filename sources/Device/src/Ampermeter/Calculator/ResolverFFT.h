// 2022/11/01 15:04:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


class ResolverFFT
{
public:
    ResolverFFT(const BufferADC &);
    // Находит индекс массива частоты
    int FindIndexFreq() const;
private:
    static const int SIZE = BufferADC::SIZE / 2;
    uint8 data[SIZE];

    void CalculateFFT(float dataR[BufferADC::SIZE], float result[BufferADC::SIZE]);
    void Normalize(float *data, int);

    int GetLogN();
};
