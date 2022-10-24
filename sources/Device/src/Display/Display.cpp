// 2022/10/17 13:48:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Display/DiagramInput.h"
#include "Display/DInterface.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>


void Display::SetDC(float value)
{
    char buffer[100];

    std::sprintf(buffer, "textDC.txt=\"%.4f mA\"\xFF\xFF\xFF", value);

    HAL_USART2::Send(buffer);
}


void Display::SetAC(float value)
{
    char buffer[100];

    std::sprintf(buffer, "ac.txt=\"%.4f mA\"\xFF\xFF\xFF", value);

    HAL_USART2::Send(buffer);
}


void Display::Update()
{
    static float voltageDC = 0.0f;

    voltageDC += 1.0f;

    Display::SetDC(voltageDC);

    Interface::SendCommand("waveInput.dis=77");

    static int last = 0;
    static int d = 1;

    Interface::SendCommandFormat("add 16,0,%d", last);
    last += d;
    if (last == 255 || last == 0)
    {
        d = -d;
    }
    Interface::SendCommandFormat("add 16,1,%d", last);
}


void Display::DrawData(const BufferADC &data)
{
    DiagramInput::SetData(data);

    DiagramInput::Draw();
}
