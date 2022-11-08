// 2022/10/28 11:17:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Indicator.h"
#include "Nextion/Nextion.h"
#include "Display/Colors.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include <cstdio>
#include <cstring>
#include <cmath>


namespace Indicator
{
    static const int MAX_LEN = 32;

    // Неизменяемый текст
    struct Label
    {
        Label(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &);
        void Disable();
        void Enable();
        void SetText(const char [MAX_LEN]);
    private:
        char  text[MAX_LEN];
        int   x;
        int   y;
        int   width;
        int   height;
        int   font;
        Color color;
    };

    // Изменяемый текст
    struct Text : public Label
    {
        Text(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_color) :
         Label(_x, _y, _w, _h, _font, _text, _color) {}
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


    static Label labelDC(big_x_label, big_y_0, big_width_label, big_height, 7, "DC:", Color::MeasureDC);
    static Label labelDCsmall(small_x_label, small_y_0, small_width_label, small_height, 0, "DC:", Color::MeasureDC);
    static Label labelAC(big_x_label, big_y_1, big_width_label, big_height, 7, "AC:", Color::MeasureAC);
    static Label labelACsmall(small_x_label, small_y_1, small_width_label, small_height, 0, "AC:", Color::MeasureAC);

    static Text textDC(big_x_text, big_y_0, big_width_text, big_height, 7, "", Color::MeasureDC);
    static Text textDCsmall(small_x_text, small_y_0, small_width_text, small_height, 0, "", Color::MeasureDC);
    static Text textAC(big_x_text, big_y_1, big_width_text, big_height, 7, "", Color::MeasureAC);
    static Text textACsmall(small_x_text, small_y_1, small_width_text, small_height, 0, "", Color::MeasureAC);

    static Text textDeltaADC(7, 207, 150, 40, 3, "", Color::White);

    static bool is_big = true;

    static char measureDC[MAX_LEN] = { '\0' };
    static char measureAC[MAX_LEN] = { '\0' };

    static void SetBig();

    static void SetSmall();

    // before - количество цифр (без учёта знака) перед запятой, after - количество цифр после запятой
    static void ConvertDoubleToText(double value, char buffer[MAX_LEN], int after, pchar suffix);

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


void Indicator::SetMeasures(double dc, double ac, int range)
{
    static const int after[6]    = { 4, 3, 2, 4, 3, 3 };
    static const pchar suffix[6] = { "mA", "mA", "mA", "A", "A", "A" };

    ConvertDoubleToText(dc, measureDC, after[range], suffix[range]);
    ConvertDoubleToText(ac, measureAC, after[range], suffix[range]);

    WriteMeasures();
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


void Indicator::ConvertDoubleToText(double value, char out[MAX_LEN], int after, pchar suffix)
{
    std::strcpy(out, value < 0.0 ? "-" : "+");

    char buffer[MAX_LEN];

    char format[] = { '%', '0', (char)((6 - after) | 0x30), '.', (char)(after | 0x30), 'f', ' ', '%', 's', '\0'};

    std::sprintf(buffer, format, std::fabs(value), suffix);

    std::strcat(out, buffer);
}


void Indicator::Reset(int range)
{
    pchar message = nullptr;

    switch (range)
    {
    case 0:        message = "*.***** mA";        break;
    case 1:        message = "**.**** mA";        break;
    case 2:        message = "***.*** mA";        break;
    case 3:        message = "*.***** A";         break;
    case 4:        message = "**.**** A";         break;
    case 5:        message = "**.**** A";         break;
    }

    std::strcpy(measureDC, message);
    std::strcpy(measureAC, message);

    WriteMeasures();
}


void Indicator::SetDeltaADC(int delta)
{
    char buffer[32];
    std::sprintf(buffer, "%d", delta);
    textDeltaADC.SetText(buffer);
}


Indicator::Label::Label(int _x, int _y, int _w, int _h, int _font, pchar _text, const Color &_color) :
    x(_x), y(_y), width(_w), height(_h), font(_font), color(_color)
{
    std::strcpy(text, _text);
}


void Indicator::Label::Enable()
{
    Nextion::DrawString(x, y, width, height, font, color, Color::Background, text);
}


void Indicator::Label::Disable()
{
    Nextion::DrawString(x, y, width, height, font, color, Color::Background, "");
}


void Indicator::Label::SetText(const char _text[MAX_LEN])
{
    std::strcpy(text, _text);

    Enable();
}
