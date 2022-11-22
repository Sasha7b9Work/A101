// 2022/11/22 08:45:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    float gain[6];
    int zero[6];

    float GetGain(int range);      // Коэффициент усиления
    void SetGainK(int range, float);
    float GetGainK(int range);

    int GetZero(int range);
    void SetZero(int range, int);

    void Save();
    void Load();
};


extern CalibrationSettings cal;
