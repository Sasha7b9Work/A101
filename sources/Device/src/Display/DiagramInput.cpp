// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <limits>


using namespace Primitives;


namespace DiagramInput
{
    static BufferADC data;

    static const int num_points = 100;      // Столько точек графика выводится
    static const float height = 256;        // Таков размах по вре
    static const float y0 = 128;
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}



void DiagramInput::Draw()
{
    TimeMeterMS meter;

    static uint next_time = 0;

    if (HAL_TIM::TimeMS() < next_time)
    {
        return;
    }

    next_time = HAL_TIM::TimeMS() + 250;

    float scale = height / (data.MaxReal() - data.MinReal());
    float ave = (data.MaxReal() + data.MinReal()) / 2.0f;

    DInterface::SendCommandFormat("addt 16,0,%d", num_points);

    while (DInterface::LastCode() != ReturnCodeDI::TransparentDataReady)
    {
        if (meter.ElapsedTime() > 200)
        {
            break;
        }
    }

    for(int i = 0; i < num_points; i++)
    {
        float value = y0 + scale * (data.At(i) - ave);

        if (value < 0)
        {
            value = 0;
        }
        else if(value > 255)
        {
            value = 255;
        }

        DInterface::SendByte((uint8)value);
    }

    meter.Reset();
    
    while (DInterface::LastCode() != ReturnCodeDI::TransparentDataFinished)
    {
        if(meter.ElapsedTime() > 200)
        {
            break;
        }
    }
}
