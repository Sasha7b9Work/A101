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
    if ((size % 4) == 0)
    {
        return HAL_CRC_Calculate(&handleCRC, (uint *)data, size / 4);
    }

    uint size_buffer = size;

    while (size_buffer % 4)
    {
        ++size_buffer;
    }

    uint result = 0;

    uint *buffer = new uint[size_buffer];

    std::memcpy(buffer, data, size);

    for (uint i = size; i < size_buffer; i++)
    {
        buffer[i] = 0;
    }

    result = HAL_CRC_Calculate(&handleCRC, buffer, size_buffer / 4);

    delete [] buffer;

    return result;
}
