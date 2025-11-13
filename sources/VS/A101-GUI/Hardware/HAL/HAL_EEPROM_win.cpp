// 2022/11/22 09:04:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"


void HAL_EEPROM::Save(CalibrationSettings *)
{

}


bool HAL_EEPROM::Load(CalibrationSettings *)
{
    return false;
}


void HAL_EEPROM::Save(Settings *)
{

}


bool HAL_EEPROM::Load(Settings *)
{
    return false;
}


void HAL_EEPROM::EraseSector(uint /*address*/)
{

}


void HAL_EEPROM::WriteData(uint /*address*/, const void */*data*/, int /*size*/)
{

}
