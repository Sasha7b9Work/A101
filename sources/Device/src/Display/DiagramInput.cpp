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
    static const double height = 256;        // Таков размах по вре
    static const double y0 = 128;
    static bool enabled = false;
    static uint time_next_draw = 0;         // Время следующей отрисовки картинки

    static uint8 points[1024];

    static bool NeedDraw();

    static int NumPoints();
}


void DiagramInput::SetData()
{
    double scale = height / (BufferADC::Max().Real() - BufferADC::Min().Real());
    double ave = (BufferADC::Max().Real() + BufferADC::Min().Real()) / 2.0;

    for (int i = 0; i < NumPoints(); i++)
    {
        double value = y0 + scale * (BufferADC::At(i).Real() - ave);

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

//    int index = 0;
//
//    int points_in_point = BufferADC::SIZE / NumPoints() - 1;
//
//    for (int i = 0; i < NumPoints(); i++)
//    {
//        float sum_points = 0.0f;
//
//        for (int j = 0; j < points_in_point; j++)
//        {
//            index = j + i * points_in_point;
//            if (index < BufferADC::SIZE)
//            {
//                sum_points += BufferADC::At(index).Real();
//            }
//        }
//
//        float value = sum_points / points_in_point;
//
//        value = y0 + scale * (value - ave);
//
//        if (value < 0)
//        {
//            value = 0;
//        }
//        else if (value > 255)
//        {
//            value = 255;
//        }
//
//        points[i] = (uint8)value;
//    }
//
//    LOG_WRITE("index %d", index);
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
