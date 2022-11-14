// 2022/11/14 13:39:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings.h"


Settings set;


float CalibrationSettings::Gain(int range)
{
    static const float koeff[6] = { 1e-2f, 1e-1f, 1e0f, 1e-2f, 1e-1f, 1e0f };

    return koeff[range] * 61.521f;
}
