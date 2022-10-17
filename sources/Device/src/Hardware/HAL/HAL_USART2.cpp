// 2022/10/17 12:30:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Display/DInterface.h"
#include <stm32f4xx_hal.h>
#include <cstring>


namespace HAL_USART2
{
    static UART_HandleTypeDef handleUSART2 =
    {
        USART2,
        {
            115200,
            UART_WORDLENGTH_8B,
            UART_STOPBITS_1,
            UART_PARITY_NONE,
            UART_MODE_TX_RX,
            UART_HWCONTROL_NONE,
            UART_OVERSAMPLING_16
        }
    };

    void *handle = (void *)&handleUSART2;

    uint8 buffer = 0;
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


void HAL_USART2::Send(pchar command)
{
    HAL_UART_Transmit(&handleUSART2, (uint8 *)command, (uint16)std::strlen(command), 100);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *)
{
    Display::Interface::Process(HAL_USART2::buffer);

    HAL_UART_Receive_IT((UART_HandleTypeDef *)HAL_USART2::handle, (uint8_t *)&HAL_USART2::buffer, 1);
}
