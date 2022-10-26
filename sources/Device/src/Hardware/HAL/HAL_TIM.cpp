// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif
#include <stm32f4xx_hal.h>
#ifndef WIN32
#pragma clang diagnostic pop
#endif


uint HAL_TIM::TimeMS()
{
    return HAL_GetTick();
}


void HAL_TIM::Delay(uint timeMS)
{
    HAL_Delay(timeMS);
}
