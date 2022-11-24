// 2022/10/30 16:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ComPort.h"
#include "rs232.h"
#include <string>
#include <iostream>
#include <thread>

using namespace std;


static void FuncThread(int num_port, void (*func_callback)(char))
{
    uint8 buffer = 0;

    while (true)
    {
        if (RS232_PollComport(num_port, &buffer, 1) == 1)
        {
            func_callback((char)buffer);
        }
    }
}


void ComPort::Open(char *name_port, void (*func_callback)(char))
{
    static uint time = GetTickCount();

    port = ExtractNumberPort(name_port);

    while (RS232_OpenComport(port - 1, 115200, "8n1", 0) != 0)
    {
        if (GetTickCount() - time > 1000)
        {
            time = GetTickCount();

            cout << "Can not open com port number " << port << endl;
        }
    }

    thread t(FuncThread, port, func_callback);
}


int ComPort::ExtractNumberPort(char *name_port)
{
    string name(name_port);

    int result = 0;

    for each (auto c in name)
    {
        if (isdigit(c))
        {
            result *= 10;
            result += c & 0x0F;
        }
        else
        {
            if (result != 0)
            {
                break;
            }
        }
    }

    return result;
}


void ComPort::Send(pchar msg)
{
    string message = msg;

    message.append("\x0D");

    RS232_SendBuf(port, (const unsigned char *)message.c_str(), (int)message.size());
}


void ComPort::SendBuffer(uint8 *buffer, int size)
{
    RS232_SendBuf(port, buffer, size);
}
