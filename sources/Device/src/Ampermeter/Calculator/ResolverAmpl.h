// 2024/08/21 15:57:39 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


// Вычисляет амплутудное значение
class ResolverAmpl
{
public:

    ResolverAmpl(const Period &, REAL min, REAL max);

    REAL GetResult() const { return _ampl; }

private:

    REAL _ampl = 0.0f;

    REAL CalculateMax(int first, int last, REAL ave, REAL min, REAL max) const;

    REAL CalculateMin(int first, int last, REAL ave, REAL min, REAL max) const;
};
