// 2022/11/14 11:07:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/PasswordResolver.h"
#include "Menu/Pages/Pages.h"


namespace PasswordResolver
{
    static const int NUM_BYTES = 6;
    static const uint8 bytes[NUM_BYTES] = { 0, 1, 2, 3, 4, 5 };
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
        PageTwo::EnableCalibration();
//        PageTwo::EnableDebug();
    }
}
