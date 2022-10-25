// 2022/10/18 08:51:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_TIM4
{
    static TIM_HandleTypeDef handleTIM4 = { TIM4 };
    static uint period = 83;
}


void HAL_TIM4::Init()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    handleTIM4.Init.Prescaler = 0;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM4.Init.Period = period;                        // „астота APB1 - 84 ћ√ц. —читать будем через 1 ћ√ц
    handleTIM4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&handleTIM4);
}


void HAL_TIM4::StartPeriodicUS(uint timeUS)
{
    period = 84 * timeUS - 1;
    Stop();
    TIM4->CNT = 0;
    TIM4->CR1 |= TIM_CR1_CEN;
}


void HAL_TIM4::WaitEvent()
{
    while (TIM4->CNT < period)
    {
    }
    Stop();
    TIM4->CNT = 0;
    TIM4->CR1 |= TIM_CR1_CEN;
}


void HAL_TIM4::Stop()
{
    TIM4->CR1 &= (uint)~TIM_CR1_CEN;
}
