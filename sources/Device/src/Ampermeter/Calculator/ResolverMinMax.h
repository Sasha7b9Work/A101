// 2024/08/19 11:30:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


// Вычисляет миинмальное значение
class ResolverMinMax
{
public:

    ResolverMinMax(const Period &);

    REAL GetMin() const { return min; }

    REAL GetMax() const { return max; }

private:

    REAL min = 0.0;
    REAL max = 0.0;
};
