// 2024/11/17 11:20:24 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AD7691.h"
#include "Settings/Settings.h"


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


pchar SampleRate::TextUGO()
{
    static const pchar texts[Count][Lang::Count] =
    {
        {"40Ãö",  "40Hz"},
        {"4Ãö",   "4Hz"},
        {"0.4Ãö", "0.4Hz"}
    };

    return texts[Get()][set.lang];
}
