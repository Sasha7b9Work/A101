// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler()


namespace HAL
{
    void Init();

    void ErrorHandler();
}


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
}


// Для измерения малых интервалов времени - микросекунд и тиков
namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
}


// Для событий таймера Timer
namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
}


// Для чтения АЦП (по сигналу этого таймера нужно читать одну точку АЦП)
namespace HAL_TIM4
{
    void Init();

    // Запустить таймер с периодом срабатывания dUS микросекнуд
    void StartPeriodicUS(uint dUS);

    // Выход из этой функции происходит, когда срабатывает таймер. Одновременно запускаетя следующий счёт
    void WaitEvent();

    void Stop();
}


// АЦП AD7691BRMZ
namespace HAL_SPI2
{
    void Init();
}


// RS232
namespace HAL_UART4
{
    void Init();
}


// Дисплей
namespace HAL_USART2
{
    void Init();

    void Send(pchar);

    void SendByte(uint8);

    extern void *handle;   // UART_HandleTypeDef
}


// USB FT232RL
namespace HAL_USART3
{
    void Init();
}


#ifdef __cplusplus
extern "C" {
#endif

    void SysTick_Handler();
    void MemManage_Handler();
    void UsageFault_Handler();
    void HardFault_Handler();
    void BusFault_Handler();
    void PendSV_Handler();
    void NMI_Handler();
    void SVC_Handler();
    void DebugMon_Handler();
    void TIM3_IRQHandler();
    void USART2_IRQHandler();
    
#ifdef __cplusplus
}
#endif
