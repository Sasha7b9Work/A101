// 2022/10/27 08:33:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

#define LOG_WRITE(...)  Log::Write(__VA_ARGS__)
#define LOG_TRACE()     Log::Write(__FILE__, __LINE__, "")
#define LOG_WRITE_TRACE(...) Log::Write(__FILE__, __LINE__, __VA_ARGS__)

#ifndef WIN32
#pragma clang diagnostic pop
#endif


namespace Log
{
    void Write(const char *file, int line, const char *format, ...);
    void Write(const char *format, ...);
}
