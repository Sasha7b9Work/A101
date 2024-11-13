// 2022/11/14 13:39:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"
#include <cstring>


namespace NS_Settings
{
    static Settings set_def
    {
        sizeof(Settings),
        0,
        false,      // middle_of_3
        false,      // smooth
        false,      // firLPF

        true,       // en_DC
        true,       // en_AC
        { TypeMeasure::Count, TypeMeasure::Count, TypeMeasure::Count },
        MeasuresOnDisplay::AC_DC,
        Baudrate::_9600,
        Parity::No,
        StopBits::_1,
        ChangeRange::Hand,
        TypeGraph::Raw,
        100,        // brightness

        0U,         // serial number
        Lang::RU
    };

    static Settings stored = set_def;
}

Settings set = NS_Settings::set_def;


void Settings::Storage::Store(const Settings &settings)
{
    NS_Settings::stored = settings;
}


void Settings::Storage::Restore(Settings &settings)
{
    settings = NS_Settings::stored;
}


void Settings::Save()
{
    size = sizeof(*this);
    crc32 = CalculateCRC32();

    Settings loaded;

    HAL_EEPROM::Load(&loaded);

    if (!loaded.IsEqual(this))
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
    return sizeof(*this) - 2 * sizeof(size);
}


const uint8 *Settings::PointerToFirstData() const
{
    return (const uint8 *)this + 2 * sizeof(size);
}


void Settings::Load()
{
    if (!HAL_EEPROM::Load(this))
    {
        uint sn = set.serial_number;

        *this = NS_Settings::set_def;

        this->serial_number = sn;
    }
}


uint Settings::CalculateCRC32() const
{
    return Math::CalculateCRC32(PointerToFirstData(), (int)SizeData());
}


bool TypeMeasure::IsShown() const
{
    for (int i = 0; i < 3; i++)
    {
        if (set.en_add_meas[i] == value)
        {
            return true;
        }
    }

    return false;
}


uint Baudrate::ToRaw() const
{
    static const uint raws[Count] =
    {
        9600,
        115200
    };

    return raws[current];
}


bool Lang::IsRU()
{
    return set.lang == RU;
}


void TypeMeasure::GetUnitsForFrequnesy(REAL freq, char ru[32], char en[32])
{
    if (freq < 1e-3f)
    {
        std::strcpy(ru, "ìêÃö");
        std::strcpy(en, "uHz");
    }
    else if (freq < 1.0f)
    {
        std::strcpy(ru, "ìÃö");
        std::strcpy(en, "mHz");
    }
    else if (freq < 1e3f)
    {
        std::strcpy(ru, "Ãö");
        std::strcpy(en, "Hz");
    }
    else if (freq < 1e6f)
    {
        std::strcpy(ru, "êÃö");
        std::strcpy(en, "kHz");
    }
    else if (freq < 1e9f)
    {
        std::strcpy(ru, "ÌÃö");
        std::strcpy(en, "MHz");
    }
    else
    {
        std::strcpy(ru, "ÃÃö");
        std::strcpy(en, "GHz");
    }
}


int TypeMeasure::GetNumDigitsAfterComma(REAL freq)
{
    /*
    *   < 1m         2      10**-3
    *   < 10m        4      10**-2
    *   < 100m       3      10**-1
    *   < 1          2      10**0
    *   < 10         4      10**1  [1...10)
    *   < 100        3      10**2  [10...100)
    *   < 1k         2      10**3  [100...1k)
    *   < 10k        4      10**4
    *   < 100k       3      10**5
    *   < 1M         2      10**6
    *   < 10Ì        4      10**7
    *   < 100Ì       3      10**8
    *   < 1Ã         2      10**9
    */

    if (freq < 1e-3)        return 0;
    else if (freq < 1e-2)   return 2;
    else if (freq < 1e-1)   return 1;
    else if (freq < 1e0)    return 0;
    else if (freq < 1e1)    return 2;
    else if (freq < 1e2)    return 1;
    else if (freq < 1e3)    return 0;
    else if (freq < 1e4)    return 2;
    else if (freq < 1e5)    return 1;
    else if (freq < 1e6)    return 0;
    else if (freq < 1e7)    return 2;
    else if (freq < 1e8)    return 1;
    else if (freq < 1e9)    return 0;

    return 2;
}
