// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL.h"
#include <limits>


using namespace Primitives;


namespace DiagramInput
{
    static BufferADC data;

    static const int num_points = 390;           // Столько точек графика выводится
    static uint8 pixels[num_points];
    static const float height = 240;        // Таков размах по вре
    static const float y0 = 128;
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


void DiagramInput::Draw()
{
    float scale = height / (data.MaxReal() - data.MinReal());

    for (int i = 0; i < num_points; i++)
    {
        pixels[i] = (uint8)(y0 + scale * data.At(i));
    }

    Display::Interface::SendCommandFormat("addt 16,0,%d", num_points);

    HAL_TIM::Delay(50);

    for (int i = 0; i < num_points; i++)
    {
        Display::Interface::SendByte(pixels[i]);
    }
}
