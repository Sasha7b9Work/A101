// 2022/11/08 13:02:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/FinderDC.h"


namespace FinderDC
{
    // √рубо найти посто€нную составл€ющую
    double FindRough(const BufferADC &, int period);
}


double FinderDC::Calculate(const BufferADC &data, int period)
{
    double dc_rough = FindRough(data, period);

    return dc_rough;
}


double FinderDC::FindRough(const BufferADC &data, int period)
{
    double sum = 0.0;

    for (int i = 0; i < period; i++)
    {
        sum += data[i].Real();
    }

    return sum / (double)period;
}
