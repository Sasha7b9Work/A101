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

    if (RS232_OpenComport(number, 115200, "8n1", 0) == 0)
    {
        std::thread thread(ComPort::FuncReceive, this);
        thread.detach();
    }
    else
    {
        number = -1;
    }

    return (number >= 0);
}


void ComPort::Close()
{
    RS232_CloseComport(number);

    number = -1;
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
        com_port->ReadByte();
    }
}


void ComPort::ReadByte()
{
    uint8 byte = 0;

    if (RS232_PollComport(number, &byte, 1))
    {
        received.Push(byte);
    }
}


void ComPort::Buffer::Push(uint8 byte)
{
    mutex_buffer.lock();

    buffer.push_back(byte);

    mutex_buffer.unlock();
}


std::vector<uint8> ComPort::Buffer::PopAll()
{
    mutex_buffer.lock();

    std::vector<uint8> result = buffer;

    buffer.clear();

    mutex_buffer.unlock();

    return result;
}
