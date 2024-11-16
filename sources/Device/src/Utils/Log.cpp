// 2022/10/27 08:32:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Log.h"
#include "Hardware/HAL/HAL.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


namespace Log
{
    static int counter = 1;
}


void Log::Write(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    char line[300];

    sprintf(line, "%d : %s\x0d\x0a", counter++, message);

    HAL_USART3::SendText(line);
    HAL_UART4::SendText(line);
}


void Log::Write(const char *file, int line, const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    char buffer[300];

    sprintf(buffer, "%d : %s          %s:%d\x0d\x0a", counter++, message, file, line);

    HAL_USART3::SendText(buffer);
    HAL_UART4::SendText(buffer);
}
