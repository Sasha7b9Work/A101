// 2024/08/19 11:30:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverMin.h"
#include <limits>


ResolverMin::ResolverMin(const Period &)
{
    REAL min_value = std::numeric_limits<REAL>::max();

    for (int i = 0; i < BufferADC::NumElements(); i++)
    {
        REAL value = BufferADC::At(i).Real();

        if (value < min_value)
        {
            min_value = value;
        }
    }

    result = min_value;
}
