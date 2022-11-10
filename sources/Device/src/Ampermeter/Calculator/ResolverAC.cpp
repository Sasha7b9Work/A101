// 2022/11/10 10:47:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include <cmath>


ResolverAC::ResolverAC(const BufferADC &data, const Period &period)
{
    float sum = 0.0f;

    for (int i = period.first.first; i < period.last.first; i++)
    {
        float value = data[i].Real() - period.dc.Real();

        sum += value * value;
    }

    result = std::sqrtf(sum / (period.last.first - period.first.first));
}
