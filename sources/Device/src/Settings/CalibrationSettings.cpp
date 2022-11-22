// 2022/11/22 08:45:15 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/CalibrationSettings.h"
#include "Hardware/HAL/HAL.h"


namespace NS_CalibrationSettings
{
    static CalibrationSettings cal_def
    {
        { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f },
        { 3245, 3245, 3245, 3251, 3245, 3242 }
    };
}


CalibrationSettings cal = NS_CalibrationSettings::cal_def;


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


void CalibrationSettings::Save()
{
    HAL_EEPROM::Save(this);
}


void CalibrationSettings::Load()
{
    if (!HAL_EEPROM::Load(this))
    {
        *this = NS_CalibrationSettings::cal_def;
    }
}
