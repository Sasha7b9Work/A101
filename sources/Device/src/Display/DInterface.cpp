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


namespace Display
{
    namespace Interface
    {
        static const int SIZE_BUFFER = 10;
        static uint8 buffer[SIZE_BUFFER];
        static int pointer = 0;

        static void DecodeBuffer();
    }
}


void Display::Interface::CallbackOnReceive(uint8 byte)
{
    if (byte == 0xFF || byte == 'Z')
    {
        DecodeBuffer();
    }
    else
    {
        buffer[pointer++] = byte;
    }
}


void Display::Interface::DecodeBuffer()
{
    if(pointer == 0)
    {
        return;
    }
    
    uint8 byte = buffer[pointer - 1];
    
    if (byte >= '1' && byte <= '8')
    {
        int button = byte & 0x0F;

        Button::ForIndex(button)->Press();
    }

    pointer = 0;
}


void Display::Interface::SendCommand(pchar command)
{
    HAL_USART2::Send(command);

    HAL_USART2::Send("\xFF\xFF\xFF");
}


void Display::Interface::SendByte(uint8 byte)
{
    HAL_USART2::SendByte(byte);
}


void Display::Interface::SendCommandFormat(pchar format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommand(message);
}
