// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <limits>


namespace DiagramInput
{
    static BufferADC data;

    static const float height = 256;        // Таков размах по вре
    static const float y0 = 128;
    static bool enabled = false;
    static uint time_next_draw = 0;         // Время следующей отрисовки картинки

    static bool NeedDraw();

    static int NumPoints();
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


bool DiagramInput::NeedDraw()
{
    if (HAL_TIM::TimeMS() < time_next_draw)
    {
        return false;
    }

    time_next_draw = HAL_TIM::TimeMS() + 1000;

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

    uint8 points[1024];

    for (int i = 0; i < NumPoints(); i++)
    {
        float value = y0 + scale * (data[i].Real() - ave);

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

    Nextion::WaveInput::Draw(points, NumPoints());
}


void DiagramInput::Enable(bool _enable)
{
    if (enabled == _enable)
    {
        return;
    }

    time_next_draw = 0;

    enabled = _enable;

    Nextion::WaveFFT::Disable(0);
    Nextion::WaveFFT::Disable(1);
    Nextion::WaveInput::Disable(0);

    if (enabled)
    {
        if (DiagramFFT::IsEnabled())
        {
            Nextion::WaveFFT::Enable(0);
            Nextion::WaveInput::Enable(0);
        }
        else
        {
            Nextion::WaveInput::Enable(1);
        }
    }
    else
    {
        if (DiagramFFT::IsEnabled())
        {
            Nextion::WaveFFT::Enable(1);
        }
    }
}


bool DiagramInput::IsEnabled()
{
    return enabled;
}


void DiagramInput::Repaint()
{
    time_next_draw = 0;
}


int DiagramInput::NumPoints()
{
    return (DiagramFFT::IsEnabled() && DiagramInput::IsEnabled()) ? 390 : 783;
}
