// 2022/11/23 11:28:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/Communicator.h"
#include "Hardware/HAL/HAL.h"


void Communicator::SendWith0D0A(Direction::E dir, pchar message)
{
    if (dir & Direction::USB)
    {
        HAL_USART3::SendTextWith0D0A(message);
    }

    if (dir & Direction::RS232)
    {
        HAL_UART4::SendTextWith0D0A(message);
    }
}


void Communicator::Send(Direction::E dir, pchar message)
{
    if (dir & Direction::USB)
    {
        HAL_USART3::SendText(message);
    }

    if (dir & Direction::RS232)
    {
        HAL_UART4::SendText(message);
    }
}
