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
        { 61.53e-2f, 61.53e-1f, 61.53e0f, 61.53e-2f, 61.53e0f, 61.53e-1f },
        { 3244,      3234,      3234,     3250,      3232,     3233 }
    }
};


float CalibrationSettings::GetGain(int range)
{
    return gain[range];
}


void CalibrationSettings::SetGain(int range, float value)
{
    gain[range] = value;
}


int CalibrationSettings::GetZero(int range)
{
    return zero[range];
}


void CalibrationSettings::SetZero(int range, int value)
{
    zero[range] = value;
}
