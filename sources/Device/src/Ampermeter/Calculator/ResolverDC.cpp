// 2022/11/08 13:02:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverDC.h"


namespace ResolverDC
{
    // √рубо найти посто€нную составл€ющую
    double FindRough(const BufferADC &, int period);
}


double ResolverDC::Calculate(const BufferADC &data, int period)
{
    double dc_rough = FindRough(data, period);

    return dc_rough;
}


double ResolverDC::FindRough(const BufferADC &data, int period)
{
    float sum = 0.0f;

    for (int i = 0; i < period; i++)
    {
        sum += data[i].Real();
    }

    return sum / (float)period;
}
