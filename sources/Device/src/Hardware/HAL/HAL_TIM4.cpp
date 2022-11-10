// 2022/10/18 08:51:29 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "stm_includes.h"


namespace HAL_TIM4
{
    static TIM_HandleTypeDef handleTIM4 =
    {
        TIM4,
        {

        },
        HAL_TIM_ACTIVE_CHANNEL_CLEARED,
        {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr},
        HAL_UNLOCKED,
        HAL_TIM_STATE_RESET,
        {HAL_TIM_CHANNEL_STATE_RESET, HAL_TIM_CHANNEL_STATE_RESET, HAL_TIM_CHANNEL_STATE_RESET, HAL_TIM_CHANNEL_STATE_RESET},
        {HAL_TIM_CHANNEL_STATE_RESET, HAL_TIM_CHANNEL_STATE_RESET, HAL_TIM_CHANNEL_STATE_RESET, HAL_TIM_CHANNEL_STATE_RESET},
        HAL_DMA_BURST_STATE_RESET
    };

    static uint period = 83;
}


void HAL_TIM4::Init()
{
    __HAL_RCC_TIM4_CLK_ENABLE();

    handleTIM4.Init.Prescaler = 83;
    handleTIM4.Init.CounterMode = TIM_COUNTERMODE_UP;
    handleTIM4.Init.Period = (uint)(-1);                            // „астота APB1 - 84 ћ√ц. —чЄтчик будет тикать через 1мкс
    handleTIM4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    handleTIM4.Init.RepetitionCounter = 0;
    handleTIM4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&handleTIM4);
}


void HAL_TIM4::StartPeriodicUS(uint timeUS)
{
    period = timeUS;

    Stop();

    TIM4->CNT = 0;
    TIM4->CR1 |= TIM_CR1_CEN;
}


void HAL_TIM4::WaitEvent()
{
    while (TIM4->CNT < period)
    {
    }

    TIM4->CR1 &= (uint)~TIM_CR1_CEN;

    TIM4->CNT = 0;

    TIM4->CR1 |= TIM_CR1_CEN;
}


void HAL_TIM4::Stop()
{
    TIM4->CR1 &= (uint)~TIM_CR1_CEN;
}
