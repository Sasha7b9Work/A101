// 2022/11/24 13:19:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Timer.h"


TimeMeter::TimeMeter()
{
    time = GetTickCount();
}


uint TimeMeter::ElapsedTime()
{
    return GetTickCount() - time;
}


void TimeMeter::Wait(uint timeMS)
{
    time = GetTickCount();

    while (ElapsedTime() < timeMS)
    {
    }
}
