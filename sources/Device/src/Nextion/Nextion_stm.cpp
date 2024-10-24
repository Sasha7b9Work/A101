// 2022/10/31 19:37:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Nextion.h"
#include "Nextion/DiagramInput.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Profiler.h"
#include "Nextion/Display.h"
#include <cstdarg>
#include <cstdio>
#include <cstring>


namespace Nextion
{
    static void SendCommandRAW(pchar);

    // ������� ����������� ��� ��������� ����
    void WaitResponse(pchar, ResponseCode::E);

    static pchar Convert(pchar text)
    {
        static uint8 buffer[32];

        uint8 *pointer = buffer;

        while (*text)
        {
            *pointer = (uint8)*text;

            if (*pointer > 127)
            {
                *pointer = (uint8)((*pointer) - 16);
            }

            text++;
            pointer++;
        }

        *pointer = 0;

        return (pchar)buffer;
    }
}


void Nextion::DrawRect(const Rect &rect, const Color &color)
{
    color.SetAsCurrent();

    SendCommandFormat("draw %d,%d,%d,%d,%s", rect.x, rect.y, rect.x + rect.width, rect.y + rect.height, Color::Current().GetValue());
}


void Nextion::FillRect(const Rect &rect, const Color & color)
{
    color.SetAsCurrent();

    SendCommandFormat("fill %d,%d,%d,%d,%s", rect.x, rect.y, rect.width, rect.height, Color::Current().GetValue());
}


void Nextion::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    color.SetAsCurrent();

    SendCommandFormat("line %d,%d,%d,%d,%s", x1, y1, x2, y2, Color::Current().GetValue());
}


void Nextion::DrawLineWhite(int x1, int y1, int x2, int y2)
{
    char buffer[128];

    std::sprintf(buffer, "line %d,%d,%d,%d,65535", x1, y1, x2, y2);

    SendCommandRAW(buffer);
}


void Nextion::DrawString(const Rect &rect, int font, const Color &color, const Color &back_color, pchar text, bool h_align, bool v_align)
{
    SendCommandFormat("xstr %d,%d,%d,%d,%d,%s,%s,%d,%d,1,\"%s\"",
        rect.x, rect.y, rect.width, rect.height, font, color.GetValue(), back_color.GetValue(), h_align ? 1 : 0, v_align ? 1 : 0, Convert(text));
}


void Nextion::WaveInput::Draw(const Rect &rect, uint16 *points)
{
    Nextion::FillRect(rect, Color::Background);

    const int num_points = rect.width;
    const int x = rect.x;

    Nextion::DrawLineH(rect.y, rect.x, rect.x + rect.width, Color::Gray75);
    Nextion::DrawLineH(rect.y + rect.height / 2, rect.x, rect.x + rect.width);
    Nextion::DrawLineH(rect.y + rect.height, rect.x, rect.x + rect.width);

    for (int i = 1; i < num_points; i++)
    {
        Nextion::DrawLineWhite(
            x + i - 1,
            points[i - 1],
            x + i,
            points[i]);
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


void Nextion::SendCommandFormat(const char *format, ...)
{
    char message[256];

    std::va_list args;
    va_start(args, format);
    std::vsprintf(message, format, args);
    va_end(args);

    SendCommandRAW(message);
}


void Nextion::SendCommandRAW(pchar command)
{
    LastCode::Set(ResponseCode::None);

    HAL_USART2::SendNZ(command);

    HAL_USART2::SendNZ("\xFF\xFF\xFF");

    Profiler::AddCommand();
}


void Nextion::WaitResponse(pchar
#ifdef LOGGED
    command
#endif
    , ResponseCode::E
#ifdef LOGGED
    code
#endif
)
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

#ifdef LOGGED
    if (LastCode::Get() != code)
    {
        LOG_WRITE("Error in %s : Received %02Xh but expected %02Xh", command, LastCode::Get(), code);
    }
#endif
}


void Nextion::Page::Enable(int number)
{
    SendCommandFormat("page %d", number);
}
