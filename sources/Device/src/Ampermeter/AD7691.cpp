// 2024/11/17 11:20:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"


uint SampleRate::TimeFullRead()
{
    static const uint times[Count] =
    {
        164,
        1638,
        16384
    };

    return times[value];
}
