// 2022/10/17 15:30:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Hardware/Timer.h"
#include "Utils/Profiler.h"
#include "Menu/PasswordResolver.h"
#include "Nextion/Keyboard.h"
#include "Menu/Menu.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <cstdlib>


#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif


/*
*   ��������� �� �������
*  +------------+--------+--------+
*  | ������     | ������ | ������ |
*  +------------+--------+--------+
*  |      Page 0         |        |
*  |   1        |  "0Z"  |        |
*  |   2        |  "1Z"  |        |
*  |   3        |  "2Z"  |        |
*  |   4        |  "3Z"  |        |
*  |   5        |  "4Z"  |        |
*  |   6        |  "5Z"  |        |
*  |  ����      |  "6Z"  |        |
*  |      Page 1 ����������       |
*  | ���������� |        |  "7Z"  |
*  | ������     |        |  "8Z"  |
*  +------------+--------+--------+
* 
*   ������� � �������
* 
*   1. �������� �������
*       �������:
*   - waveInput - ������ �������� �������
*   - waveInputSmall - ����������� ������ �������� �������
*   - waveFFT - ������ ���
*   - waveFFTsmall - ����������� ������ ���
*       �����:
*   - avp0...avp5 - ����� ���, �������������� ������� 1...6
*   - labelAC, textAC - ����������� ����������� ���������
*   - labelACsmall, textACsmall - ����������� ����������� ����������� ����������
*   - labelDC, textDC - ����������� ����������� ���������
*   - labelDCsmall, textDCsmall - ����������� ����������� ����������� ���������
*       ������:
*   - button0...button5 - 6 ������ ����� ������
*   - btMenu - ������ ����
* 
*   2. �������
*   - "vis waveInput,1" - �������� ����������� waveInput
*   - "button1.val=1" - "����������" button1
*   - "textAC.text="21.3008 mA"" ������� ������� �� textAC
*/


namespace Nextion
{
    struct BufferUART //-V730
    {
        void Push(char);
        bool Pop(char *);
    private:
        static const int SIZE = 128;
        char buffer[SIZE];
        int in_p = 0;               // ���� ����� ������������ ���� �� UART (Push)
        int out_p = 0;              // ���� ���� ����� ��������
        bool mutex_uart = false;    // ���� true, �� ����� ����� �����������
    };

    // ��� �������� �� �������
    struct Command
    {
        Command() : size(0) {} //-V730
        Command(pchar bytes, int _size);
        virtual ~Command() {}

        bool IsEmpty() const { return (size == 0); }
        virtual bool Execute() { return false; }
    protected:
        static const int MAX_LEN = 32;
        char buffer[MAX_LEN];
        int size;
    };


    struct CommandCoordinate : public Command
    {
        CommandCoordinate(pchar _bytes, int _size) : Command(_bytes, _size) {}

        virtual bool Execute() override;
    };


    // ��������� ����� �������
    struct AnswerFF : public Command
    {
        AnswerFF(pchar _bytes, int _size) : Command(_bytes, _size) {}
        virtual ~AnswerFF() override {}

        virtual bool Execute() override;
    };


    struct BufferData
    {
        BufferData() : pointer(0) {} //-V730

        void Push(char byte);
        Command *ExtractCommand();
        int NumBytes() const { return pointer; }
        char operator[](int i) { return buffer[i]; }
    private:
        static const int SIZE = 128;
        char buffer[SIZE];
        int pointer;
        void RemoveFromStart(int num_bytes);
    };
}


namespace Nextion
{
    static BufferUART bufferUART;                   // ���� ���������� ������ �� UART

    static BufferData data;                         // � ����� �������� ������

    namespace LastCode
    {
        static ResponseCode::E code = ResponseCode::InstructionSuccessful;
    }
}


void Nextion::Update()
{
    char byte = 0;

    while (bufferUART.Pop(&byte))
    {
        data.Push(byte);
    }

    bool run = true;

    while (run)
    {
        Command *command = data.ExtractCommand();

        run = command->Execute();

        delete command;
    }
}


void Nextion::CallbackOnReceive(char byte)
{
    bufferUART.Push(byte);
}


Nextion::Command::Command(const char *bytes, int _size) : size(_size)
{
    std::memcpy(buffer, bytes, (uint)size);
    buffer[size] = '\0';
}


bool Nextion::CommandCoordinate::Execute()
{
    BitSet32 x((uint *)buffer);
    BitSet32 y((uint *)(buffer + 4));

    if (buffer[8] == '+')
    {
        Keyboard::CallbackOnPress((int)x.word, (int)y.word);
    }
    else if (buffer[8] == '-')
    {
        Keyboard::CallbackOnRelease((int)x.word, (int)y.word);
    }

    return true;
}


bool Nextion::AnswerFF::Execute()
{
    if (IsEmpty())
    {
        return false;
    }

    LastCode::Set((ResponseCode::E)buffer[0]);

    if(size > 1)
    {
//        LOG_WRITE("Size return code %d", size);
    }

    return true;
}


void Nextion::BufferData::Push(char byte)
{
    if (pointer == SIZE)
    {
        RemoveFromStart(1);
    }

    buffer[pointer++] = byte;
}


Nextion::Command *Nextion::BufferData::ExtractCommand()
{
    for (int i = 0; i < pointer; i++)
    {
        if (buffer[i] == (uint8)'+' || buffer[i] == (uint8)'-')
        {
            int num_symbols = i + 1;

            if (i == 8)
            {
                CommandCoordinate *result = new CommandCoordinate(buffer, num_symbols);

                RemoveFromStart(num_symbols);

                return result;
            }
            else
            {
                RemoveFromStart(num_symbols);
            }
        }
    }

    if (pointer > 3)
    {
        for (int i = 0; i < pointer - 2; i++)
        {
            if (std::memcmp(&buffer[i], "\xFF\xFF\xFF", 3) == 0)
            {
                AnswerFF *result = new AnswerFF(buffer, i);

//                for (int j = 0; j < i + 3; j++)
//                {
//                    char symbol[2] = { (char)buffer[j], '\0' };
//                    HAL_USART3::SendText(symbol);
//                }

                RemoveFromStart(i + 3);

                return result;
            }
        }
    }

    return new Command();
}


void Nextion::BufferData::RemoveFromStart(int num_bytes)
{
    if (num_bytes == pointer)
    {
        pointer = 0;
    }
    else
    {
        std::memmove(buffer, buffer + num_bytes, (uint)(pointer - num_bytes));
        pointer -= num_bytes;
    }
}


void Nextion::BufferUART::Push(char byte)
{
    mutex_uart = true;

    if (out_p > 1)
    {
        int num_bytes = out_p;

        std::memmove(buffer, buffer + num_bytes, (size_t)num_bytes);

        out_p = 0;
        in_p -= num_bytes;
    }

    buffer[in_p++] = byte;

    mutex_uart = false;
}


bool Nextion::BufferUART::Pop(char *byte)
{
    if (mutex_uart || in_p == out_p)
    {
        return false;
    }

    *byte = buffer[out_p++];

    return true;
}


void Nextion::DrawLineH(int y, int x1, int x2, const Color &color)
{
    DrawLine(x1, y, x2, y, color);
}


void Nextion::DrawLineV(int x, int y1, int y2, const Color &color)
{
    DrawLine(x, y1, x, y2, color);
}


ResponseCode::E Nextion::LastCode::Get()
{
    return code;
}


void Nextion::LastCode::Set(ResponseCode::E _code)
{
    code = _code;
}


void Nextion::SetVisible(pchar name, bool visible)
{
    SendCommandFormat("vis %s,%d", name, visible ? 1 : 0);
}


#ifndef WIN32
#pragma clang diagnostic pop
#endif
