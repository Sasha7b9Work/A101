// 2022/10/18 16:57:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SampleRate
{
    SampleRate(uint _time_points_us = 10) : time_points_us(_time_points_us) {}
    uint Time() const { return time_points_us; }
    float Freq() const { return 1.0f / ((float)time_points_us * 1e-6f); }
private:
    uint time_points_us;
};


struct ValueADC
{
    ValueADC(int _reading);
    float Real();
private:
    int value;        // Прочитанное значение, уже преобразованное
};


// АЦП
namespace AD7691
{
    void Init();

    int ReadValue();

    // При вкл/откл генератора нужно вызывать эту функцию
    void GeneratorChangedEvent();

    SampleRate CurrentSampleRate();
}
