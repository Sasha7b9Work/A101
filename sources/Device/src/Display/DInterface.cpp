// 2022/10/17 15:30:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


#ifndef WIN32
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wweak-vtables"
#endif


/*
*   Принимаем от дисплея
*  +--------+---------------+
*  | кнопка | нажата/отжата |
*  +--------+---------------+
*  |   1    | "11Z" / "10Z" |
*  |   2    | "21Z" / "20Z" |
*  |   3    | "31Z" / "30Z" |
*  |   4    | "41Z" / "40Z" |
*  |   5    | "51Z" / "50Z" |
*  |   6    | "61Z" / "60Z" |
*  |  Меню  | "71Z" / "70Z" |
*  +--------+---------------+
* 
*   Передаём в дисплей
* 
*   1. Элементы дисплея
*       Графики:
*   - waveInput - график входного сигнала
*   - waveInputSmall - уменьшенный график входного сигнала
*   - waveFFT - график БПФ
*   - waveFFTsmall - уменьшенный график БПФ
*       Текст:
*   - avp1...avp2 - метки АВП, соотвествующие кнопкам 1...6
*   - labelAC, textAC - отображение переменного измерения
*   - labelACsmall, textACsmall - уменьшенное отображение переменного напряжения
*   - labelDC, textDC - отображение постоянного измерения
*   - labelDCsmall, textDCsmall - уменьшенное отображение постоянного измерения
*       Кнопки:
*   - button1...button6 - 6 кнопок внизу экрана
*   - btMenu - кнопка меню
* 
*   2. Команды
*   - "vis waveInput,1" - включить отображение waveInput
*   - "button1.val=1" - "подсветить" button1
*   - "textAC.text="21.3008 mA"" нанести надпись на textAC
*/


namespace DInterface
{
    struct CommandUART
    {
        CommandUART() : size(0) {}
        CommandUART(uint8 *bytes, int _size);
        virtual ~CommandUART() {}

        bool IsEmpty() const   { return (size == 0); }
        virtual bool Execute() { return false; }
    protected:
        static const int SIZE = 16;
        uint8 buffer[SIZE];
        int size;
    };

    struct CommandUART_Z : public CommandUART
    {
        CommandUART_Z(uint8 *_bytes, int _size) : CommandUART(_bytes, _size) {}
        virtual ~CommandUART_Z() override {}

        virtual bool Execute() override;
    };

    struct CommandUART_FF : public CommandUART
    {
        CommandUART_FF(uint8 *_bytes, int _size) : CommandUART(_bytes, _size) {}
        virtual ~CommandUART_FF() override {}

        virtual bool Execute() override;
    };


    template<uint size>
    struct BufferUART
    {
        BufferUART() : pointer(0) {}

        void Push(uint8 byte);
        CommandUART *ExtractCommand();
        int NumBytes() const        { return pointer;   }
        uint8 operator[](int i)     { return buffer[i]; }
    private:
        uint8 buffer[size];
        int pointer;
        void RemoveFromStart(int num_bytes);
    };

    static BufferUART <32>buffer;

    static ReturnCodeDI::E last_code = ReturnCodeDI::InstructionSuccessful;

    static int bytes_received = 0;          // Всего принято байт
}


void DInterface::Update()
{
    for (int i = 0; i < buffer.NumBytes(); i++)
    {
//        Log::Write("byte %d : %2X", bytes_received - buffer.NumBytes() + i, buffer[i]);
    }

    bool run = true;

    while (run)
    {
        CommandUART *command = buffer.ExtractCommand();

        run = command->Execute();

        delete command;
    }
}


ReturnCodeDI::E DInterface::LastCode()
{
    return last_code;
}


void DInterface::CallbackOnReceive(uint8 byte)
{
    bytes_received++;

    buffer.Push(byte);
}


void DInterface::SendCommand(pchar command)
{
    last_code = ReturnCodeDI::_None;

    HAL_USART2::Send(command);

    HAL_USART2::Send("\xFF\xFF\xFF");
}


void DInterface::SendByte(uint8 byte)
{
    last_code = ReturnCodeDI::_None;

    HAL_USART2::SendByte(byte);
}


void DInterface::SendCommandFormat(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommand(message);
}

#ifndef WIN32
#pragma clang diagnostic pop
#endif


DInterface::CommandUART::CommandUART(uint8 *bytes, int _size) : size(_size)
{
    std::memcpy(buffer, bytes, (uint)size);
}


bool DInterface::CommandUART_Z::Execute()
{
    if (IsEmpty())
    {
        return false;
    }
    else if (size == 2)
    {
        uint8 byte1 = buffer[0];

        if (byte1 >= '1' && byte1 <= '9')
        {
            int button = byte1 & 0x0F;

            int state = buffer[1] & 0x0F;

            Button::ForIndex(button)->ToState(state);

            return true;
        }
    }

    return true;
}


bool DInterface::CommandUART_FF::Execute()
{
    if (IsEmpty())
    {
        return false;
    }
    else if (size == 1)
    {
        last_code = (ReturnCodeDI::E)buffer[0];
        if (last_code != ReturnCodeDI::TransparentDataFinished && last_code != ReturnCodeDI::TransparentDataReady)
        {
            Log::Write("Return code %2X", (int)last_code);
        }
    }
    else
    {
        Log::Write("Size return code %d", size);
    }

    return true;
}


template<uint size>
void DInterface::BufferUART<size>::Push(uint8 byte)
{
    if (pointer == size)
    {
        RemoveFromStart(1);
    }

    buffer[pointer++] = byte;
}


template<uint size>
DInterface::CommandUART *DInterface::BufferUART<size>::ExtractCommand()
{
    for (int i = 0; i < pointer; i++)
    {
        if (buffer[i] == (uint8)'Z')
        {
            CommandUART_Z *result = new CommandUART_Z(buffer, i);

            RemoveFromStart(i + 1);

            return result;
        }
    }

    if (pointer > 3)
    {
        for (int i = 0; i < pointer - 2; i++)
        {
            if (std::memcmp(&buffer[i], "\xFF\xFF\xFF", 3) == 0)
            {
                CommandUART_FF *result = new CommandUART_FF(buffer, i);

                RemoveFromStart(i + 3);

                return result;
            }
        }
    }

    return new CommandUART();
}


template<uint size>
void DInterface::BufferUART<size>::RemoveFromStart(int num_bytes)
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
