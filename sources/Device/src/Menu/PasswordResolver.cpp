// 2022/11/14 11:07:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/PasswordResolver.h"


namespace PasswordResolver
{
    static const int NUM_BYTES = 11;
    static const uint8 bytes[NUM_BYTES] = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 };
    static int pointer = 0;
}


void PasswordResolver::AppendByte(uint8 byte)
{
    if (bytes[pointer] == (byte & 0x0F))
    {
        pointer++;
    }
    else
    {
        pointer = 0;
    }

    if (pointer == NUM_BYTES)
    {

    }
}
