// 2022/10/17 15:30:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Hardware/Timer.h"
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
*  |   1    | "01Z" / "00Z" |
*  |   2    | "11Z" / "10Z" |
*  |   3    | "21Z" / "20Z" |
*  |   4    | "31Z" / "30Z" |
*  |   5    | "41Z" / "40Z" |
*  |   6    | "51Z" / "50Z" |
*  |  Меню  | "61Z" / "60Z" |
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
*   - avp0...avp5 - метки АВП, соотвествующие кнопкам 1...6
*   - labelAC, textAC - отображение переменного измерения
*   - labelACsmall, textACsmall - уменьшенное отображение переменного напряжения
*   - labelDC, textDC - отображение постоянного измерения
*   - labelDCsmall, textDCsmall - уменьшенное отображение постоянного измерения
*       Кнопки:
*   - button0...button5 - 6 кнопок внизу экрана
*   - btMenu - кнопка меню
* 
*   2. Команды
*   - "vis waveInput,1" - включить отображение waveInput
*   - "button1.val=1" - "подсветить" button1
*   - "textAC.text="21.3008 mA"" нанести надпись на textAC
*/


namespace DInterface
{
    struct BufferUART
    {
        void Push(uint8);
        bool Pop(uint8 *);
    private:
        static const int SIZE = 128;
        uint8 buffer[SIZE];
        int in_p = 0;               // Сюда будет записываться байт из UART (Push)
        int out_p = 0;              // Этот байт нужно забирать
        bool mutex_uart = false;    // Если true, то буфер занят прерыванием
    };

    struct Command
    {
        Command() : size(0) {} //-V730
        Command(const uint8 *bytes, int _size);
        virtual ~Command() {}

        bool IsEmpty() const { return (size == 0); }
        virtual bool Execute() { return false; }
    protected:
        static const int MAX_LEN = 32;
        uint8 buffer[MAX_LEN];
        int size;
    };


    // Команда дисплея
    struct CommandZ : public Command
    {
        CommandZ(const uint8 *_bytes, int _size) : Command(_bytes, _size) {}
        virtual ~CommandZ() override {}

        virtual bool Execute() override;
    };


    // Служебный ответ дисплея
    struct AnswerFF : public Command
    {
        AnswerFF(const uint8 *_bytes, int _size) : Command(_bytes, _size) {}
        virtual ~AnswerFF() override {}

        virtual bool Execute() override;
    };


    struct BufferData
    {
        BufferData() : pointer(0) {} //-V730

        void Push(uint8 byte);
        Command *ExtractCommand();
        int NumBytes() const { return pointer; }
        uint8 operator[](int i) { return buffer[i]; }
    private:
        static const int SIZE = 128;
        uint8 buffer[SIZE];
        int pointer;
        void RemoveFromStart(int num_bytes);
    };
}


namespace DInterface
{
    static BufferUART bufferUART;                   // Сюда складываем даныне из UART

    static BufferData data;                         // А здесь принятые данные

    static ResponseCode::E last_code = ResponseCode::InstructionSuccessful;
}


void DInterface::Update()
{
    uint8 byte = 0;

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

    if (data.NumBytes())
    {
//        LOG_WRITE("After %s in buffer %d received bytes : %2Xh", __FUNCTION__, data.NumBytes(), data[0]);
    }
}


ResponseCode::E DInterface::LastCode()
{
    return last_code;
}


void DInterface::CallbackOnReceive(uint8 byte)
{
    bufferUART.Push(byte);
}


void DInterface::SendCommandRAW(pchar command, bool wait)
{
    last_code = ResponseCode::None;

    HAL_USART2::SendNZ(command);

    HAL_USART2::SendNZ("\xFF\xFF\xFF");

    if (wait)
    {
        WaitResponse(command, ResponseCode::InstructionSuccessful);
    }
}


void DInterface::WaitResponse(pchar command, ResponseCode::E code)
{
    TimeMeterMS meter;

    while (last_code == ResponseCode::None)
    {
        Update();

        if (meter.ElapsedTime() > 200)
        {
            LOG_WRITE("No response received");

            break;
        }
    }

    if (last_code != code)
    {
        LOG_WRITE("Error in %s : Received %02Xh but expected %02Xh", command, last_code, code);
    }
}


void DInterface::SendByte(uint8 byte)
{
    last_code = ResponseCode::None;

    HAL_USART2::SendByte(byte);
}


void DInterface::SendCommandFormat(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommandRAW(message, true);
}


void DInterface::SendCommandFormatWithoutWaiting(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommandRAW(message, false);
}


void DInterface::SendCommandFormatLog(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    LOG_WRITE(message);

    SendCommandRAW(message, true);
}


DInterface::Command::Command(const uint8 *bytes, int _size) : size(_size)
{
    std::memcpy(buffer, bytes, (uint)size);
}


bool DInterface::CommandZ::Execute()
{
    if (IsEmpty())
    {
        return false;
    }
    else if (size == 1)
    {
        uint8 byte1 = buffer[0];

        if (byte1 >= '0' && byte1 <= '9')
        {
            int button = (byte1 & 0x0F);

            Page::Current()->GetButton(button)->Press();

            return true;
        }
    }

    return true;
}


bool DInterface::AnswerFF::Execute()
{
    if (IsEmpty())
    {
        return false;
    }

    last_code = (ResponseCode::E)buffer[0];

    if(size > 1)
    {
        LOG_WRITE("Size return code %d", size);
    }

    return true;
}


void DInterface::BufferData::Push(uint8 byte)
{
    if (pointer == SIZE)
    {
        RemoveFromStart(1);
    }

    buffer[pointer++] = byte;
}


DInterface::Command *DInterface::BufferData::ExtractCommand()
{
    for (int i = 0; i < pointer; i++)
    {
        if (buffer[i] == (uint8)'Z')
        {
            CommandZ *result = new CommandZ(buffer, i);

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
                AnswerFF *result = new AnswerFF(buffer, i);

                RemoveFromStart(i + 3);

                return result;
            }
        }
    }

    return new Command();
}


void DInterface::BufferData::RemoveFromStart(int num_bytes)
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


void DInterface::BufferUART::Push(uint8 byte)
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


bool DInterface::BufferUART::Pop(uint8 *byte)
{
    if (mutex_uart || in_p == out_p)
    {
        return false;
    }

    *byte = buffer[out_p++];

    return true;
}


#ifndef WIN32
#pragma clang diagnostic pop
#endif
