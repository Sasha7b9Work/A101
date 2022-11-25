// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


#define ERROR_HANDLER() HAL::ErrorHandler()


namespace HAL
{
    void Init();

    void ErrorHandler();
}


namespace HAL_CRC32
{
    void Init();

    uint Calculate(const void *data, uint num_bytes);
}


namespace HAL_EEPROM
{
    void Erase(int size);

    void Write(uint8 *buffer, int size);
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


// USB FT232RL
namespace HAL_USART3
{
    void Init();

    void SendText(pchar);

    // Послать с 0d в конце
    void SendTextWith0D(pchar);

    extern void *handle;    // UART_HandleTypeDef

    void CallbackOnReceive();
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
    void USART3_IRQHandler();
#ifdef __cplusplus
}
#endif
