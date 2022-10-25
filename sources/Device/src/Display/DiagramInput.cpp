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
    static const float height = 256;        // Таков размах по вре
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


void DiagramInput::Draw()
{
    float scale = height / (data.MaxReal() - data.MinReal());
    float y0 = (data.MaxReal() + data.MinReal()) / 2.0f;

    Display::Interface::SendCommandFormat("addt 16,0,%d", num_points);

    HAL_TIM::Delay(50);

    for (int i = 0; i < num_points; i++)
    {
        float value = y0 + scale * data.At(i);

        if (value < 0)
        {
            value = 0;
        }
//        else if(value > 255)
//        {
//            value = 255;
//        }

        Display::Interface::SendByte((uint8)value);
    }
}
