// 2022/10/27 08:33:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#define LOG_WRITE(...)  Log::Write(__VA_ARGS__)

#ifndef WIN32
#pragma clang diagnostic pop
#endif


namespace Log
{
    void Write(const char *format, ...);
}
