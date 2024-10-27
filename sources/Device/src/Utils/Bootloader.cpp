// 2024/10/27 15:32:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Bootloader.h"


void Bootloader::Run(uint version_build_new)
{
    if (VERSION_BUILD == version_build_new || VERSION_BUILD == 0 || VERSION_BUILD == (uint)-1)
    {
        return;
    }
}
