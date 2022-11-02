// 2022/10/17 12:31:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "SCPI/SCPI.h"
#include "stm_includes.h"
#include <cstring>


namespace HAL_USART3
{
    static UART_HandleTypeDef handleUSART3 =
    {
        USART3,
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

    void *handle = (void *)&handleUSART3;

    static uint8 buffer = 0;
}


void HAL_USART3::Init()
{
    __HAL_RCC_USART3_CLK_ENABLE();

    HAL_PIO::Init(HPort::_D, HPin::_8, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF7_USART2);      // TX
    HAL_PIO::Init(HPort::_D, HPin::_9, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF7_USART2);      // RX

    HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART3_IRQn);

    HAL_UART_Init(&handleUSART3);

    HAL_UART_Receive_IT(&handleUSART3, (uint8_t *)&buffer, 1);
}


void HAL_USART3::SendZ(pchar message)
{
    HAL_UART_Transmit(&handleUSART3, (const uint8 *)message, (uint16)(std::strlen(message) + 1), 100);
}


void HAL_USART3::Send(pchar message)
{
    HAL_UART_Transmit(&handleUSART3, (const uint8 *)message, (uint16)(std::strlen(message)), 100);
}


void HAL_USART3::CallbackOnReceive()
{
    SCPI::CallbackOnReceive(buffer);

    HAL_UART_Receive_IT(&handleUSART3, (uint8_t *)&buffer, 1);
}
