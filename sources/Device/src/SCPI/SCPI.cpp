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


void SCPI::AppendNewData(Direction::E dir, char *data, uint size)
{
    for (uint i = 0; i < size; i++)
    {
        CallbackOnReceive(dir, (uint8)*data++);
    }
}


void SCPI::CallbackOnReceive(Direction::E dir, uint8 byte)
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
    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    Buffer<1024> symbols;

    for (int i = 0; i < Size(); i++)
    {
        if (buffer[i] == 0x0a || buffer[i] == 0x0d)
        {
            RemoveFirst(symbols.Size());

            while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
            {
                RemoveFirst(1);
            }

            symbols.Append('\0');

            return ParseCommand(symbols);
        }

        symbols.Append(buffer[i]);
    }

    return new Command();
}


SCPI::Command *SCPI::InBuffer::ParseCommand(Buffer<1024> &symbols)
{
    char *data = (char *)symbols.Data();

    if (std::strcmp(data, "*IDN?") == 0)
    {
        return new CommandIDN();
    }

    if (std::strcmp(data, "MEAS?") == 0)
    {
        return new CommandMEAS();
    }

    if (std::strlen(data) == 2)
    {
        if (data[0] == 'I')                             // I0...I5
        {
            int range = data[1] & 0x0F;

            if (range >= 0 && range < 6)
            {
                return new CommandRangeI(range);
            }
        }
        else if (data[0] == 'J')                        // J0...J5
        {
            int range = data[1] & 0x0F;

            if (range >= 0 && range < 6)
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

            if (range >= 0 && range < 6)
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

    String<1024> message((char *)symbols.Data());

    SCPI::Error(dir, message.c_str());

    return new Command();
}

String<> SCPI::InBuffer::FirstWord(Buffer<1024> &symbols)
{
    String<> result;

    int pos = 0;

    // Пропускаем первые пробелы

    while (pos < symbols.Size() && symbols[pos] == ' ')
    {
        pos++;
    }

    if (pos == symbols.Size())
    {
        return result;
    }

    while (pos < symbols.Size() && symbols[pos] != ' ')
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
