// 2022/10/17 12:30:56 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include <stm32f4xx_hal.h>


namespace HAL_USART2
{
    static UART_HandleTypeDef handleUSART2;

    void *handle = (void *)&handleUSART2;
}


void HAL_USART2::Init()
{

}
