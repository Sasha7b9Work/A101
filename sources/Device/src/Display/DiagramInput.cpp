// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <limits>


namespace DiagramInput
{
    bool NeedDraw();

    static BufferADC data;

    static const int num_points = 783;      // Столько точек графика выводится
    static const float height = 256;        // Таков размах по вре
    static const float y0 = 128;
    static bool enabled = false;
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


bool DiagramInput::NeedDraw()
{
    static uint next_time = 0;

    if (HAL_TIM::TimeMS() < next_time)
    {
        return false;
    }

    next_time = HAL_TIM::TimeMS() + 500;

    return true;
}


void DiagramInput::Draw()
{
    if (!IsEnabled() || !NeedDraw())
    {
        return;
    }

    float scale = height / (data.MaxReal() - data.MinReal());
    float ave = (data.MaxReal() + data.MinReal()) / 2.0f;

    uint8 points[num_points];

    for (int i = 0; i < num_points; i++)
    {
        float value = y0 + scale * (data.At(i) - ave);

        if (value < 0)
        {
            value = 0;
        }
        else if (value > 255)
        {
            value = 255;
        }

        points[i] = (uint8)value;
    }

    Painter::DrawWave(points, num_points);
}


void DiagramInput::Enable(bool _enable)
{
    enabled = _enable;
}


bool DiagramInput::IsEnabled()
{
    return enabled;
}
