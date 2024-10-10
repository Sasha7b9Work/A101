// 2022/10/27 08:33:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"

#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif


#ifdef LOGGED
    #define LOG_WRITE(...)       Log::Write(__VA_ARGS__)
    #define LOG_ERROR(...)       Log::Write(__VA_ARGS__)
    #define LOG_WRITE_TRACE(...) Log::Write(__FILE__, __LINE__, __VA_ARGS__)
    #define LOG_TRACE()          LOG_WRITE_TRACE("")
    #define LOG_ERROR_TRACE(...) Log::Write(__FILE__, __LINE__, __VA_ARGS__)
#else
    #define LOG_WRITE(...)
    #define LOG_ERROR(...)
    #define LOG_WRITE_TRACE(...)
    #define LOG_TRACE()
    #define LOG_ERROR_TRACE(...)
#endif

#ifndef WIN32
#pragma clang diagnostic pop
#endif


namespace Log
{
    void Write(const char *file, int line, const char *format, ...);
    void Write(const char *format, ...);
}
