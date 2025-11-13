// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Utils/String.h"
#include "stm_includes.h"


namespace Device
{
    static void JumpToMainApplication();

    // ≈сть ли нова€ прошивка дл€ обновлени€
    static bool ExistFirmwareInROM();

    static void UpgradeFirmware();

    static const uint START_ADDRESS = ADDR_SECTOR_7;
}


void Device::Init()
{
    HAL::Init();
}


void Device::Update()
{
    if (ExistFirmwareInROM())
    {
        UpgradeFirmware();

        HAL_EEPROM::EraseSector(ADDR_SECTOR_7);
        HAL_EEPROM::EraseSector(ADDR_SECTOR_8);
    }

    JumpToMainApplication();
}


bool Device::ExistFirmwareInROM()
{
    for (uint address = ADDR_SECTOR_7; address < ADDR_SECTOR_7 + 32; address += 4)
    {
        if (HAL_EEPROM::ReadUint(address) != 0xFFFFFFFF)
        {
            return true;
        }
    }

    return false;
}


void Device::UpgradeFirmware()
{
    while (true)
    {
        uint size = HAL_EEPROM::ReadUint(START_ADDRESS);

        if (size > 128 * 1024 * 2)                          // Ѕольше двух секторов прошивка занимать не может
        {
            return;
        }

        uint crc32 = HAL_EEPROM::ReadUint(START_ADDRESS + 4);

        uint rd_crc32 = SU::CalculateCRC32((const void *)(START_ADDRESS + 8), (int)size);

        if (crc32 != rd_crc32)
        {
            return;
        }

        uint read_address = START_ADDRESS + 8;
        uint write_address = ADDR_SECTOR_5;

        static const int SIZE_BUFFER = 1024;
        uint8 buffer[SIZE_BUFFER];

        HAL_EEPROM::EraseSector(ADDR_SECTOR_5);
        HAL_EEPROM::EraseSector(ADDR_SECTOR_6);

        while ((int)size > 0)
        {
            HAL_EEPROM::ReadData(read_address, buffer, SIZE_BUFFER);
            HAL_EEPROM::WriteData(write_address, buffer, SIZE_BUFFER);
            size -= SIZE_BUFFER;
            read_address += SIZE_BUFFER;
            write_address += SIZE_BUFFER;
        }

        crc32 = HAL_EEPROM::ReadUint(START_ADDRESS + 4);

        rd_crc32 = SU::CalculateCRC32((const void *)(ADDR_SECTOR_5), (int)HAL_EEPROM::ReadUint(START_ADDRESS));

        if (crc32 == rd_crc32)
        {
            return;
        }
    }
}


void Device::JumpToMainApplication()
{
#define APP_ADDRESS 0x8020000

    typedef void (*pFunction)(void);
    pFunction jump_to_app;
    uint32_t jump_address;

    __disable_irq();

    SCB->VTOR = APP_ADDRESS;

    __set_MSP(*(__IO uint *)APP_ADDRESS);

    jump_address = *(__IO uint32_t *)(APP_ADDRESS + 4);
    jump_to_app = (pFunction)jump_address;
    __enable_irq();

    jump_to_app();
}
