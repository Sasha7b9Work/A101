// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Nextion/Nextion.h"
#include "Nextion/Display.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/Calculator/Calculator.h"
#include <limits>
//#include <string>
#include <cstdio>
#include <cstdlib>


namespace DiagramInput
{
    static const int height = 368;
    static const int y0 = 295;

    static const int NUM_POINTS = Display::WIDTH;

    static uint16 points[NUM_POINTS];       // Здесь точки в координатах экрана

    static bool data_installed = false;     // Признак того, что данные для отрисовки установлены
    static int first_point = 0;             // С этой точки начнём отрисовку в следующий раз
    static int elapsed_point = NUM_POINTS;  // Столько точек осталось отрисовать

    static void Clear();

    static bool InstallSignal();
    static bool InstallSignalFull();
    static bool InstallSignalAC();
    static bool InstallFFT();

    static void DrawSignal();
    static void DrawFFT();

    // Возвращет мантиссу и порядок числа value
    static bool GetMantissaOrder(REAL value, REAL *mantissa, int *order);
}


void DiagramInput::InstallData()
{
    if (Page::Current() != PageGraph::self || data_installed)
    {
        return;
    }

    data_installed = false;

    if (set.type_graph.IsSignal())
    {
        data_installed = InstallSignal();
    }
    else
    {
        data_installed = InstallFFT();
    }
}


bool DiagramInput::InstallSignal()
{
    if (set.type_signal.IsFull())
    {
        return InstallSignalFull();
    }
    else
    {
        return InstallSignalAC();
    }
}


bool DiagramInput::InstallSignalFull()
{
    int range = Range::Current();

    REAL scale = height / Measure::MaxIAbs(range) / 3.0;

    REAL k = cal.gain[range].Get();

    for (int i = 0; i < NUM_POINTS; i++)
    {
        REAL value_abs = BufferADC::At(i).Real() * k;

        if (range > 2)
        {
            value_abs *= 1e3;
        }

        int16 value_int = (int16)(scale * value_abs);

        Math::Limitation<int16>(&value_int, -height / 2, height / 2);

        points[i] = (uint16)(y0 + value_int);
    }

    return true;
}


bool DiagramInput::InstallSignalAC()
{
    bool correct = false;

    REAL dc = Calculator::GetAbsDC(&correct);

    if (!correct)
    {
        return false;
    }

    int range = Range::Current();
//    REAL scale = height / Measure::MaxIAbs(range) / 3.0;
    REAL k = cal.gain[range].Get();

    REAL min = std::numeric_limits<REAL>::max();
    REAL max = std::numeric_limits<REAL>::min();

    {                                                          // Находим минимальное и масимальное значения на отрезке
        for (int i = 0; i < NUM_POINTS; i++)
        {
            REAL value_abs = BufferADC::At(i).Real() * k + dc;

            if (range > 2)       { value_abs *= 1e3; }

            Math::Limitation(&value_abs, min, max);
        }
    }

    REAL amplitude = max - min;                                 // Размах переменного напряжения

    if (amplitude == 0.0f)
    {
        return false;
    }

    REAL mantissa = 0.0;
    int order = 0;

    GetMantissaOrder(amplitude, &mantissa, &order);



    return true;
}


bool DiagramInput::InstallFFT()
{
    ResolverFFT resolver(1);

    REAL scale = (REAL)height / 255.0;

    for (int i = 0; i < NUM_POINTS; i++)
    {
        uint16 value = (uint16)(resolver.At(i) * scale);
        points[i] = value;
    }

    return true;
}


void DiagramInput::Draw()
{
    if (!data_installed)
    {
        return;
    }

    if (set.type_graph.IsSignal())
    {
        DrawSignal();
    }
    else
    {
        DrawFFT();
    }
}


void DiagramInput::DrawSignal()
{
    int num_points = 50;

    if (num_points > elapsed_point)
    {
        num_points = elapsed_point;
    }

    elapsed_point -= num_points;

    Rect rect{
        (first_point == 0) ? first_point : (first_point - 1),
        y0 - height / 2,
        (first_point == 0) ? num_points : (num_points + 1),
        height };

    Nextion::WaveInput::Draw(rect, points + (first_point == 0 ? first_point : (first_point - 1)));

    first_point += num_points;

    if (elapsed_point == 0)
    {
        Reset(false);
    }
}


void DiagramInput::DrawFFT()
{
    int num_points = 50;

    if (num_points > elapsed_point)
    {
        num_points = elapsed_point;
    }

    elapsed_point -= num_points;

    for (int i = 0; i < num_points; i++)
    {
        Nextion::DrawLineV(first_point + 1, Display::HEIGHT -1, Display::HEIGHT - height, Color::Background);
        Nextion::DrawLineV(first_point + 1, Display::HEIGHT -1, Display::HEIGHT - points[first_point], Color::White);
        first_point++;
    }

    char buffer[32];
    if (set.lang == Lang::RU)
    {
        sprintf(buffer, "%02.0f Дб", ResolverFFT::minDB);
    }
    else
    {
        sprintf(buffer, "%02.0f Db", ResolverFFT::minDB);
    }

    const int w = 80;
    const int h = 30;

    Nextion::DrawString({ Display::WIDTH - w - 5, Display::HEIGHT - h - 5, w, h }, Font::_5_GB30b, Color::White, Color::Background, buffer, true, true);

    if (elapsed_point == 0)
    {
        Reset(false);
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


void DiagramInput::Reset(bool clear)
{
    data_installed = false;
    first_point = 0;
    elapsed_point = NUM_POINTS;

    if (clear)
    {
        Clear();
    }
}

void DiagramInput::Clear()
{
    Nextion::FillRect({ 0, y0 - height / 2, Display::WIDTH - 1, height }, Color::Background);
}


bool DiagramInput::GetMantissaOrder(REAL value, REAL *mantissa, int *order)
{
    char buffer[32];

    sprintf(buffer, "%E", value);

    char *pointer_E = std::strchr(buffer, 'E');

    if (pointer_E)
    {
        *order = std::atoi(pointer_E + 1);

        *pointer_E = '\0';

        *mantissa = std::strtod(buffer, nullptr);

        return true;
    }

    return false;
}
