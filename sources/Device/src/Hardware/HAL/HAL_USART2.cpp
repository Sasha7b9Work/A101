// 2022/10/17 12:30:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Display/DInterface.h"
#include "stm_includes.h"
#include "Utils/Profiler.h"
#include <cstring>


namespace HAL_USART2
{
    static UART_HandleTypeDef handleUSART2 =
    {
        // 256000 - идут ошибки 0xFC
        // 250000 - ошибки
        // 230400 - работает, время 50-100 мс
        // 115200 - время 75-100 мс
        // 57600 - время 150-200 мс
        USART2,
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

    void *handle = (void *)&handleUSART2;

    static uint8 buffer = 0;
}


void HAL_USART2::Init()
{
    __HAL_RCC_USART2_CLK_ENABLE();

    HAL_PIO::Init(HPort::_A, HPin::_2, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF7_USART2);      // TX
    HAL_PIO::Init(HPort::_A, HPin::_3, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF7_USART2);      // RX

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_UART_Init(&handleUSART2);

    HAL_UART_Receive_IT(&handleUSART2, (uint8_t *)&buffer, 1);
}


void HAL_USART2::SendNZ(pchar command)
{

    uint16 num_bytes = (uint16)std::strlen(command);

    Profiler::AddBytes(num_bytes);

    HAL_UART_Transmit(&handleUSART2, (const uint8 *)command, num_bytes, 100);
}


void HAL_USART2::SendByte(uint8 byte)
{
    Profiler::AddByte();

    HAL_UART_Transmit(&handleUSART2, &byte, 1, 100);
}


void HAL_USART2::CallbackOnReceive()
{
    DInterface::CallbackOnReceive(HAL_USART2::buffer);

    HAL_UART_Receive_IT((UART_HandleTypeDef *)HAL_USART2::handle, (uint8_t *)&HAL_USART2::buffer, 1);
}
