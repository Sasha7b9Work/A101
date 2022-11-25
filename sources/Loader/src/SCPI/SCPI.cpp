// 2022/11/02 15:18:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Commands.h"
#include "Utils/String.h"
#include "Hardware/HAL/HAL.h"
#include "Updater.h"
#include "Device.h"
#include <cctype>


namespace SCPI
{
    class InBuffer : public Buffer2048<uint8>
    {
    public:
        InBuffer() = default;
        void Update();
    private:
        Command *ParseCommand(Buffer<uint8, 1024> &);
        String<> FirstWord(const Buffer<uint8, 1024> &);
        Command *ExtractCommand();
    };

    void Send(pchar);

    // Входной буфер. Здесь находятся принимаемые символы
    static InBuffer in_usb;
}


void SCPI::Update()
{
    in_usb.Update();
}


void SCPI::InBuffer::Update()
{
    bool run = true;

    while (run)
    {
        Command *command = ExtractCommand();

        run = command->Execute();

        delete command;
    }
}


void SCPI::CallbackOnReceive(uint8 byte)
{
    if (Updater::BytesLeft())
    {
        Updater::AppendByte(byte);
    }
    else
    {
        in_usb.Append((uint8)std::toupper(byte));
    }
}


void SCPI::OnEvent::WriteBuffer()
{
    if (Updater::BytesLeft())
    {
        Send("?");
    }
    else
    {
        State::Set(State::Completed);
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
    if (std::strcmp((char *)symbols.Data(), "A101?") == 0)
    {
        return new CommandREQUEST();
    }

    String<> first_word = FirstWord(symbols);

    if (first_word == "SIZE")
    {
        return new CommandSIZE((pchar)(symbols.Data() + first_word.Size()));
    }

    return new Command();
}

String<> SCPI::InBuffer::FirstWord(const Buffer<uint8, 1024> &symbols)
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


void SCPI::Send(pchar message)
{
    HAL_USART3::SendTextWith0D(message);
}
