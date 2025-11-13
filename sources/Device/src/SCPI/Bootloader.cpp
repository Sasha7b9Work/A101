// 2024/10/27 15:32:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "defines.cpp"
#include "SCPI/Bootloader.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/StringUtils.h"
#include "Nextion/Display.h"
#include "Nextion/Nextion.h"
#include "SCPI/SCPI.h"
#include "Hardware/Timer.h"
#include <cstdio>
#include <stm32f4xx_hal.h>


namespace Bootloader
{
    static bool in_progress = false;

    static uint version = 0;
    static uint size = 0;           // Столько данных запрошено
    static uint crc32 = 0;

    static const int MAX_SIZE_CHAIN = 1024;
    static char chain[MAX_SIZE_CHAIN];
    static int size_chain = 0;

    static uint address_write = ADDR_SECTOR_7;      // Сюда будем писать следующие данные

    // Запрос новой порции. Если false - все данные приняты
    static bool RequestNewChain();

    // Записать новую порцию данных в память
    static void WriteChainToMemory();

    static void PrepareROM();

    static void WriteToROM(const void *buffer, int size);

    static void SendCommand(pchar);

    static void DrawString(int x, int y, pchar);

    static TimeMeterMS meter;

    static void DisplayCompleted();

    static void DisplayError();
}


void Bootloader::Run(uint num_ver, uint size_ver, uint crc32_ver)
{
    version = num_ver;
    size = size_ver;
    crc32 = crc32_ver;

    in_progress = true;

    DisplayFunc();

    PrepareROM();

    WriteToROM(&size, 4);
    WriteToROM(&crc32, 4);

    RequestNewChain();

    meter.Reset();
}


void Bootloader::OnReceiveByte(char symbol)
{
    if (size_chain < MAX_SIZE_CHAIN)
    {
        chain[size_chain++] = symbol;
    }
}


bool Bootloader::InProgress()
{
    return in_progress;
}


bool Bootloader::RequestNewChain()
{
    size_chain = 0;

    if (size == 0)
    {
        return false;
    }

    char message[64];

    std::sprintf(message, "UPGRADE %d", (size >= MAX_SIZE_CHAIN) ? MAX_SIZE_CHAIN : size);

    SendCommand(message);

    return true;
}


void Bootloader::Update()
{
    if (size_chain == MAX_SIZE_CHAIN ||     // Если заполнен весь приёмный буфер
        size_chain == (int)size)            // Или приняты все байты
    {
        size -= size_chain;

        WriteChainToMemory();               // Записываем следующую порцию в ПЗУ

        if (!RequestNewChain())
        {
            uint *pointer = (uint *)ADDR_SECTOR_7;
            uint rd_size = *pointer++;
            uint rd_crc32 = *pointer++;

            uint calc_crc32 = SU::CalculateCRC32(pointer, (int)rd_size);

            if (calc_crc32 == rd_crc32)
            {
                in_progress = false;

                DisplayCompleted();

                HAL_NVIC_SystemReset();
            }
            else
            {
                in_progress = false;
                SendCommand("UPGRADE RESET");
            }
        }
    }
    else
    {
        if (meter.ElapsedTime() > 30000)        // Долго нет новых данных - обрыв связи
        {
            DisplayError();

            meter.Wait(3000);

            HAL_NVIC_SystemReset();
        }
    }
}


void Bootloader::SendCommand(pchar command)
{
    SCPI::Send(Direction::USB, command);
    SCPI::Send(Direction::RS232, command);
}


void Bootloader::WriteChainToMemory()
{
    WriteToROM(chain, MAX_SIZE_CHAIN);
}


void Bootloader::PrepareROM()
{
    HAL_EEPROM::EraseSector(ADDR_SECTOR_7);
    HAL_EEPROM::EraseSector(ADDR_SECTOR_8);

    address_write = ADDR_SECTOR_7;
}


void Bootloader::WriteToROM(const void *buffer, int sz)
{
    HAL_EEPROM::WriteData(address_write, buffer, sz);

    address_write += (uint)sz;
}


void Bootloader::DisplayFunc()
{
    Display::Clear();

    char message[128];

    int y = 10;
    int dy = 70;

    std::sprintf(message, "Версия : %d", version);
    DrawString(10, y, message);
    y += dy;

    uint *pointer = (uint *)ADDR_SECTOR_7;
    uint full_size = *pointer;

    uint received = address_write - ADDR_SECTOR_7 - 8;

    std::sprintf(message, "Принято : %u/%d Kbytes, %u %%", received / 1024, full_size / 1024, received * 100 / full_size);
    DrawString(10, y, message);
    y += dy;

    std::sprintf(message, "Времени прошло : %u s", meter.ElapsedTime() / 1000);
    DrawString(10, y, message);
    y += dy;
}


void Bootloader::DrawString(int x, int y, pchar message)
{
    Nextion::DrawString({ x, y, 700, 50 }, 0, Color::White, Color::Background, message);
}


void Bootloader::DisplayCompleted()
{
    Display::Clear();

    DrawString(10, 100, "Прошивка загружена.");
    DrawString(10, 160, "Перезагрузка.");
}


void Bootloader::DisplayError()
{
    Display::Clear();

    DrawString(10, 100, "При обновлении произошла ошибка.");
    DrawString(10, 160, "Перезагрузка.");
}
