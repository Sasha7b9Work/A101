// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"


ResolverFrequency::ResolverFrequency(const Period &period)
{
    frequency = period.GetFrequency();
}
