// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    float Gain(int range);      // Коэффициент усиления
    int Zero(int range);
};


struct Settings
{
    CalibrationSettings cal;
};


extern Settings set;
