// 2022/03/30 08:25:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Display/Colors.h"
#include <cstdarg>
#include <cstring>
#include <cstdio>
#include <cstdlib>


#define DEFAULT_SIZE_STRING 64

template<int capacity = DEFAULT_SIZE_STRING>
class String
{
public:

    String() { Clear(); }

    explicit String(pchar format, ...)
    {
        std::va_list args;
        va_start(args, format);
        int num_symbols = std::vsprintf(buffer, format, args);
        va_end(args);

        if (num_symbols < 0 || num_symbols > capacity - 1)
        {
            LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, num_symbols);
        }
    }

    void SetFormat(pchar format, ...)
    {
        std::va_list args;
        va_start(args, format);
        int num_symbols = std::vsprintf(buffer, format, args);
        va_end(args);

        if (num_symbols < 0 || num_symbols > capacity - 1)
        {
            LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, num_symbols);
        }
    }

    char *c_str() const { return (char *)&buffer[0]; }

    int Size() const { return (int)std::strlen(buffer); }

    void Append(pchar str)
    {
        int need_size = Size() + (int)std::strlen(str) + 1;

        if (need_size > capacity)
        {
            LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, need_size);

            int pointer = Size();

            while (pointer < capacity)
            {
                Append(str[pointer++]);
            }

            buffer[capacity - 1] = '\0';

            LOG_WRITE(buffer);
        }
        else
        {
            std::strcat(buffer, str);
        }
    }

    void Clear() { buffer[0] = '\0'; }

    void Append(char symbol)
    {
        if (!Filled())
        {
            int pos = (int)std::strlen(buffer);
            buffer[pos] = symbol;
            buffer[pos + 1] = '\0';
        }
        else
        {
            LOG_ERROR_TRACE("buffer is full");
        }
    }

    void Append(const String<capacity> &rhs) { Append(rhs.c_str()); }

    // Добавить байты. В конец дописать ноль
    void AppendBytes(const void *bytes, int num_bytes)
    {
        int need_size = Size() + num_bytes + 1;

        if (need_size > capacity)
        {
            LOG_ERROR_TRACE("Very small string buffer %d, need %d:", capacity, need_size);
            LOG_WRITE(c_str());
        }
        else
        {
            std::memcpy(buffer + Size(), bytes, (uint)num_bytes);
            buffer[need_size - 1] = '\0';
        }
    }

    bool Filled() const { return (Size() == capacity - 1); }

    String<capacity> GetWord(const int num_word)
    {
        String<capacity> result;

        int pos = 0;

        int current_word = 0;

        while (pos < Size())
        {
            // Выбираем пробелы
            while (pos < Size() && buffer[pos] == ' ')
            {
                pos++;
            }

            // Теперь выбираем символы
            while (pos < Size() && buffer[pos] != ' ')
            {
                result.Append(buffer[pos]);
                pos++;
            }

            if (current_word == num_word)
            {
                return result;
            }

            result.Clear();

            current_word++;
        }

        return result;
    }

    char &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static char null_char = 0;

        return null_char;
    }

    bool operator==(pchar string)
    {
        return std::strcmp(buffer, string) == 0;
    }

    bool ToInt(int *out)
    {
        char *str = buffer;

        int sign = str[0] == '-' ? -1 : 1;

        if (str[0] < '0' || str[0] > '9')
        {
            ++str;
        }

        int length = (int)(std::strlen(str));

        if (length == 0)
        {
            return false;
        }

        *out = 0;

        int pow = 1;

        for (int i = length - 1; i >= 0; i--)
        {
            int val = str[i] & (~(0x30));
            if (val < 0 || val > 9)
            {
                return false;
            }
            *out += val * pow;
            pow *= 10;
        }

        if (sign == -1)
        {
            *out *= -1;
        }

        return true;
    }

private:

    char buffer[capacity];
};
