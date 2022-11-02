// 2022/11/02 08:12:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/ComPort.h"
#include <vector>
#include <queue>


using namespace std;


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
        buffer.Push(port.PopAll());

        while (buffer.Extract().Execute())
        {
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

    data.erase(data.begin(), data.begin() + (int)bytes.size());

    while (data.size() && (data[0] == 0x0d || data[0] == 0x0a))
    {
        data.erase(data.begin());
    }

    return Command(bytes.data(), bytes.size());
}


bool SCPI::Command::Execute()
{
    if (buffer.size() == 0)
    {
        return true;
    }



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
