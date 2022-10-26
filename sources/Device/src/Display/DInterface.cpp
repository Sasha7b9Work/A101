// 2022/10/17 15:30:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstring>


/*
*  +--------+---------------+
*  | кнопка | нажата/отжата |
*  +--------+---------------+
*  |   1    | "11Z" / "10Z" |
*  |   2    | "21Z" / "20Z" |
*  |   3    | "31Z" / "30Z" |
*  |   4    | "41Z" / "40Z" |
*  |   5    | "51Z" / "50Z" |
*  |   6    | "61Z" / "60Z" |
*  |  Меню  | "71Z" / "70Z" |
*  +--------+---------------+
*/


namespace DInterface
{
    struct CommandUART
    {
        CommandUART() : size(0) {}
        CommandUART(uint8 *bytes, int _size) : size(_size)
        {
            std::memcpy(buffer, bytes, (uint)size);
        }
        virtual ~CommandUART() {}
        bool IsEmpty() const { return (size == 0); }
        virtual bool Execute()
        {
            return false;
        };
    protected:
        static const int SIZE = 16;
        uint8 buffer[SIZE];
        int size;
    };

    struct CommandUART_Z : public CommandUART
    {
        CommandUART_Z(uint8 *bytes, int size) : CommandUART(bytes, size) {}

        virtual bool Execute() override
        {
            if (IsEmpty())
            {
                return false;
            }

            if (size == 2)
            {
                uint8 byte1 = buffer[0];

                if (byte1 >= '1' && byte1 <= '9')
                {
                    int button = byte1 & 0x0F;

                    int state = buffer[1] & 0x0F;

                    Button::ForIndex(button)->ToState(state);
                    
                    return false;
                }
            }

            return false;
        }
    };

    struct CommandUART_FF : public CommandUART
    {
        CommandUART_FF(uint8 *bytes, int size) : CommandUART(bytes, size) {}

        virtual bool Execute() override
        {
            if (IsEmpty())
            {
                return false;
            }
            
            return false;
        }
    };


    template<uint size>
    struct BufferUART
    {
        BufferUART() : pointer(0) {}
        void Push(uint8 byte)
        {
            if (pointer == size)
            {
                RemoveFromStart(1);
            }

            buffer[pointer++] = byte;
        }
        CommandUART *GetCommand()
        {
            for (int i = 0; i < pointer; i++)
            {
                if (buffer[i] == (uint8)'Z')
                {
                    CommandUART_Z *result = new CommandUART_Z(buffer, i);

                    RemoveFromStart(i + 1);

                    return result;
                }
            }

            if (pointer > 3)
            {
                for (int i = 0; i < pointer - 3; i++)
                {
                    if (std::memcmp(&buffer[i], "\xFF\xFF\xFF", 3) == 0)
                    {

                    }
                }
            }

            return new CommandUART();
        }
    private:
        uint8 buffer[size];
        int pointer;
        void RemoveFromStart(int num_bytes)
        {
            if (num_bytes == pointer)
            {
                pointer = 0;
            }
            else
            {
                std::memmove(buffer, buffer + num_bytes, (uint)(pointer - num_bytes));
                pointer -= num_bytes;
            }
        }
    };

    static BufferUART <32>buffer;

    static ReturnCodeDI::E last_code = ReturnCodeDI::InstructionSuccessful;
}


void DInterface::Update()
{
    bool run = true;

    while (run)
    {
        CommandUART *command = buffer.GetCommand();

        run = command->Execute();

        delete command;
    }
}


ReturnCodeDI::E DInterface::LastCode()
{
    return last_code;
}


void DInterface::CallbackOnReceive(uint8 byte)
{
    buffer.Push(byte);
}


void DInterface::SendCommand(pchar command)
{
    last_code = ReturnCodeDI::_None;

    HAL_USART2::Send(command);

    HAL_USART2::Send("\xFF\xFF\xFF");
}


void DInterface::SendByte(uint8 byte)
{
    last_code = ReturnCodeDI::_None;

    HAL_USART2::SendByte(byte);
}


void DInterface::SendCommandFormat(pchar format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommand(message);
}
