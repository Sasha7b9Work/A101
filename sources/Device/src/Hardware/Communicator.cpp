// 2022/11/23 11:28:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Communicator.h"
#include "Hardware/HAL/HAL.h"


void Communicator::SendWith0D0A(pchar message)
{
    HAL_USART3::SendTextWith0D0A(message);
}


void Communicator::Send(pchar message)
{
    HAL_USART3::SendText(message);
}
