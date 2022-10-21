// 2022/10/21 17:00:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/HAL/HAL_PIO.h"
#include <stm32f4xx_hal_conf.h>


const uint16 HPin::_0 = GPIO_PIN_0;
const uint16 HPin::_1 = GPIO_PIN_1;
const uint16 HPin::_2 = GPIO_PIN_2;
const uint16 HPin::_3 = GPIO_PIN_3;
const uint16 HPin::_4 = GPIO_PIN_4;
const uint16 HPin::_5 = GPIO_PIN_5;
const uint16 HPin::_6 = GPIO_PIN_6;
const uint16 HPin::_7 = GPIO_PIN_7;
const uint16 HPin::_8 = GPIO_PIN_8;
const uint16 HPin::_9 = GPIO_PIN_9;
const uint16 HPin::_10 = GPIO_PIN_10;
const uint16 HPin::_11 = GPIO_PIN_11;
const uint16 HPin::_12 = GPIO_PIN_12;
const uint16 HPin::_13 = GPIO_PIN_13;
const uint16 HPin::_14 = GPIO_PIN_14;
const uint16 HPin::_15 = GPIO_PIN_15;


void HAL_PIO::Write(HPort::E port, uint16 pin, HState::E state)
{

}


void HAL_PIO::Write(HPort::E port, uint16 pin, bool state)
{

}

