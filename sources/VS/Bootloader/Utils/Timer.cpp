// 2025/11/12 22:06:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Timer.h"


void TimerMS::Reset()
{
    time_reset = (uint)std::clock();
}


uint TimerMS::ElapsedTime()
{
    return std::clock() - time_reset;
}
