// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <limits>


namespace DiagramInput
{
    static const REAL height = 256;        // Таков размах по вре
    static const REAL y0 = 128;
    static uint time_next_draw = 0;         // Время следующей отрисовки картинки

    static uint8 points[1024];

    static int NumPoints();
}


void DiagramInput::SetData()
{
    REAL scale = height / (BufferADC::Max().Real() - BufferADC::Min().Real());
    REAL ave = (BufferADC::Max().Real() + BufferADC::Min().Real()) / 2.0;

    for (int i = 0; i < NumPoints(); i++)
    {
        REAL value = y0 + scale * (BufferADC::At(i).Real() - ave);

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


void DiagramInput::Draw()
{
    Nextion::WaveInput::Draw(0, points, NumPoints());
}


void DiagramInput::Repaint()
{
    time_next_draw = 0;
}


int DiagramInput::NumPoints()
{
    return 783;
}
