// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    float GetGain(int range);      // Коэффициент усиления
    int GetZero(int range);
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
