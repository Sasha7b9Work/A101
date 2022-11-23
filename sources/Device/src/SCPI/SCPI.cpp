// 2022/11/02 15:18:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Parser/Parser.h"
#include "SCPI/Commands.h"
#include "Hardware/Communicator.h"
#include <cctype>


namespace SCPI
{
    class InBuffer : public Buffer2048<uint8>
    {
    public:
        Command ExtractCommand();
    };

    void Send(pchar);

    // Входной буфер. Здесь находятся принимаемые символы
    static InBuffer in;
}


void SCPI::Update()
{
    static int counter = 0;

    while (in.ExtractCommand().Execute())
    {
        counter++;
    }
}


void SCPI::CallbackOnReceive(uint8 byte)
{
    in.Append((uint8)std::toupper(byte));
}


SCPI::Command SCPI::InBuffer::ExtractCommand()
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
            break;
        }

        symbols.Append(buffer[i]);
    }

    RemoveFirst(symbols.Size());

    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    symbols.Append('\0');

    if (std::strcmp((char *)symbols.Data(), "*IDN?") == 0)
    {
        return CommandIDN();
    }

    return Command();
}


void SCPI::Send(pchar message)
{
    Communicator::SendWith0D0A(message);
}
