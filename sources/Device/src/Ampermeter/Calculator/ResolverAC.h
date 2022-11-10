// 2022/11/10 10:47:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


class ResolverAC
{
public:

    ResolverAC(const BufferADC &, const Period &);

    float GetResult() const { return result; }

private:

    float result = 0.0f;
};
