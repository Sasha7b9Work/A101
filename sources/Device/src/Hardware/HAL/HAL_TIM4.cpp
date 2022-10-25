// 2022/10/18 08:51:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_TIM4
{
    static TIM_HandleTypeDef handleTIM4 = { TIM4 };
}


void HAL_TIM4::Init()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    handleTIM4.Init.Prescaler = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM4.Init.Period = (uint)(-1);
//    handleTIM4
}


void HAL_TIM4::StartPeriodicUS(uint)
{

}


void HAL_TIM4::WaitEvent()
{

}


void HAL_TIM4::Stop()
{

}
