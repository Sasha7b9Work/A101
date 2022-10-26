// 2022/10/17 15:30:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include <cstdarg>
#include <cstdio>
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
    template<uint size>
    struct BufferUART
    {
        BufferUART() : pointer(0) {}
        void Push(uint8 byte)
        {
            if (pointer == size)
            {
                pointer = 0;
            }

            buffer[pointer++] = byte;
        }
    private:
        uint8 buffer[size];
        int pointer;
    };

    static BufferUART <32>buffer;

    static ReturnCodeDI::E last_code = ReturnCodeDI::InstructionSuccessful;
}


void DInterface::Update()
{

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
