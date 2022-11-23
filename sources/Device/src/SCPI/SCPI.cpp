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
    static InBuffer buffer;
}


void SCPI::Update()
{
    static int counter = 0;

    while (buffer.ExtractCommand().Execute())
    {
        counter++;
    }
}


void SCPI::CallbackOnReceive(uint8 byte)
{
    buffer.Append(byte);
}


SCPI::Command SCPI::InBuffer::ExtractCommand()
{
    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    Command command;

    for (int i = 0; i < Size(); i++)
    {
        if (buffer[0] == 0x0a || buffer[0] == 0x0d)
        {
            break;
        }

        command.Append(buffer[i]);
    }

    RemoveFirst(command.Size());

    while (Size() && (buffer[0] == 0x0a || buffer[0] == 0x0d))
    {
        RemoveFirst(1);
    }

    return command;
}


bool SCPI::Command::Execute()
{
    if (Size() == 0)
    {
        return false;
    }

    if(IsEquals(""))
    {
        return false;
    }

    return true;
}
