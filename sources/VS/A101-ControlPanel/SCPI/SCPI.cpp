// 2022/11/02 08:12:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ComPort.h"
#include <vector>


using namespace std;


namespace SCPI
{
    static ComPort port;
}


void SCPI::Init()
{
    port.Open();
}


void SCPI::DeInit()
{
    port.Close();
}


void SCPI::Update()
{
    if (port.IsOpened())
    {
        vector<uint8> data = port.PopAll();

        if (data.size())
        {
            data = data;
        }
    }
    else
    {
        port.Open();
    }
}


void SCPI::Send(uint8 byte)
{
    if (port.IsOpened())
    {
        port.Send(byte);
    }
    else
    {
        port.Open();
    }
}
