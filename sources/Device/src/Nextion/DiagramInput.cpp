// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Display.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include <limits>


namespace DiagramInput
{
    static const int height = 368;
    static const int y0 = 295;

    static const int NUM_POINTS = Display::WIDTH;

    static uint16 points[NUM_POINTS];       // Здесь точки в координатах экрана

    static bool data_installed = false;     // Признак того, что данные для отрисовки установлены
    static int first_point = 0;             // С этой точки начнём отрисовку в следующий раз
    static int elapsed_point = NUM_POINTS;  // Столько точек осталось отрисовать

    static void DrawCoordinateAxes();
}


void DiagramInput::InstallData()
{
    if (Page::Current() != PageGraph::self || data_installed)
    {
        return;
    }

    data_installed = true;

    DrawCoordinateAxes();

    int range = Range::Current();

    REAL scale = height / Measure::MaxIAbs(range) / 2;

    REAL k = cal.gain[range].Get();

    for (int i = 0; i < NUM_POINTS; i++)
    {
        REAL value_abs = BufferADC::At(i).Real() * k;

        if (range > 2)
        {
            value_abs *= 1e3;
        }

        int16 value_int = (int16)(scale * value_abs);

        if (value_int < -height / 2)
        {
            value_int = -height / 2;
        }

        if (value_int > height / 2)
        {
            value_int = height / 2;
        }

        points[i] = (uint16)(y0 + value_int);
    }
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

    Rect rect{ first_point, y0 - height / 2,  num_points, height };

    Nextion::WaveInput::Draw(rect, points + first_point);

    first_point += num_points;

    if (elapsed_point == 0)
    {
        Reset();
    }
}


void DiagramInput::InstallData2(float data[1024])
{
    if (data_installed)
    {
        return;
    }

    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

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

    for (int i = 0; i < NUM_POINTS; i++)
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


bool DiagramInput::DataInstalled()
{
    return data_installed;
}


void DiagramInput::Reset()
{
    data_installed = false;
    first_point = 0;
    elapsed_point = NUM_POINTS;
}

void DiagramInput::DrawCoordinateAxes()
{
    Nextion::DrawRect({0, y0 - height / 2, Display::WIDTH - 1, height}, Color::Gray75);

    Nextion::DrawLineH(y0, 0, Display::WIDTH);
}
