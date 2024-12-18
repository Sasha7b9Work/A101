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


// ��� ��������� ����� ���������� ������� - ����������� � �����
namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
}


// ��� ������� ������� Timer
namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
}


// ��� ������ ��� (�� ������� ����� ������� ����� ������ ���� ����� ���)
namespace HAL_TIM4
{
    void Init();

    // ��������� ������ � �������� ������������ dUS �����������
    void Start();

    void Stop();
}


// ��� AD7691BRMZ
namespace HAL_SPI2
{
    void Init();
}


// �������
namespace HAL_USART2
{
    void Init();

    // �������� ��� ������������ ����
    void SendNZ(pchar);

    extern void *handle;   // UART_HandleTypeDef

    void CallbackOnReceive();

    // ��������� �������� �� �����������
    bool TransferITAllowed();

    // �������� �� ����������
    void TransmitIT(pchar);

    // ���������� �� ���������� �������� �� IT
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
