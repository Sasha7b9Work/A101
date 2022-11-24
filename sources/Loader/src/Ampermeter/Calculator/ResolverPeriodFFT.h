// 2022/11/06 17:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"
#include "Ampermeter/Calculator/ResolverFFT.h"


/*
*  Использует для нахождения периода БПФ
*/


class ResolverPeriodFFT
{
public:
    ResolverPeriodFFT(const ResolverFFT &);
    int ToPoints() const { return period; }
private:
    int period = -1;
    int sum[BufferADC::SIZE];

    // Найти разницу между максимальным и минимальным интегралом при изменении начала измеряемого отрезка от
    // минимального возможного до минимально возможного
    // Если 
    int FindDelta(int per, int delta_out);

    int FindDelta2(int per, int delta_out);

    // Найти интеграл для данного отрезка, начиная с точки index_start
    int FindIntegral(int line, int index_start);

    void CalculateSums();
};
