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

    Interface::SendCommand("addt 17,0,400");

    HAL_TIM::Delay(20);

    Interface::SendCommand("waveInput.dis=50");

    for (int i = 0; i < 400; i++)
    {
        Interface::SendByte((uint8)i);
    }
}


void Display::DrawData(const BufferADC &data)
{
    DiagramInput::SetData(data);

    DiagramInput::Draw();
}
