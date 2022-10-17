// 2022/10/17 12:30:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


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
}


void HAL_USART2::Init()
{
    /*
    * PA2 - TX
    * PA3 - RX
    */

    __HAL_RCC_USART2_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitTypeDef is;
    is.Pin = GPIO_PIN_2 | GPIO_PIN_3;
    is.Mode = GPIO_MODE_AF_PP;
    is.Pull = GPIO_NOPULL;
    is.Speed = GPIO_SPEED_HIGH;
    is.Alternate = GPIO_AF7_USART2;

    HAL_GPIO_Init(GPIOA, &is);

    HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
    HAL_NVIC_EnableIRQ(USART2_IRQn);

    HAL_UART_Init(&handleUSART2);
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *)
{

}
