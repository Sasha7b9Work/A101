// 2022/10/28 11:17:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Indicator.h"
#include "Nextion/Nextion.h"
#include "Display/Colors.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Hardware/HAL/HAL.h"
#include <cstdio>
#include <cstring>
#include <cmath>


namespace Indicator
{
    static const int MAX_LEN = 32;

    struct TextString
    {
        TextString(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_colorText, const Color &_colorBack = Color::Count);
        void Disable();
        void Enable();
        void SetText(const char[MAX_LEN]);
    private:
        char  text[MAX_LEN];
        int   x;
        int   y;
        int   width;
        int   height;
        int   font;
        Color colorText;
        Color colorBack;
    };


    static const int big_x_label = 38;
    static const int big_y_0 = 74;
    static const int big_y_1 = 236;
    static const int big_width_label = 175;
    static const int big_height = 95;

    static const int big_x_text = 220;
    static const int big_width_text = 550;

    static const int small_x_label = 146;
    static const int small_y_0 = 220;
    static const int small_y_1 = 294;
    static const int small_width_label = 150;
    static const int small_height = 57;

    static const int small_x_text = 361;
    static const int small_width_text = 300;


    static TextString labelDC(big_x_label, big_y_0, big_width_label, big_height, 7, "DC:", Color::MeasureDC, Color::ButtonPress);
    static TextString labelDCsmall(small_x_label, small_y_0, small_width_label, small_height, 0, "DC:", Color::MeasureDC);
    static TextString labelAC(big_x_label, big_y_1, big_width_label, big_height, 7, "AC:", Color::MeasureAC, Color::ButtonPress);
    static TextString labelACsmall(small_x_label, small_y_1, small_width_label, small_height, 0, "AC:", Color::MeasureAC);

    static TextString textDC(big_x_text, big_y_0, big_width_text, big_height, 7, "", Color::MeasureDC, Color::ButtonPress);
    static TextString textDCsmall(small_x_text, small_y_0, small_width_text, small_height, 0, "", Color::MeasureDC);
    static TextString textAC(big_x_text, big_y_1, big_width_text, big_height, 7, "", Color::MeasureAC, Color::ButtonPress);
    static TextString textACsmall(small_x_text, small_y_1, small_width_text, small_height, 0, "", Color::MeasureAC);

    static TextString textDeltaADC(7, 207, 150, 40, 3, "", Color::White);

    static bool is_big = true;

    static char measureDC[MAX_LEN] = { '\0' };
    static char measureAC[MAX_LEN] = { '\0' };

    static void SetBig();

    static void SetSmall();

    // before - количество цифр (без учёта знака) перед запятой, after - количество цифр после запятой
    static void ConvertDoubleToText(float value, char buffer[MAX_LEN], int after, pchar suffix);

    static void WriteMeasures();
}


void Indicator::AutoSize()
{
    if (DiagramInput::IsEnabled() || DiagramFFT::IsEnabled())
    {
        SetSmall();
    }
    else
    {
        SetBig();
    }
}


void Indicator::SetBig()
{
    is_big = true;

    labelDCsmall.Disable();
    labelACsmall.Disable();
    textDCsmall.Disable();
    textACsmall.Disable();

    labelDC.Enable();
    labelAC.Enable();
    textDC.Enable();
    textAC.Enable();

    textDC.SetText(measureDC);
    textAC.SetText(measureAC);

    labelAC.Enable();
}


void Indicator::SetSmall()
{
    is_big = false;

    labelDC.Disable();
    labelAC.Disable();
    textDC.Disable();
    textAC.Disable();

    labelDCsmall.Enable();
    labelACsmall.Enable();
    textDCsmall.Enable();
    textACsmall.Enable();

    textDCsmall.SetText(measureDC);
    textACsmall.SetText(measureAC);
}


void Indicator::Update()
{
    static uint next_time = 0;

    if (HAL_TIM::TimeMS() < next_time)
    {
        return;
    }

    next_time += 500;

    WriteMeasures();
}


void Indicator::SetMeasures(float dc, float ac, int range)
{
    static const int after[6]    = { 4, 3, 2, 4, 3, 3 };
    static const pchar suffix[6] = { "mA", "mA", "mA", "A", "A", "A" };

    ConvertDoubleToText(dc, measureDC, after[range], suffix[range]);
    ConvertDoubleToText(ac, measureAC, after[range], suffix[range]);
}


void Indicator::WriteMeasures()
{
    if (is_big)
    {
        textDC.SetText(measureDC);
        textAC.SetText(measureAC);
    }
    else
    {
        textDCsmall.SetText(measureDC);
        textACsmall.SetText(measureAC);
    }
}


void Indicator::ConvertDoubleToText(float value, char out[MAX_LEN], int after, pchar suffix)
{
    std::strcpy(out, value < 0.0f ? "-" : "+");

    value = std::fabs(value);

    int before = 5 - after;

    if (before == 3)
    {
        if (value < 10.0f)
        {
            std::strcat(out, "00");
        }
        else if (value < 100.0f)
        {
            std::strcat(out, "0");
        }
    }

    if (before == 2 && value < 10.0f)
    {
        std::strcat(out, "0");
    }

    char buffer[MAX_LEN];

    char format[] = { '%', '0', (char)((before + 1) | 0x30), '.', (char)(after | 0x30), 'f', ' ', '%', 's', '\0'};

    std::sprintf(buffer, format, (double)value, suffix);

    std::strcat(out, buffer);
}


void Indicator::Reset(int range)
{
    pchar message = nullptr;

    switch (range)
    {
    case 0:     message = "*.***** mA";     break;
    case 1:     message = "**.**** mA";     break;
    case 2:     message = "***.*** mA";     break;
    case 3:     message = "*.***** A";      break;
    case 4:
    case 5:     message = "**.**** A";      break;
    default:    message = "***.*** *";      break;
    }

    std::strcpy(measureDC, message);
    std::strcpy(measureAC, message);

    WriteMeasures();
}


void Indicator::SetDeltaADC(int delta)
{
    char buffer[32];
    std::sprintf(buffer, "%d", delta);
//    textDeltaADC.SetText(buffer);
}


Indicator::TextString::TextString(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_colorText, const Color &_colorBack) :
    x(_x), y(_y), width(_w), height(_h), font(_font), colorText(_colorText), colorBack(_colorBack)
{
    std::strcpy(text, _text);
}


void Indicator::TextString::Enable()
{
    Nextion::DrawString(x, y, width, height, font, colorText, (colorBack.value == Color::Count.value) ? Color::Background : Color::ButtonPress, text);
}


void Indicator::TextString::Disable()
{
    Nextion::DrawString(x, y, width, height, font, colorText, Color::Background, "");
}


void Indicator::TextString::SetText(const char _text[MAX_LEN])
{
    if (std::strcmp(_text, text) == 0)
    {
        return;
    }

    std::strcpy(text, _text);

    Enable();
}
