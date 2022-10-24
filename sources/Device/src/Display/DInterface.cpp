// 2022/10/17 15:30:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/HAL/HAL.h"
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
//    int state = buffer[pointer - 2] & 0x0F;

    if (button >= 0x01 && button <= 0x06)
    {
        static int states[6][7] =
        {
            {1, 0, 0, 0, 1, 1, 0},      // 2mA
            {1, 0, 1, 1, 0, 1, 0},      // 20mA
            {1, 1, 0, 1, 1, 0, 0},      // 200mA
            {0, 0, 0, 1, 0, 0, 0},      // 2A
            {1, 0, 1, 1, 0, 1, 0},      // 20A
            {1, 1, 0, 1, 1, 0, 0}       // 50A
        };

        int range = button - 1;

        HAL_PIO::Write(PIN_US1, states[range][0] == 1); //-V525
        HAL_PIO::Write(PIN_US2, states[range][1] == 1);
        HAL_PIO::Write(PIN_US3, states[range][2] == 1);
        HAL_PIO::Write(PIN_US4, states[range][3] == 1);
        HAL_PIO::Write(PIN_US6, states[range][4] == 1);
        HAL_PIO::Write(PIN_US7, states[range][5] == 1);
        HAL_PIO::Write(PIN_US8, states[range][6] == 1);
    }

    pointer = 0;
}


void Display::Interface::SendCommand(pchar command)
{
    HAL_USART2::Send(command);

    HAL_USART2::Send("\xFF\xFF\xFF");
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
