// 2022/10/28 11:17:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Indicator.h"
#include "Display/DInterface.h"
#include <cstdio>
#include <cstring>


namespace Indicator
{
    static const int MAX_LEN = 32;

    // Неизменяемый текст
    struct Label
    {
        Label(pchar _name_control, pchar _text);
        void Disable();
        void Enable();
        void SetText(char [MAX_LEN]);
    private:
        pchar name_control;
        char text[MAX_LEN];
    };

    static Label labelDC("labelDC", "DC:");
    static Label labelDCsmall("labelDCsmall", "DC:");
    static Label labelAC("labelAC", "AC:");
    static Label labelACsmall("lableACsmall", "AC:");

    static Label textDC("textDC", "");
    static Label textDCsmall("textDCsmall", "");
    static Label textAC("textAC", "");
    static Label textACsmall("textACsmall", "");

    static bool is_big = true;

    static char measureDC[MAX_LEN] = { '\0' };
    static char measureAC[MAX_LEN] = { '\0' };
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

    DInterface::SendCommandFormat(
        "xstr 261,236,530,111,7,65535,15319,0,0,1,\"%s\"", measureAC
    );

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


Indicator::Label::Label(pchar _name_control, pchar _text) :
    name_control(_name_control)
{
    std::strcpy(text, _text);
}


void Indicator::Label::Enable()
{
    DInterface::SendCommandFormat("%s.txt=\"%s\"", name_control, text);
}


void Indicator::Label::Disable()
{
    DInterface::SendCommandFormat("%s.txt=\"\"", name_control);
}


void Indicator::Label::SetText(char _text[MAX_LEN])
{
    std::strcpy(text, _text);

    Enable();
}
