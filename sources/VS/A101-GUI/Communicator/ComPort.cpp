// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Communicator/ComPort.h"
#include "SCPI/Parser.h"
#undef CRC
#include "Communicator/rs232.h"
#include <cstring>


static int openedPort = -1;


bool ComPort::Open()
{
    char mode[] = { '8', 'N', '1', '\0' };

    uint8 buffer[4096];

    pchar message = "*IDN?\r\n";

    for (int i = 0; i < 30; i++)
    {
        if (RS232_OpenComport(i, 115200, mode, 0) == 0)
        {
            RS232_SendBuf(i, (uint8 *)message, (int)std::strlen(message));

            int n = RS232_PollComport(i, buffer, 4095);

            if (n > 0)
            {
                BufferParser pars_buf;

                Parser::GetWord((char *)buffer, 3, &pars_buf);

                if (std::strcmp(pars_buf.data, "A101") == 0)
                {
                    openedPort = i;
                    return true;
                }
            }

            RS232_CloseComport(i);
        }
    }

    return false;
}

void ComPort::Close()
{
    if (openedPort != -1)
    {
        RS232_CloseComport(openedPort);
    }
}


bool ComPort::IsOpened()
{
    return openedPort != -1;
}


void ComPort::Send(pchar buffer)
{
    if (IsOpened())
    {
        char *p = (char *)buffer;

        RS232_SendBuf(openedPort, reinterpret_cast<uint8 *>(p), (int)std::strlen(buffer));
    }
}

int ComPort::Receive(char *buffer, int size)
{
    if (IsOpened())
    {
        return RS232_PollComport(openedPort, reinterpret_cast<unsigned char *>(buffer), size);
    }

    return 0;
}
