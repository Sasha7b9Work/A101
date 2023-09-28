// 2022/10/18 15:54:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Measure
{
    float value = 0.0f;
    bool out_of_range = false;
};


namespace Ampermeter
{
    void Init();

    void Update();

    void ReadData();

    // если out_of_range == true - выход за границы диапазона
    Measure GetDC();
    Measure GetAC();
    Measure GetAmpl();
    Measure GetPeak();
    Measure GetMin();
    Measure GetMax();
}
