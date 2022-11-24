// 2022/10/28 11:17:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Indicator.h"
#include "Nextion/Nextion.h"
#include "Display/Colors.h"
#include "Display/DiagramInput.h"
#include "Display/DiagramFFT.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Controls/TextString.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/Communicator.h"
#include "Utils/String.h"
#include <cstdio>
#include <cstring>
#include <cmath>


namespace Indicator
{
    static const int small_x_label = 146;
    static const int small_y_0 = 220;
    static const int small_y_1 = 294;
    static const int small_width_label = 150;
    static const int small_height = 57;

    static const int small_x_text = 361;
    static const int small_width_text = 300;

    static WindowMeasureDC windowDC(38, 74, 5);
    static WindowMeasureAC windowAC(38, 236, 5);

    static TextString labelDCsmall(small_x_label, small_y_0, small_width_label, small_height, 0, "DC:", Color::MeasureDC);
    static TextString labelACsmall(small_x_label, small_y_1, small_width_label, small_height, 0, "AC:", Color::MeasureAC);
    static TextString textDCsmall(small_x_text, small_y_0, small_width_text, small_height, 0, "", Color::MeasureDC);
    static TextString textACsmall(small_x_text, small_y_1, small_width_text, small_height, 0, "", Color::MeasureAC);
    static TextString textPeakADC(10, 358, 200, 40, 2, "", Color::White);
    static TextString textAveADC(250, 358, 200, 40, 2, "", Color::White);

    static bool is_big = true;

    static char measureDC[TextString::MAX_LEN] = { '\0' };
    static char measureAC[TextString::MAX_LEN] = { '\0' };

    static int need_sendUSB = 0;                                       // Столько раз нужно передавать данные наружу
    static int need_sendRS232 = 0;

    static void SetBig();

    static void SetSmall();

    // before - количество цифр (без учёта знака) перед запятой, after - количество цифр после запятой
    static void ConvertDoubleToText(float value, char buffer[TextString::MAX_LEN], int after, pchar suffix);

    static void WriteMeasures();
}


void Indicator::Init()
{
    windowDC.Init();
    windowAC.Init();
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

    windowDC.Enable();
    windowAC.Enable();

    windowDC.SetMeasure(measureDC);
    windowAC.SetMeasure(measureAC);
}


void Indicator::SetSmall()
{
    is_big = false;

    windowDC.Disable();
    windowAC.Disable();

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


void Indicator::SetMeasures(float dc, float ac)
{
    int range = Range::Current();

    static const int after[6]    = { 4, 3, 2, 4, 3, 3 };
    const pchar suffix = (range < 3) ? "mA" : "A";

    ConvertDoubleToText(dc, measureDC, after[range], suffix);
    ConvertDoubleToText(ac, measureAC, after[range], suffix);

    if (need_sendUSB || need_sendRS232)
    {
        String<> messageDC("DC:%s", measureDC);
        String<> messageAC("AC:%s", measureAC + 1);

        if (need_sendUSB)
        {
            need_sendUSB--;

            Communicator::SendWith0D0A(Direction::USB, messageDC.c_str());
            Communicator::SendWith0D0A(Direction::USB, messageAC.c_str());
        }

        if (need_sendRS232)
        {
            need_sendRS232--;

            Communicator::SendWith0D0A(Direction::RS232, messageDC.c_str());
            Communicator::SendWith0D0A(Direction::RS232, messageAC.c_str());
        }
    }
}


void Indicator::SetOverflow()
{
    for (int i = 0; (i < TextString::MAX_LEN) && (measureDC[i] != '\0'); i++)
    {
        if (measureDC[i] != '.') { measureDC[i] = '^'; }
        if (measureAC[i] != '.') { measureAC[i] = '^'; }
    }
}


void Indicator::WriteMeasures()
{
    if (is_big)
    {
        windowDC.SetMeasure(measureDC);
        windowAC.SetMeasure(measureAC);
    }
    else
    {
        textDCsmall.SetText(measureDC);
        textACsmall.SetText(measureAC);
    }
}


void Indicator::ConvertDoubleToText(float value, char out[TextString::MAX_LEN], int after, pchar suffix)
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

    char buffer[TextString::MAX_LEN];

    char format[] = { '%', '0', (char)((before + 1) | 0x30), '.', (char)(after | 0x30), 'f', ' ', '%', 's', '\0'};

    std::sprintf(buffer, format, (double)value, suffix);

    std::strcat(out, buffer);
}


void Indicator::OnEvent::CnageRange()
{
    windowAC.OnEventChangeRange();
    windowDC.OnEventChangeRange();
}


void Indicator::OnEvent::SendDataToCommunicator(Direction::E dir, int num)
{
    if (dir & Direction::USB)
    {
        need_sendUSB = num;
    }

    if (dir & Direction::RS232)
    {
        need_sendRS232 = num;
    }
}


void Indicator::SetPeakADC(int
#ifdef LOGGED
    delta
#endif
)
{
#ifdef LOGGED
    textPeakADC.SetText(String<>("peak:%d", delta).c_str());
#endif
}


void Indicator::SetAveADC(int
#ifdef LOGGED
    delta
#endif
)
{
#ifdef LOGGED
    textAveADC.SetText(String<>("ave:%d", delta).c_str());
#endif
}
