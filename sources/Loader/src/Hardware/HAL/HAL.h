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
#define ADDR_SECTOR_0   ((uint)0x08000000)  //  FLASH_SECTOR_0  16k  Загрузчик
#define ADDR_SECTOR_1   ((uint)0x08004000)  //  FLASH_SECTOR_1  16k
#define ADDR_SECTOR_2   ((uint)0x08008000)  //  FLASH_SECTOR_2  16k
#define ADDR_SECTOR_3   ((uint)0x0800c000)  //  FLASH_SECTOR_3  16k
#define ADDR_SECTOR_4   ((uint)0x08010000)  //  FLASH_SECTOR_4  64k
#define ADDR_SECTOR_5   ((uint)0x08020000)  //  FLASH_SECTOR_5  128k // / Основная прошивка
#define ADDR_SECTOR_6   ((uint)0x08040000)  //  FLASH_SECTOR_6  128k // /
#define ADDR_SECTOR_7   ((uint)0x08060000)  //  FLASH_SECTOR_7  128k // / Здесь хранится прошивка обновления. Если она есть, загрузчик должен записать и стереть эти 
#define ADDR_SECTOR_8   ((uint)0x08080000)  //  FLASH_SECTOR_8  128k // / два сектора
#define ADDR_SECTOR_9   ((uint)0x080a0000)  //  FLASH_SECTOR_9  128k

    void EraseSector(uint address);
    void WriteData(uint address, const void *data, int size);
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
