// 2025/11/12 14:36:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/ComPort.h"
#include "Communicator/rs232.h"
#include "Utils/String.h"
#include "Upgrader.h"
#include <cstring>
#include <chrono>
#include <thread>


using namespace std::chrono;


namespace ComPort
{
    static int connected_port = -1;     // 0 == COM1

    static String lastAnswer;

    // Столько первых портов опрашивается
    static const int NUM_PORTS = 32;

    // Нумерация начинается с 0 : 0 == COM1
    bool PortIsExist(int);

    int Receive(char *buffer, int size);

    static std::vector<bool> ports;
}


std::vector<bool> &ComPort::GetComports()
{
    ports.clear();

    for (int i = 0; i < NUM_PORTS; i++)
    {
        static const pchar mode = "8N1";

        if (RS232_OpenComport(i, 9600, mode, 0) == 0)
        {
            ports.push_back(true);

            RS232_CloseComport(i);
        }
        else
        {
            ports.push_back(false);
        }
    }

    return ports;
}


bool ComPort::TryConnect(int num_port)
{
    if (RS232_OpenComport(num_port, 115200, "8N1", 0) == 0)
    {
        connected_port = num_port;

        return true;
    }

    connected_port = -1;

    return false;
}


pchar ComPort::NameOpenedPort()
{
    static char buffer[32];

    std::sprintf(buffer, "COM%d", connected_port + 1);

    return buffer;
}


void ComPort::Close()
{
    if (IsConnected())
    {
        RS232_CloseComport(connected_port);

        connected_port = -1;
    }
}


bool ComPort::IsConnected()
{
    return connected_port != -1;
}


void ComPort::SendCommand(const wxString &command)
{
    if (IsConnected())
    {
        Send((void *)command.c_str().AsChar(), (int)command.Length());

        Send("\x0a\x0d", 2);
    }
}


void ComPort::Send(void *buffer, int num_bytes)
{
    if (IsConnected())
    {
        RS232_SendBuf(connected_port, (uint8 *)buffer, num_bytes);
    }
}


int ComPort::Receive(char *buffer, int size)
{
    if (IsConnected())
    {
        return RS232_PollComport(connected_port, reinterpret_cast<unsigned char *>(buffer), size);
    }

    return 0;
}


void ComPort::Update()
{
    if (IsConnected())
    {
        const int SIZE_BUFFER = 4096;
        static char buffer[SIZE_BUFFER] = { 0 };  // Принятые данные

        int n = ComPort::Receive(buffer, SIZE_BUFFER);

        if (n)
        {
            Upgrader::OnEventReceive((uint8 *)buffer, n);
        }
    }
}
