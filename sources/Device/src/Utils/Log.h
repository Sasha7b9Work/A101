// 2022/10/27 08:33:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define LOG_WRITE(...)  Log::Write(__VA_ARGS__)


namespace Log
{
    void Write(const char *format, ...);
}
