// 2022/11/02 08:51:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/ComPort.h"
#include "SCPI/rs232.h"
#include "Dialogs/DialogNumberComPort.h"




bool ComPort::Open()
{
    number = DialogNumberComPort::NumComPort();

    if (!RS232_OpenComport(number, 115200, "8n1", 0))
    {
        number = -1;
    }

    return (number >= 0);
}


bool ComPort::IsOpened() const
{
    return (number >= 0);
}


void ComPort::Send(uint8 byte)
{
    RS232_SendByte(number, byte);
}
