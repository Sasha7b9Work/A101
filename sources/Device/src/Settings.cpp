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
    }
};


float CalibrationSettings::Gain(int range)
{
    static const float koeff[6] = { 1e-2f, 1e-1f, 1e0f, 1e-2f, 1e0f, 1e-1f };

    return koeff[range] * 61.53f;
}



int CalibrationSettings::Zero(int range)
{
    static const int zeros[6] = { 3244, 3234, 3234, 3250, 3232, 3233 };

    return zeros[range];
}
