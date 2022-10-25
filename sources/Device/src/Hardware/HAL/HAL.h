// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings;


#define ERROR_HANDLER() HAL::ErrorHandler()


namespace HAL
{
    void Init();

    void ErrorHandler();
};


namespace HAL_TIM
{
    uint TimeMS();

    void Delay(uint timeMS);
};


// ��� ��������� ����� ���������� ������� - ����������� � �����
namespace HAL_TIM2
{
    void Init();

    uint GetTicks();

    void StartMultiMeasurement();

    uint TimeUS();
};


// ��� ������� ������� Timer
namespace HAL_TIM3
{
    void Init();

    void StartIT(uint period);

    void StopIT();
};


// ��� ������ ��� (�� ������� ����� ������� ����� ������ ���� ����� ���)
namespace HAL_TIM4
{
    void Init();

    // ��������� ������ � �������� ������������ dUS �����������
    void StartPeriodicUS(uint dUS);

    // ����� �� ���� ������� ����������, ����� ����������� ������. ������������ ���������� ��������� ����
    void WaitEvent();

    void Stop();
}


// ��� AD7691BRMZ
namespace HAL_SPI2
{
    void Init();
};


// RS232
namespace HAL_UART4
{
    void Init();
}


// �������
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
