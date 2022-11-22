// 2022/11/14 13:39:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


namespace NS_Settings
{
    static Settings set_def
    {
        sizeof(Settings),
        0,
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
    Settings loaded;

    if (!HAL_EEPROM::Load(&loaded) || !loaded.IsEqual(this))
    {
        HAL_EEPROM::Save(this);
    }
}


bool Settings::IsEqual(Settings *rhs)
{
    if (size != rhs->size)
    {
        return false;
    }

    return std::memcmp(PointerToFirstData(), rhs->PointerToFirstData(), SizeData()) == 0;
}


uint Settings::SizeData()
{
    return sizeof(Settings) - 2 * sizeof(size);
}


uint8 *Settings::PointerToFirstData()
{
    return (uint8 *)this + 2 * sizeof(size);
}


void Settings::Load()
{
    if (!HAL_EEPROM::Load(this))
    {
        *this = NS_Settings::set_def;
    }
}


uint Settings::CalculateCRC32()
{
    return HAL_CRC32::Calculate(PointerToFirstData(), SizeData());
}
