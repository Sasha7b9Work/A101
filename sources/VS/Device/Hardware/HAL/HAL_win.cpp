// 2022/10/21 16:57:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "stm_includes.h"
#include <ctime>


uint HAL_GetTick()
{
    return clock();
}


void HAL_Delay(uint dT)
{
    uint end = HAL_GetTick() + dT;

    while (HAL_GetTick() < end)
    {
    }
}


void HAL_GPIO_WritePin(GPIO_TypeDef *, uint16_t, GPIO_PinState)
{

}


void  HAL_GPIO_Init(GPIO_TypeDef *, GPIO_InitTypeDef *)
{

}


void HAL::Init()
{

}
