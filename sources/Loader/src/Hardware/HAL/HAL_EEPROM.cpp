// 2022/11/16 10:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "stm_includes.h"
#include <cstring>


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
    static uint NumberSector(uint address)
    {
        struct StructSector
        {
            uint number;
            uint startAddress;
        };

        static const StructSector sectors[] =
        {
            {FLASH_SECTOR_0,  ADDR_SECTOR_0},
            {FLASH_SECTOR_1,  ADDR_SECTOR_1},
            {FLASH_SECTOR_2,  ADDR_SECTOR_2},
            {FLASH_SECTOR_3,  ADDR_SECTOR_3},
            {FLASH_SECTOR_4,  ADDR_SECTOR_4},
            {FLASH_SECTOR_5,  ADDR_SECTOR_5},
            {FLASH_SECTOR_6,  ADDR_SECTOR_6},
            {FLASH_SECTOR_7,  ADDR_SECTOR_7},
            {FLASH_SECTOR_8,  ADDR_SECTOR_8},
            {FLASH_SECTOR_9,  ADDR_SECTOR_9},
            {FLASH_SECTOR_10, SECTOR_CALIBRATION},
            {FLASH_SECTOR_11, SECTOR_SETTINGS},
            {0, 0}
        };

        int i = 0;
        while (sectors[i].startAddress)
        {
            if (sectors[i].startAddress == address)
            {
                return sectors[i].number;
            }
            i++;
        }

        return (uint)(-1);
    }

    struct Sector
    {
        Sector(uint _address, uint _size, uint _size_place) : address(_address), size(_size), size_place(_size_place) {}
        // Возвращает первое свободное для записи место
        uint8 *FindEmptyPlace()
        {
            uint *pointer = (uint *)address;

            while ((uint)pointer < address + size)
            {
                if (*pointer == (uint)(-1))
                {
                    return (uint8 *)pointer;
                }

                pointer += size_place / sizeof(uint);
            }

            Erase();

            return (uint8 *)address;
        }
        // Возвращает последние сохранённые данные
        uint8 *FindLastSaved()
        {
            uint *pointer = (uint *)address;

            while ((uint)pointer < address + size)
            {
                if (*pointer == (uint)(-1))
                {
                    if (pointer == (uint *)address)
                    {
                        return nullptr;
                    }
                    else
                    {
                        return (uint8 *)(pointer - size_place / sizeof(uint));
                    }
                }

                pointer += size_place / sizeof(uint);
            }

            return nullptr;
        }
        void Erase()
        {
            CLEAR_FLASH_FLAGS;

            HAL_FLASH_Unlock();

            FLASH_EraseInitTypeDef is;

            is.TypeErase = TYPEERASE_SECTORS;
            is.Sector = NumberSector(address);
            is.NbSectors = 1;
            is.VoltageRange = VOLTAGE_RANGE_3;

            uint error = 0;

            HAL_FLASHEx_Erase(&is, &error);

            HAL_FLASH_Lock();
        }
        void Write(uint8 *place, void *data, uint num_bytes)
        {
            CLEAR_FLASH_FLAGS;

            HAL_FLASH_Unlock();

            for (uint i = 0; i < num_bytes; i++)
            {
                HAL_FLASH_Program(TYPEPROGRAM_BYTE, (uint)place++, ((uint8 *)data)[i]);
            }

            HAL_FLASH_Lock();
        }
        void Read(const uint8 *place, void *data, uint num_bytes)
        {
            std::memcpy(data, place, num_bytes);
        }
    private:
        const uint address;       // Адрес сектора
        const uint size;          // Разер сектора
        const uint size_place;    // Размер "места". Данные в сетора записывают по адресам, кратным этому значению
    };

    template<class T, uint addr>
    struct SectorSettings : public Sector
    {
        SectorSettings() : Sector(addr, 128 * 1024, 512) {}

        void Write(T *settings)
        {
            bool success = false;

            while(!success)
            {
                uint8 *place = FindEmptyPlace();

                Sector::Write(place, settings, sizeof(*settings));

                T loaded;

                Read(&loaded);

                success = loaded.IsEqual(settings);
            }
        }

        bool Read(T *settings)
        {
            uint8 *place = FindLastSaved();

            uint size_settings = *((uint *)place); //-V522

            if (size_settings != sizeof(T))
            {
                return false;
            }

            Sector::Read(place, settings, size_settings);

            uint crc32_settings = *((uint *)(place + sizeof(settings->size)));

            bool result = (crc32_settings == settings->CalculateCRC32());

            return result;
        }
    };
}


void HAL_EEPROM::EraseSector(uint address)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    FLASH_EraseInitTypeDef is;

    is.TypeErase = TYPEERASE_SECTORS;
    is.Sector = NumberSector(address);
    is.NbSectors = 1;
    is.VoltageRange = VOLTAGE_RANGE_3;

    uint error = 0;

    HAL_FLASHEx_Erase(&is, &error);

    HAL_FLASH_Lock();
}


void HAL_EEPROM::WriteData(uint address, const void *data, int size)
{
    CLEAR_FLASH_FLAGS;

    HAL_FLASH_Unlock();

    uint8 *place = (uint8 *)address;

    for (uint i = 0; i < (uint)size; i++)
    {
        HAL_FLASH_Program(TYPEPROGRAM_BYTE, (uint)place++, ((uint8 *)data)[i]);
    }

    HAL_FLASH_Lock();
}


uint HAL_EEPROM::ReadUint(uint address)
{
    uint *pointer = (uint *)address;

    return *pointer;
}


void HAL_EEPROM::ReadData(uint address, void *data, int size)
{
    std::memcpy(data, (const void *)address, (uint)size);
}
