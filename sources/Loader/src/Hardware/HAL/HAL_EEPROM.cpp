// 2022/11/16 10:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "stm_includes.h"
#include "SCPI/SCPI.h"
#include <cstring>


#define ADDR_SECTOR_0   ((uint)0x08000000)  //  FLASH_SECTOR_0  16k  Загрузчик
#define ADDR_SECTOR_1   ((uint)0x08004000)  //  FLASH_SECTOR_1  16k
#define ADDR_SECTOR_2   ((uint)0x08008000)  //  FLASH_SECTOR_2  16k
#define ADDR_SECTOR_3   ((uint)0x0800c000)  //  FLASH_SECTOR_3  16k
#define ADDR_SECTOR_4   ((uint)0x08010000)  //  FLASH_SECTOR_4  64k
#define ADDR_SECTOR_5   ((uint)0x08020000)  //  FLASH_SECTOR_5  128k Основная прошивка
#define ADDR_SECTOR_6   ((uint)0x08040000)  //  FLASH_SECTOR_6  128k
#define ADDR_SECTOR_7   ((uint)0x08060000)  //  FLASH_SECTOR_7  128k
#define ADDR_SECTOR_8   ((uint)0x08080000)  //  FLASH_SECTOR_8  128k
#define ADDR_SECTOR_9   ((uint)0x080a0000)  //  FLASH_SECTOR_9  128k Обновлялка

#define ADDR_SECTOR_10  ((uint)0x080c0000)  //  FLASH_SECTOR_10 128k Калибровки
#define SECTOR_CALIBRATION  ADDR_SECTOR_10

#define ADDR_SECTOR_11  ((uint)0x080e0000)  //  FLASH_SECTOR_11 128k Настройки
#define SECTOR_SETTINGS     ADDR_SECTOR_11

#define CLEAR_FLASH_FLAGS \
    __HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP    |  /* end of operation flag              */   \
                           FLASH_FLAG_OPERR  |  /* operation error flag               */   \
                           FLASH_FLAG_WRPERR |  /* write protected error flag         */   \
                           FLASH_FLAG_PGAERR |  /* programming alignment error flag   */   \
                           FLASH_FLAG_PGPERR |  /* programming parallelism error flag */   \
                           FLASH_FLAG_PGSERR)   /* programming sequence error flag    */


namespace HAL_EEPROM
{
    // Осталось байт для записи
    static int elapsed_bytes = 0;

    static uint address = 0x08020000;
}

void HAL_EEPROM::Erase(int size)
{
    elapsed_bytes = size;

    address = 0x08020000;

    int num_sectors = size / (1024 * 128) + 1;

    if (num_sectors > 4)
    {
        num_sectors = 4;
    }

    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef is;

    is.TypeErase = TYPEERASE_SECTORS;
    is.Sector = 5;
    is.NbSectors = (uint)num_sectors;
    is.VoltageRange = VOLTAGE_RANGE_3;

    uint error = 0;

    HAL_FLASHEx_Erase(&is, &error);

    HAL_FLASH_Lock();
}


void HAL_EEPROM::Write(uint8 *buffer, int size)
{
    if (elapsed_bytes <= 0)
    {
        return;
    }

    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    for (uint i = 0; i < (uint)size; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, address++, buffer[i]);
    }

    HAL_FLASH_Lock();

    elapsed_bytes -= size;

    SCPI::OnEvent::WriteBuffer();
}
