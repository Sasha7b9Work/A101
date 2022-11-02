// 2022/11/02 08:51:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/ComPort.h"
#include "SCPI/rs232.h"
#include "Dialogs/DialogNumberComPort.h"
#include <thread>



ComPort::~ComPort()
{
    Close();
}


bool ComPort::Open()
{
    number = DialogNumberComPort::NumComPort();

    if (!RS232_OpenComport(number, 115200, "8n1", 0))
    {
        number = -1;
    }
    else
    {
        std::thread thread(ComPort::FuncReceive, this);
        thread.detach();
    }

    return (number >= 0);
}


void ComPort::Close()
{
    RS232_CloseComport(number);
}


bool ComPort::IsOpened() const
{
    return (number >= 0);
}


void ComPort::Send(uint8 byte)
{
    RS232_SendByte(number, byte);
}


void ComPort::FuncReceive(ComPort *com_port)
{
    while (com_port->IsOpened())
    {
        uint8 byte = 0;

        if (RS232_PollComport(com_port->number, &byte, 1) != 1)
        {
            com_port->CallbackOnReceive(byte);
        }
    }
}
