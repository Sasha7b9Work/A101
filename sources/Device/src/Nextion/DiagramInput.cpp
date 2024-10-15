// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include <limits>


namespace DiagramInput
{
    static const REAL height = 256;         // Таков размах по высоте
    static const REAL y0 = 128;

    static uint8 points[1024];

    static int NumPoints();

    static bool data_installed = false;     // Признак того, что данные для отрисовки установлены
    static int first_point = 0;             // С этой точки начнём отрисовку в следующий раз
    static int elapsed_point = NumPoints(); // Столько точек осталось отрисовать
}


void DiagramInput::InstallData(int data[1024])
{
    if (data_installed)
    {
        return;
    }

    int min = std::numeric_limits<int>::max();
    int max = std::numeric_limits<int>::min();

    for (int i = 0; i < 1024; i++)
    {
        if (data[i] < min)
        {
            min = data[i];
        }
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    REAL scale = height / (max - min);
    REAL ave = (max + min) / 2.0f;

    for (int i = 0; i < NumPoints(); i++)
    {
        REAL value = y0 + scale * (data[i] - ave);

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

    data_installed = true;
}


void DiagramInput::_InstallData()
{
    if (data_installed)
    {
        return;
    }

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

    data_installed = true;

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
    if (!data_installed)
    {
        return;
    }

    int num_points = 50;

    if (num_points > elapsed_point)
    {
        num_points = elapsed_point;
    }

    elapsed_point -= num_points;

    Nextion::WaveInput::Draw(first_point, points + first_point, num_points);

    first_point += num_points;

    if (elapsed_point == 0)
    {
        Reset();
    }
}


bool DiagramInput::DataInstalled()
{
    return data_installed;
}


void DiagramInput::Reset()
{
    data_installed = false;
    first_point = 0;
    elapsed_point = NumPoints();
}


int DiagramInput::NumPoints()
{
    return 783;
}
