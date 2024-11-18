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
#include <cstdio>
#include <cstdlib>


namespace DiagramInput
{
    static const int height = 368;
    static const int y0 = 295;
    static char zero_AC[32] = { '\0' };
    static char delta_AC[32] = { '\0' };

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

    // Преобразовать value к порядку order и вернуть мантиссу
    static REAL ConvertMantissaToOrder(const REAL value, const int order);

    static void ConvertZeroACToASCII(REAL value, char buffer[32]);

    static void ConvertDeltaACToASCII(REAL mantissa, int order, char buffer[32]);
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


bool DiagramInput::InstallSignalAC()
{
    bool correct = false;

    REAL dc = Calculator::GetAbsDC(&correct);

    ConvertZeroACToASCII(dc, zero_AC);

    if (!correct)
    {
        return false;
    }

    int range = Range::Current();
    REAL k = cal.gain[range].Get();

    REAL min = std::numeric_limits<REAL>::max();
    REAL max = -std::numeric_limits<REAL>::max();

    const int start_i = 10;

    {                                                          // Находим минимальное и масимальное значения на отрезке
        for (int i = 0; i < NUM_POINTS; i++)
        {
            REAL value_abs = BufferADC::At(i + start_i).Real() * k;

            if (range > 2)
            {
                value_abs *= 1e3;
            }

            value_abs += dc;

            if (value_abs < min)
            {
                min = value_abs;
            }

            if (value_abs > max)
            {
                max = value_abs;
            }
        }
    }

    REAL amplitude = max - min;                                 // Размах переменного напряжения

    if (amplitude == 0.0f)
    {
        return false;
    }

    REAL mantissa = 0.0;
    int order = 0;

    if (!GetMantissaOrder(amplitude, &mantissa, &order))
    {
        return false;
    }

    REAL scale_max_AC = 0.0;

    if (mantissa < 2.0)         // Размах до 2
    {
        scale_max_AC = 2.0;
    }
    else if (mantissa < 5.0)    // Размах до 5
    {
        scale_max_AC = 5.0;
    }
    else                        // Размах до 10
    {
        scale_max_AC = 10.0;
    }

    ConvertDeltaACToASCII(scale_max_AC, order, delta_AC);

    volatile REAL scale = height / scale_max_AC;

    for (int i = 0; i < NUM_POINTS; i++)
    {
        volatile REAL value_abs = BufferADC::At(i + start_i).Real() * k;

        if (range > 2)
        {
            value_abs *= 1e3;
        }

        value_abs += dc;

        int16 value_int = (int16)(scale * ConvertMantissaToOrder(value_abs, order));

        Math::Limitation<int16>(&value_int, -height / 2, height / 2);

        points[i] = (uint16)(y0 + value_int);
    }

    return true;
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

    if (!set.type_signal.IsFull())
    {
        delta_AC[0] = '+';

        Nextion::DrawString({ 0, y0 - height / 2 + 1, 150, 34 }, Font::_0_GB34b, Color::White, Color::Background, delta_AC, false, false);

        delta_AC[0] = '-';

        Nextion::DrawString({ 0, y0 + height / 2 - 35, 150, 34 }, Font::_0_GB34b, Color::White, Color::Background, delta_AC, false, false);

        Nextion::DrawString({ 0, y0 - 34, 150, 34 }, Font::_0_GB34b, Color::White, Color::Background, zero_AC, false, false);
    }

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


REAL DiagramInput::ConvertMantissaToOrder(const REAL _value, const int _order)
{
    REAL mantissa = 0.0f;
    int order = 0;

    GetMantissaOrder(_value, &mantissa, &order);

    int delta = order - _order;

    while (delta > 0)
    {
        mantissa *= 10.0;
        delta--;
    }

    while (delta < 0)
    {
        mantissa *= 0.1;
        delta++;
    }

    return mantissa;
}


void DiagramInput::ConvertDeltaACToASCII(REAL mantissa, int order, char buffer[32])
{
    while (order > 0)
    {
        mantissa *= 10.0;
        order--;
    }

    while (order < 0)
    {
        mantissa *= 0.1;
        order++;
    }

    ConvertZeroACToASCII(mantissa, buffer + 1);
}


void DiagramInput::ConvertZeroACToASCII(REAL value, char buffer[32])
{
    value *= 1e-3;

    if (value >= 1e3)
    {
        sprintf(buffer, "%.2f kA", value / 1e3);
    }
    else if (value >= 1e2)
    {
        sprintf(buffer, "%.0f A", value);
    }
    else if (value >= 1e1)
    {
        sprintf(buffer, "%.1f A", value);
    }
    else if (value >= 1e0)                  // > 1A
    {
        sprintf(buffer, "%.2f A", value);
    }
    else if (value >= 1e-1)                 // > 100 mA
    {
        sprintf(buffer, "%.0f mA", value * 1e3);
    }
    else if (value >= 1e-2)                 // > 10 mA
    {
        sprintf(buffer, "%.1f mA", value * 1e3);
    }
    else if (value >= 1e-3)                  // > 1 mA
    {
        sprintf(buffer, "%.2f mA", value * 1e3);
    }
    else if (value >= 1e-4)                 // > 100 uA
    {
        sprintf(buffer, "%.0f uA", value * 1e6);
    }
    else if (value >= 1e-5)                 // > 10 uA
    {
        sprintf(buffer, "%.1f uA", value * 1e6);
    }
    else if (value >= 1e-6)                 // > 1 uA
    {
        sprintf(buffer, "%.2f uA", value * 1e6);
    }
    else if (value >= 1e-7)                  // > 100 nA
    {
        sprintf(buffer, "%.0f nA", value * 1e9);
    }
    else if (value >= 1e-8)                 // > 10 nA
    {
        sprintf(buffer, "%.1f nA", value * 1e9);
    }
    else if(value >= 1e-9)                  // > 1 na
    {
        sprintf(buffer, "%.2f nA", value * 1e9);
    }
    else
    {
        std::strcpy(buffer, "0 nA");
    }
}
