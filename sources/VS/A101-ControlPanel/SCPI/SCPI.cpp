// 2022/11/02 08:12:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ComPort.h"


namespace SCPI
{
    static ComPort port;
}


void SCPI::Init()
{
    port.Open();
}


void SCPI::Send(uint8)
{
    if (port.IsOpened())
    {

    }
    else
    {
        port.Open();
    }
}
