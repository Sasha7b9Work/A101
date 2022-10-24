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
        static char buffer[SIZE_BUFFER];
        static int pointer = 0;

        static void DecodeBuffer();
    }
}


void Display::Interface::CallbackOnReceive(char byte)
{
    if(byte == 0x1A)
    {
        return;
    }
    
    if((uint)byte == 0xFF)
    {
        return;
    }
    
    buffer[pointer++] = byte;

    if (pointer == SIZE_BUFFER)
    {
        pointer = 0;
    }

    if (byte == 'Z')
    {
        DecodeBuffer();
    }
}


void Display::Interface::DecodeBuffer()
{
    int button = buffer[pointer - 3] & 0x0F;
    int state = buffer[pointer - 2] & 0x0F;

    Button::ForIndex(button)->ApplyAction(state);



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
