// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    float gain[6];
    int zero[6];

    float GetGain(int range);      // Коэффициент усиления
    void SetGain(int range, float);

    int GetZero(int range);
    void SetZero(int range, int);
};


struct Settings
{
    bool middle_of_3;           // Влючение медианного фильтра
    bool smooth;                // Включение сглаживания по соседним точкам
    bool enabled_zero;          // "Влючён ноль"
    bool firLPF;                // Включён цифровой ФНЧ

    CalibrationSettings cal;
};


extern Settings set;
