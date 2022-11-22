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


void Settings::Store() const
{
    NS_Settings::stored = *this;
}


void Settings::Restore()
{
    *this = NS_Settings::stored;
}


void Settings::Save()
{
    size = sizeof(*this);
    crc32 = CalculateCRC32();

    Settings loaded;

    if (!HAL_EEPROM::Load(&loaded) || !loaded.IsEqual(this))
    {
        HAL_EEPROM::Save(this);
    }
}


bool Settings::IsEqual(const Settings *rhs) const
{
    if (size != rhs->size)
    {
        return false;
    }

    const uint8 *lhs_data = PointerToFirstData();
    const uint8 *rhs_data = rhs->PointerToFirstData();
    uint size_data = SizeData();

    bool result = std::memcmp(lhs_data, rhs_data, size_data) == 0;

    return result;
}


uint Settings::SizeData() const
{
    uint result = sizeof(*this) - 2 * sizeof(size);

    return result;
}


const uint8 *Settings::PointerToFirstData() const
{
    return (const uint8 *)this + 2 * sizeof(size);
}


void Settings::Load()
{
    if (!HAL_EEPROM::Load(this))
    {
        *this = NS_Settings::set_def;
    }
}


uint Settings::CalculateCRC32() const
{
    return HAL_CRC32::Calculate(PointerToFirstData(), SizeData());
}
