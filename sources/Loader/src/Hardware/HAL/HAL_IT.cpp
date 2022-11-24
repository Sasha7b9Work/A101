// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "stm_includes.h"


#ifdef __cplusplus
extern "C" {
#endif


    void SysTick_Handler()
    {
        HAL_IncTick();
        HAL_SYSTICK_IRQHandler();
    }


    void MemManage_Handler()
    {
        while (1)
        {
        }
    }


    void UsageFault_Handler()
    {
        while (1)
        {
        }
    }


    void HardFault_Handler()
    {
        while (1) //-V776
        {
        }
    }


    void BusFault_Handler()
    {
        while (1)
        {
        }
    }


    void PendSV_Handler()
    {
    }


    void NMI_Handler()
    {
    }


    void SVC_Handler()
    {
    }


    void DebugMon_Handler()
    {
    }


    void TIM3_IRQHandler()
    {
        if ((TIM3->SR & TIM_SR_UIF) == TIM_SR_UIF)
        {
            if ((TIM3->DIER & TIM_DIER_UIE) == TIM_DIER_UIE)
            {
                TIM3->SR = ~TIM_DIER_UIE;
                Timer::ElapsedCallback();
            }
        }
    }


    // USB
    void USART3_IRQHandler()
    {
        HAL_UART_IRQHandler((UART_HandleTypeDef *)HAL_USART3::handle);
    }

    void HAL_UART_RxCpltCallback(UART_HandleTypeDef *handle) //-V2009
    {
        if (handle == HAL_USART3::handle)
        {
            HAL_USART3::CallbackOnReceive();
        }
    }

#ifdef __cplusplus
}
#endif
