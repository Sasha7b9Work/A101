// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings;
struct Settings;


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
    void Save(CalibrationSettings *);
    bool Load(CalibrationSettings *);

    void Save(Settings *);
    bool Load(Settings *);

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

    // Посылает без завершаюшего нуля
    void SendNZ(pchar);

    void SendByte(uint8);

    extern void *handle;   // UART_HandleTypeDef

    void CallbackOnReceive();
}


// USB FT232RL
namespace HAL_USART3
{
    void Init();

    void SendText(pchar);

    // Послать с 0d0a в конце
    void SendTextWith0D0A(pchar);

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
    void USART2_IRQHandler();
    void USART3_IRQHandler();
    
#ifdef __cplusplus
}
#endif
