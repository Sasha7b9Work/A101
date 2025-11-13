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

    void Save(CalibrationSettings *);
    bool Load(CalibrationSettings *);

    void Save(Settings *);
    bool Load(Settings *);

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


// Для чтения АЦП (по сигналу этого таймера нужно читать одну точку АЦП)
namespace HAL_TIM4
{
    void Init();

    // Запустить таймер с периодом срабатывания dUS микросекнуд
    void Start();

    void Stop();
}


// АЦП AD7691BRMZ
namespace HAL_SPI2
{
    void Init();
}


// Дисплей
namespace HAL_USART2
{
    void Init();

    // Посылает без завершаюшего нуля
    void SendNZ(pchar);

    extern void *handle;   // UART_HandleTypeDef

    void CallbackOnReceive();

    // Разрешена передача по прервыванию
    bool TransferITAllowed();

    // Передать по прерывания
    void TransmitIT(pchar);

    // Вызывается по завершению передачи по IT
    void CallbackOnTransferIT();
}


// RS232
namespace HAL_UART4
{
    void Init();

    void SendText(pchar);

    void Send0D0A();

    extern void *handle;    // UART_HandleTypeDef

    void CallbackOnReceive();
}


// USB FT232RL
namespace HAL_USART3
{
    void Init();

    void SendText(pchar);

    void SendTextWith0D0A(pchar);

    void Send0D0A();

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
    void UART4_IRQHandler();
#ifdef __cplusplus
}
#endif
