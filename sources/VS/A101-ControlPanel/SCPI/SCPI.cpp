// 2022/11/02 08:12:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ComPort.h"
#include "SCPI/Parser/Parser.h"


namespace SCPI
{
    static ComPort port;


    struct Command
    {
        Command(uint8 * = nullptr, uint = 0);
        bool Execute();
    private:
        vector<char> buffer;
    };


    struct Buffer
    {
        void Push(const vector<uint8> &);
        Command Extract();
    private:
        vector<uint8> data;
    } buffer;


    static bool OpenPort();
    static void ClosePort();
}


bool SCPI::OpenPort()
{
    if (port.Open())
    {
        SCPI::Send(":connect");
    }

    return port.IsOpened();
}


void SCPI::ClosePort()
{
    SCPI::Send(":disconnect");

    port.Close();
}


void SCPI::Init()
{
    OpenPort();
}


void SCPI::DeInit()
{
    ClosePort();
}


void SCPI::Update()
{
    if (port.IsOpened())
    {
        buffer.Push(port.PopAll());

        while (buffer.Extract().Execute())
        {
        }
    }
    else
    {
        OpenPort();
    }
}


void SCPI::Event::ChangePort()
{
    ClosePort();
    OpenPort();
}


void SCPI::Send(pchar format, ...)
{
    if (port.IsOpened())
    {
        char message[1024];

        va_list args;
        va_start(args, message);
        vsprintf(message, format, args);
        va_end(args);

        port.Send((uint8 *)message, (int)strlen(message));

        port.Send((uint8 *)"\x0a\x0d", 2);
    }
}


void SCPI::Buffer::Push(const vector<uint8> &bytes)
{
    data.insert(data.end(), bytes.begin(), bytes.end());
}


SCPI::Command SCPI::Buffer::Extract()
{
    while (data.size() && (data[0] == 0x0d || data[0] == 0x0a))
    {
        data.erase(data.begin());
    }

    vector<uint8> bytes(128);

    for (uint i = 0; i < data.size(); i++)
    {
        uint8 byte = data[i];

        if (byte == 0x0d || byte == 0x0a)
        {
            break;
        }
        else
        {
            bytes.push_back(byte);
        }
    }

    if (data.size())
    {
        data.erase(data.begin(), data.begin() + (int)bytes.size());

        while (data.size() && (data[0] == 0x0d || data[0] == 0x0a))
        {
            data.erase(data.begin());
        }
    }

    return Command(bytes.data(), bytes.size());
}


bool SCPI::Command::Execute()
{
    if (buffer.size() == 0)
    {
        return true;
    }

    Parser::ParseCommand(buffer);

    return false;
}


SCPI::Command::Command(uint8 *bytes, uint size)
{
    if (bytes == nullptr || size == 0)
    {
        return;
    }

    for (uint i = 0; i < size; i++)
    {
        buffer.push_back((char)*bytes++);
    }
}
