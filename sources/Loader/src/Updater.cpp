// 2022/11/25 12:06:47 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Updater.h"
#include "Hardware/HAL/HAL.h"
#include "SCPI/SCPI.h"


namespace Updater
{
    static int bytes_left = 0;

    static uint8 buffer[1024];
    static int pointer = 0;
}


void Updater::SetSize(int size)
{
    bytes_left = size;
    pointer = 0;

    HAL_EEPROM::Erase(size);
}


int Updater::BytesLeft()
{
    return bytes_left;
}


void Updater::AppendByte(uint8 byte)
{
    buffer[pointer++] = byte;

    bytes_left--;

    if (pointer == 1024 || bytes_left == 0)
    {
        HAL_EEPROM::Write(buffer, pointer);
    }

    pointer = 0;
}
