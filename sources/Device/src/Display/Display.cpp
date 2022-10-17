// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>
#include <cstdio>


void Display::SetDC(float value)
{
    char buffer[100];

    std::sprintf(buffer, "dc.txt=\"%.4f mA\"\xFF\xFF\xFF", value);

    HAL_USART2::Send(buffer);
}
