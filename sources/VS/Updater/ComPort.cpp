// 2022/10/30 16:57:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "ComPort.h"
#include "rs232.h"
#include <string>
#include <iostream>

using namespace std;


void ComPort::Open(char *name_port)
{
    port = ExtractNumberPort(name_port);

    while (RS232_OpenComport(port, 115200, "8n1", 0) != 0)
    {
        cout << "Can not open com port number " << port << endl;
    }
}


bool ComPort::ConnectToDeivce()
{
    return false;
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