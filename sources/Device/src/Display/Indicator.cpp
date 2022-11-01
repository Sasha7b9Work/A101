// 2022/10/28 11:17:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Indicator.h"
#include "Nextion/Nextion.h"
#include "Display/Colors.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include <cstdio>
#include <cstring>


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

    static const int big_x_label = 38;
    static const int big_y_0 = 74;
    static const int big_y_1 = 236;
    static const int big_width_label = 175;
    static const int big_height = 95;

    static const int big_x_text = 261;
    static const int big_width_text = 530;

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

    static Label textDC(big_x_text, big_y_0, big_width_text, big_height, 7, "", Color::MeasureDC);
    static Label textDCsmall(small_x_text, small_y_0, small_width_text, small_height, 0, "", Color::MeasureDC);
    static Label textAC(big_x_text, big_y_1, big_width_text, big_height, 7, "", Color::MeasureAC);
    static Label textACsmall(small_x_text, small_y_1, small_width_text, small_height, 0, "", Color::MeasureAC);

    static Label textDeltaADC(7, 207, 150, 40, 3, "", Color::White);

    static bool is_big = true;

    static char measureDC[MAX_LEN] = { '\0' };
    static char measureAC[MAX_LEN] = { '\0' };

    void SetBig();

    void SetSmall();
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


void Indicator::SetMeasures(float dc, float ac)
{
    std::sprintf(measureDC, "%.4f V", (double)dc);
    std::sprintf(measureAC, "%.4f V", (double)ac);

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
