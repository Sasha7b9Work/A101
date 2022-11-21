// 2022/11/14 13:39:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"


Settings set
{
    false,      // middle_of_3
    false,      // smooth
    false,      // enabled_zero
    false,      // firLPF
    {
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 3245, 3245, 3245, 3251, 3245, 3242 }
    }
};


float CalibrationSettings::GetGain(int range)
{
    static const float k[6] = { 61.53e-2f, 61.53e-1f, 61.53e0f, 61.53e-2f, 61.53e0f, 61.53e-1f };

    return gain[range] * k[range];
}


void CalibrationSettings::SetGainK(int range, float value)
{
    gain[range] = value;
}


float CalibrationSettings::GetGainK(int range)
{
    return gain[range];
}


int CalibrationSettings::GetZero(int range)
{
    return zero[range];
}


void CalibrationSettings::SetZero(int range, int value)
{
    zero[range] = value;
}
