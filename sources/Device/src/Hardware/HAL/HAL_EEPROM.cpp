// 2022/11/16 10:05:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "stm_includes.h"
#include <cstring>


#define ADDR_SECTOR_0   ((uint)0x08000000)  //  FLASH_SECTOR_0  16k Основная прошивка
#define ADDR_SECTOR_1   ((uint)0x08004000)  //  FLASH_SECTOR_1  16k
#define ADDR_SECTOR_2   ((uint)0x08008000)  //  FLASH_SECTOR_2  16k
#define ADDR_SECTOR_3   ((uint)0x0800c000)  //  FLASH_SECTOR_3  16k
#define ADDR_SECTOR_4   ((uint)0x08010000)  //  FLASH_SECTOR_4  64k
#define ADDR_SECTOR_5   ((uint)0x08020000)  //  FLASH_SECTOR_5  128k
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
            LOG_WRITE("Erase sector %X", address);

            CLEAR_FLASH_FLAGS;

            HAL_FLASH_Unlock();

            FLASH_EraseInitTypeDef is;

            is.TypeErase = TYPEERASE_SECTORS;
            is.Sector = NumberSector();
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
        uint NumberSector()
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
    };

    template<class T, uint addr>
    struct SectorSettings : public Sector
    {
        SectorSettings() : Sector(addr, 128 * 1024, 128) {}

        void Write(T *settings)
        {
            bool success = false;

            while(!success)
            {
                uint8 *place = FindEmptyPlace();

                Sector::Write(place, settings, sizeof(*settings));

                LOG_WRITE("Saving settings to %x", place);

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

    static SectorSettings<Settings, SECTOR_SETTINGS> sect_set;
    static SectorSettings<CalibrationSettings, SECTOR_CALIBRATION> sect_cal;
}


void HAL_EEPROM::Save(CalibrationSettings *settings)
{
    settings->size = sizeof(*settings);
    settings->crc32 = settings->CalculateCRC32();

    sect_cal.Write(settings);
}


bool HAL_EEPROM::Load(CalibrationSettings *settings)
{
    return sect_cal.Read(settings);
}


void HAL_EEPROM::Save(Settings *settings)
{
    sect_set.Write(settings);
}

bool HAL_EEPROM::Load(Settings *settings)
{
    return sect_set.Read(settings);
}
