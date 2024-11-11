// 2024/10/27 15:32:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "defines.cpp"
#include "Utils/Bootloader.h"


void Bootloader::Run(uint version_build_new, uint /*size*/, uint /*crc32*/)
{
    if (VERSION_BUILD == version_build_new ||               // ќбновл€ть не нужно - совпадает с текущей версией
        VERSION_BUILD == 0 || VERSION_BUILD == (uint)-1)
    {
        return;
    }
}
