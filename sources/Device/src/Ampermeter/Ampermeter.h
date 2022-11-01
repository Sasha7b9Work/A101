// 2022/10/18 15:54:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SampleRate
{
    SampleRate(uint _time_points_us = 10) : time_points_us(_time_points_us) {}
    uint Time() const  { return time_points_us; }
    float Freq() const { return 1.0f / ((float)time_points_us * 1e-6f); }
private:
    uint time_points_us;
};


namespace Measurer
{
    void Init();

    void Update();
}
