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
    Interface::SendCommandFormat("textDC.txt=\"%.4f mA\"", value);
    Interface::SendCommandFormat("textDCsmall.txt=\"%.4f mA\"", value);
}


void Display::SetAC(float value)
{
    Interface::SendCommandFormat("textAC.txt=\"%.4f mA\"", value);
    Interface::SendCommandFormat("textACsmall.txt=\"%.4f mA\"", value);
}


void Display::Update()
{
    static const int num_points = 25;

    Interface::SendCommandFormat("addt 16,0,%d", num_points);

    HAL_TIM::Delay(100);

    static int last = 0;
    static int d = 1;

    for (int i = 0; i < num_points; i++)
    {
        last += 3;
        last -= std::rand() % 6;
        Interface::SendByte((uint8)last);
        last += d;
        if (last == 255 || last == 0)
        {
            d = -d;
        }
    }
}


void Display::DrawData(const BufferADC &data)
{
    DiagramInput::SetData(data);

    DiagramInput::Draw();
}
