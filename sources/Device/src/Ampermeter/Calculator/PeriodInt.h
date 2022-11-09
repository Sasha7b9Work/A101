// 2022/11/06 17:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"
#include "Ampermeter/Calculator/FFT.h"


class PeriodInt
{
public:
    PeriodInt(const BufferADC &, const FFT &);
    int ToPoints() const { return period; }
private:
    int period = -1;
    int sum[BufferADC::SIZE];

    // Найти разницу между максимальным и минимальным интегралом при изменении начала измеряемого отрезка от
    // минимального возможного до минимально возможного
    // Если 
    int FindDelta(const BufferADC &, int per, int delta_out);

    int FindDelta2(const BufferADC &, int per, int delta_out);

    // Найти интеграл для данного отрезка, начиная с точки index_start
    int FindIntegral(const BufferADC &, int line, int index_start);

    void CalculateSums(const BufferADC &);
};
