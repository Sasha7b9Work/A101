// 2022/11/24 13:20:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct TimeMeter
{
    TimeMeter();
    uint ElapsedTime();
    void Wait(uint timeMS);
private:
    uint time;
};
