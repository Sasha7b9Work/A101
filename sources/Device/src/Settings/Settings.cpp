// 2022/11/14 13:39:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"


namespace NS_Settings
{
    static Settings set_def
    {
        false,      // middle_of_3
        false,      // smooth
        false,      // enabled_zero
        false       // firLPF
    };

    static Settings stored = set_def;
}

Settings set = NS_Settings::set_def;


void Settings::Store()
{
    NS_Settings::stored = *this;
}


void Settings::Restore()
{
    *this = NS_Settings::stored;
}


void Settings::Save()
{
    HAL_EEPROM::Save(this);
}


void Settings::Load()
{
    if (!HAL_EEPROM::Load(this))
    {
        *this = NS_Settings::set_def;
    }
}
