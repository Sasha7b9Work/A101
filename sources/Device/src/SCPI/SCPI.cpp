// 2022/11/02 15:18:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Parser/Parser.h"
#include "SCPI/Commands.h"


namespace SCPI
{
    class InBuffer : public Buffer2048<uint8>
    {
    public:
        Command ExtractCommand();
    };


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
    in.Append(byte);
}


SCPI::Command SCPI::InBuffer::ExtractCommand()
{
    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    Buffer<uint8, 1024> data;

    for (int i = 0; i < Size(); i++)
    {
        if (buffer[0] == 0x0a || buffer[0] == 0x0d)
        {
            break;
        }

        data.Append(buffer[i]);
    }

    RemoveFirst(data.Size());

    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    return Command();
}


bool SCPI::Command::Execute()
{
    return false;
}
