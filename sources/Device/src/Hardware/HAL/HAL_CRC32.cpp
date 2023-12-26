// 2022/11/22 09:29:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "stm_includes.h"
#include <cstdlib>
#include <cstring>


namespace HAL_CRC32
{
    static CRC_HandleTypeDef handleCRC =
    {
        CRC,
        HAL_UNLOCKED,
        HAL_CRC_STATE_RESET
    };
}


void HAL_CRC32::Init()
{
    HAL_CRC_Init(&handleCRC);
}


uint HAL_CRC32::Calculate(const void *data, uint size)
{
    return HAL_CRC_Calculate(&handleCRC, (uint *)data, size / 4);          // \todo Предусмотреть ситуацию, когда size не кратно 4
}
