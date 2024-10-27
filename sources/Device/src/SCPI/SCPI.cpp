// 2022/11/02 15:18:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Commands.h"
#include "Hardware/Communicator.h"
#include "Utils/String.h"
#include "SCPI/RingBuffer.h"
#include <cctype>


namespace SCPI
{
    void Send(Direction::E, pchar);

    void Error(Direction::E, pchar);

    // Входной буфер. Здесь находятся принимаемые символы
    static InBuffer in_usb(Direction::USB);
    static InBuffer in_rs232(Direction::RS232);

    static RingBuffer ring_usb;
    static RingBuffer ring_rs232;
}


void SCPI::Update()
{
    ring_usb.GetData(in_usb);
    in_usb.Update();

    ring_rs232.GetData(in_rs232);
    in_rs232.Update();
}


void SCPI::InBuffer::Update()
{
    bool run = true;

    while (run)
    {
        Command *command = ExtractCommand();

        run = command->Execute(dir);

        delete command;
    }
}


void SCPI::CallbackOnReceive(Direction::E dir, char byte)
{
    if (dir & Direction::USB)
    {
        ring_usb.Append((uint8)std::toupper(byte));
    }

    if (dir & Direction::RS232)
    {
        ring_rs232.Append((uint8)std::toupper(byte));
    }
}


SCPI::Command *SCPI::InBuffer::ExtractCommand()
{
    while (Size() && (buffer[0] == 0x0d || buffer[0] == 0x0a))
    {
        RemoveFirst(1);
    }

    for (int i = 0; i < Size(); i++)
    {
        if (buffer[i] == 0x0d || buffer[i] == 0x0a)
        {
            if (i == 0)
            {
                break;
            }
            else
            {
                char symbols[64];

                std::memcpy(symbols, buffer, (uint)i);
                symbols[i] = '\0';

                RemoveFirst(i);

                return ParseCommand(symbols);
            }
        }
    }

    return new CommandNull();
}


SCPI::Command *SCPI::InBuffer::ParseCommand(pchar symbols)
{
    char *data = (char *)symbols;

    if (std::strcmp(data, "*IDN?") == 0)                // *IDN?
    {
        return new CommandIDN();
    }

    if (std::strcmp(data, "MEAS?") == 0)                // MEAS?
    {
        return new CommandMEAS();
    }

    if (std::strcmp(data, "INFO?") == 0)                // INFO?
    {
        return new CommandINFO();
    }

    if (std::strlen(data) == 2)
    {
        if (data[0] == 'I')                             // I0...I5
        {
            int range = data[1] & 0x0F;

            if (range < 6)
            {
                return new CommandRangeI(range);
            }
        }
        else if (data[0] == 'J')                        // J0...J5
        {
            int range = data[1] & 0x0F;

            if (range < 6)
            {
                return new CommandRangeJ(range);
            }
        }
    }

    if (std::strlen(data) == 3)
    {
        if (data[0] == 'I' && data[1] == 'J')           // IJ0...IJ5
        {
            int range = data[2] & 0x0F;

            if (range < 6)
            {
                return new CommandRangeIJ(range);
            }
        }

        if (data[0] == 'Z')                             // ZI0, ZI1, ZJ0, ZJ1
        {
            if (data[1] == 'I' || data[1] == 'J')
            {
                if (data[2] == '0' || data[2] == '1')
                {
                    return new CommandZero(data[1], data[2] == '1');
                }
            }
        }
    }

    if (std::strlen(data) > std::strlen("UPGRADE"))     // UPGRADE VERSION_BUILD
    {
        if (std::memcmp(data, "UPGRADE", std::strlen("UPGRADE")))
        {
            char command[128];
            uint version_firm = 0;
            uint size_firm = 0;
            uint crc32_firm = 0;

            if (std::sscanf(data, "%s %u %u %u", command, &version_firm, &size_firm, &crc32_firm) == 4)
            {
                return new CommandUpgradeFirmware(version_firm, size_firm, crc32_firm);
            }
        }
    }

    SCPI::Error(dir, symbols);

    return new CommandNull();
}

String<> SCPI::InBuffer::FirstWord(pchar symbols)
{
    String<> result;

    int pos = 0;

    // Пропускаем первые пробелы

    int length = (int)std::strlen(symbols);

    while (pos < length && symbols[pos] == ' ')
    {
        pos++;
    }

    if (pos == length)
    {
        return result;
    }

    while (pos < length && symbols[pos] != ' ')
    {
        result.Append((char)symbols[pos]);
        pos++;
    }

    return result;
}


void SCPI::Send(Direction::E dir, pchar message)
{
    Communicator::SendWith0D0A(dir, message);
}


void SCPI::Error(Direction::E dir, pchar text)
{
    Communicator::Send(dir, "ERROR !!! Unknown sequence : ");
    Communicator::SendWith0D0A(dir, text);
}
