// 2024/11/17 11:20:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"
#include "Settings/Settings.h"


uint SampleRate::TimeFullRead()
{
    // TimeUSonPoint() * 16384

    static const uint times[Count] =
    {
        180,
        1802,
        18022
    };

    return times[value];
}


pchar SampleRate::TextUGO()
{
    static const pchar texts[Count][Lang::Count] =
    {
        {"40��",  "40Hz"},
        {"4��",   "4Hz"},
        {"0.4��", "0.4Hz"}
    };

    return texts[Get()][set.lang];
}
