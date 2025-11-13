// 2025/11/13 08:46:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdlib>


int ParseBuffer::GetLength() const
{
    return (int)std::strlen(data);
}


char *SU::GetWord(pchar data, int num, ParseBuffer *out)
{
    return GetString(data, num, out->data);
}


char *SU::GetString(pchar data, int num, char *out, pchar *first_symbol)
{
    if (num == 1)
    {
        out[0] = '\0';

        uint len = std::strlen(data);

        for (uint i = 0; i < len; i++)
        {
            if (data[i] == ' ' || data[i] == 0 || data[i] == '\n')
            {
                out[i] = '\0';
                return out;
            }

            out[i] = data[i];
        }

        if (first_symbol)
        {
            *first_symbol = data;
        }

        return out;
    }

    num--;

    out[0] = 0;

    uint size = std::strlen(data);

    uint pos_start = 0;

    for (uint i = 0; i < size; i++)
    {
        if (i == size - 1)
        {
            return out;
        }
        if (data[i] == ' ' || data[i] == '\n')
        {
            num--;

            if (num == 0)
            {
                pos_start = i + 1;
                break;
            }
        }
    }

    uint pos_end = pos_start;

    for (uint i = pos_end; i <= size; i++)
    {
        if (data[i] == ' ' || data[i] == '\0' || data[i] == '\n')
        {
            pos_end = i;
            break;
        }
    }

    if (pos_end > pos_start)
    {
        int index = 0;

        if (first_symbol)
        {
            *first_symbol = &data[pos_start];
        }

        for (uint i = pos_start; i < pos_end; i++)
        {
            out[index++] = data[i];
        }

        out[index] = 0;
    }

    return out;
}


bool SU::AtoUInt(pchar str, uint *value, int radix)
{
    if (*str == '\0')
    {
        return false;
    }

    char *end;

    uint r = (uint)std::strtoull(str, &end, radix);

    if ((end != (str + std::strlen(str))))
    {
        return false;
    }

    *value = r;

    return true;
}
