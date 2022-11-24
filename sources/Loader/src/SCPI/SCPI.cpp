// 2022/11/02 15:18:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Commands.h"
#include "Hardware/Communicator.h"
#include "Utils/String.h"
#include <cctype>


namespace SCPI
{
    class InBuffer : public Buffer2048<uint8>
    {
    public:
        InBuffer(Direction::E _dir) : dir(_dir) {}
        void Update();
    private:
        Command *ParseCommand(Buffer<uint8, 1024> &);
        String<> FirstWord(Buffer<uint8, 1024> &);
        Command *ExtractCommand();
        const Direction::E dir;
    };

    void Send(Direction::E, pchar);

    void Error(Direction::E, pchar);

    // Входной буфер. Здесь находятся принимаемые символы
    static InBuffer in_usb(Direction::USB);
    static InBuffer in_rs232(Direction::RS232);
}


void SCPI::Update()
{
    in_usb.Update();
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


void SCPI::CallbackOnReceive(Direction::E dir, uint8 byte)
{
    if (dir & Direction::USB)
    {
        in_usb.Append((uint8)std::toupper(byte));
    }

    if (dir & Direction::RS232)
    {
        in_rs232.Append((uint8)std::toupper(byte));
    }
}


SCPI::Command *SCPI::InBuffer::ExtractCommand()
{
    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    Buffer<uint8, 1024> symbols;

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


SCPI::Command *SCPI::InBuffer::ParseCommand(Buffer<uint8, 1024> &symbols)
{
    if (std::strcmp((char *)symbols.Data(), "*IDN?") == 0)
    {
        return new CommandIDN();
    }

    if (std::strcmp((char *)symbols.Data(), "*RST") == 0)
    {
        return new CommandRST();
    }

    String<> first_word = FirstWord(symbols);

    if (first_word == "RANGE")
    {
        return new CommandRANGE((pchar)(symbols.Data() + first_word.Size()));
    }

    if (first_word == "DATA?")
    {
        return new CommandDATA((pchar)(symbols.Data() + first_word.Size()));
    }

    String<1024> message((char *)symbols.Data());

    SCPI::Error(dir, message.c_str());

    return new Command();
}

String<> SCPI::InBuffer::FirstWord(Buffer<uint8, 1024> &symbols)
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
