// 2024/08/19 11:30:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


// Вычисляет миинмальное значение
class ResolverMin
{
public:

    ResolverMin(const Period &);

    REAL GetResult() const { return result; }

private:

    REAL result = 0.0;
};
