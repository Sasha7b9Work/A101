// 2022/10/17 12:30:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "SCPI/SCPI.h"
#include "stm_includes.h"
#include <cstring>


namespace HAL_UART4
{
    static UART_HandleTypeDef handleUART4 =
    {
        UART4,
        {
            115200,
            UART_WORDLENGTH_8B,
            UART_STOPBITS_1,
            UART_PARITY_NONE,
            UART_MODE_TX_RX,
            UART_HWCONTROL_NONE,
            UART_OVERSAMPLING_16
        },
        nullptr,
        0,
        0,
        nullptr,
        0,
        0,
        0,
        nullptr,
        nullptr,
        HAL_UNLOCKED,
        HAL_UART_STATE_RESET,
        HAL_UART_STATE_RESET,
        0
    };

    void *handle = (void *)&handleUART4;

    static uint8 buffer = 0;
}


void HAL_UART4::Init()
{
    __HAL_RCC_UART4_CLK_ENABLE();

    HAL_PIO::Init(HPort::_A, HPin::_0, HMode::AF_PP, HPull::Up, HSpeed::High, HAlternate::AF8_UART4);      // TX
    HAL_PIO::Init(HPort::_A, HPin::_1, HMode::AF_PP, HPull::Up, HSpeed::High, HAlternate::AF8_UART4);      // RX

    HAL_UART_Init(&handleUART4);
    
    HAL_NVIC_SetPriority(UART4_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(UART4_IRQn);

    HAL_UART_Receive_IT(&handleUART4, (uint8_t *)&buffer, 1);
}


void HAL_UART4::SendText(pchar message)
{
    HAL_UART_Transmit(&handleUART4, (const uint8 *)message, (uint16)std::strlen(message), 100);
}


void HAL_UART4::Send0D0A()
{
    HAL_UART_Transmit(&handleUART4, (const uint8 *)"\x0D\x0A", 2, 100);
}


void HAL_UART4::CallbackOnReceive()
{
    SCPI::CallbackOnReceive(Direction::RS232, buffer);

    HAL_UART_Receive_IT(&handleUART4, (uint8_t *)&buffer, 1);
}
