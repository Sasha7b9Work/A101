// 2022/10/18 15:54:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Ampermeter
{
    void Init();

    void Update();

    void ReadData();

    // если out_of_range == true - выход за границы диапазона
    float GetDC(bool *out_of_range);
    float GetAC(bool *out_of_range);
    float GetAmpl(bool *out_of_range);
    float GetPeak(bool *out_of_range);
    float GetMin(bool *out_of_range);
    float GetMax(bool *out_of_range);
}
