// 2022/11/06 17:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


class PeriodInt
{
public:
    PeriodInt(const BufferADC &);
    int ToPoints() const { return period; }
private:
    int period = -1;
    // В этом массиве хранятся суммы значений от 1-й точки до последней, начиная с нулевой
    int sum[BufferADC::SIZE];

    // Найти разницу между максимальным и минимальным интегралом при изменении начала измеряемого отрезка от
    // минимального возможного до минимально возможного
    int FindDelta(int per);
};
