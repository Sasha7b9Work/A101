// 2022/10/31 19:37:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Nextion.h"
#include "Display/DiagramFFT.h"
#include "Display/DiagramInput.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Profiler.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


namespace Nextion
{
    static void SendByte(uint8);

    static void SendCommandFormat(const char *, ...);

    // Без ожидания ответа
    static void SendCommandFormatWithoutWaiting(const char *, ...);

    // Если wait == true, то ждать ответа
    static void SendCommandRAW(pchar, bool wait);

    // Функция заверашется при получении кода
    static void WaitResponse(pchar, ResponseCode::E);
}


namespace Nextion
{
    static void WaitResponse(ResponseCode::E);
}


void Nextion::DrawRect(int x, int y, int width, int height, const Color &color)
{
    color.SetAsCurrent();

    SendCommandFormat("draw %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Nextion::FillRect(int x, int y, int width, int height, const Color & color)
{
    color.SetAsCurrent();

    SendCommandFormat("fill %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}


void Nextion::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    SendCommandFormat("line %d,%d,%d,%d,%s", x1, y1, x2, y2, Color::CurrentValue());
}


void Nextion::DrawString(int x, int y, int width, int height, int font, const Color &color, const Color &back_color, pchar text, int h_align)
{
    SendCommandFormat("xstr %d,%d,%d,%d,%d,%d,%d,%d,0,1,\"%s\"", x, y, width, height, font, color.ToRaw(), back_color.ToRaw(), h_align, text);
}


void Nextion::WaveInput::Draw(uint8 *points, int num_points)
{
    /*
    * Маленький - в форму 9
    * Большой - в форму 7
    */


    int id = (DiagramFFT::IsEnabled() && DiagramInput::IsEnabled()) ? 9 : 7;

    SendCommandFormatWithoutWaiting("addt %d,0,%d", id, num_points);

    WaitResponse(ResponseCode::TransparentDataReady);

    for (int i = 0; i < num_points; i++)
    {
        SendByte(*points++);
    }

    WaitResponse(ResponseCode::TransparentDataFinished);
}


void Nextion::WaitResponse(ResponseCode::E code)
{
    TimeMeterMS meter;

    while (LastCode::Get() != code)
    {
        if (meter.ElapsedTime() > 200)
        {
            LOG_WRITE("Not response");
            break;
        }
    }
}


void Nextion::WaveInput::Enable(int size)
{
    SendCommandFormat("vis %s,1", size ? "waveBig" : "waveLeft");
}

void Nextion::WaveInput::Disable(int size)
{
    SendCommandFormat("vis %s,0", size ? "waveBig" : "waveLeft");
}


void Nextion::WaveFFT::Enable(int size)
{
    Nextion::SendCommandFormat("vis %s,1", size ? "waveBig" : "waveRight");
}


void Nextion::WaveFFT::Disable(int size)
{
    Nextion::SendCommandFormat("vis %s,0", size ? "waveBig" : "waveRight");
}


void Nextion::Button::SetText(pchar name_button, pchar _text)
{
    Nextion::SendCommandFormat("%s.txt=\"%s\"", name_button, _text);
}


void Nextion::Button::Highligth(pchar name_button, bool val)
{
    uint color_val = val ? Color::ButtonPress.ToRaw() : Color::Background.ToRaw();

//    LOG_WRITE("%s higthligth = %d, color %d", name_button, (int)val, color_val);

    Nextion::SendCommandFormat("%s.bco=%d", name_button, color_val);
    Nextion::SendCommandFormat("%s.bco2=%d", name_button, color_val);
}


void Nextion::Button::Eanble(pchar name_button)
{
    Nextion::SendCommandFormat("vis %s,1", name_button);
}


void Nextion::Button::Disable(pchar name_button)
{
    Nextion::SendCommandFormat("vis %s,0", name_button);
}


void Nextion::SendByte(uint8 byte)
{
    LastCode::Set(ResponseCode::None);

    HAL_USART2::SendByte(byte);
}


void Nextion::SendCommandFormat(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommandRAW(message, true);
}


void Nextion::SendCommandFormatWithoutWaiting(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommandRAW(message, false);
}


void Nextion::SendCommandRAW(pchar command, bool wait)
{
    LastCode::Set(ResponseCode::None);

    HAL_USART2::SendNZ(command);

    HAL_USART2::SendNZ("\xFF\xFF\xFF");

    Profiler::AddCommand();

    if (wait)
    {
        WaitResponse(command, ResponseCode::InstructionSuccessful);
    }
}


void Nextion::WaitResponse(pchar command, ResponseCode::E code)
{
    TimeMeterMS meter;

    while (LastCode::Get() == ResponseCode::None)
    {
        Update();

        if (meter.ElapsedTime() > 200)
        {
            LOG_WRITE("No response received");

            break;
        }
    }

    if (LastCode::Get() != code)
    {
        LOG_WRITE("Error in %s : Received %02Xh but expected %02Xh", command, LastCode::Get(), code);
    }
}


void Nextion::Page::Enable(int number)
{
    SendCommandFormat("page %d", number);
}
