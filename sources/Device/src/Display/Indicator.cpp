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
#include "Hardware/Timer.h"
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

    static WindowMeasureDC windowDC;
    static WindowMeasureAC windowAC;

    static TextString labelDCsmall(small_x_label, small_y_0, small_width_label, small_height, 0, "DC:", Color::MeasureDC);
    static TextString labelACsmall(small_x_label, small_y_1, small_width_label, small_height, 0, "AC:", Color::MeasureAC);
    static TextString textDCsmall(small_x_text, small_y_0, small_width_text, small_height, 0, "", Color::MeasureDC);
    static TextString textACsmall(small_x_text, small_y_1, small_width_text, small_height, 0, "", Color::MeasureAC);
    static TextString textPeakADC(10, 358, 215, 40, 2, "", Color::White);
    static TextString textAveADC(230, 358, 180, 40, 2, "", Color::White);
    static TextString textMinADC(412, 358, 190, 40, 2, "", Color::White);
    static TextString textMaxADC(605, 358, 190, 40, 2, "", Color::White);
    static TextString textAvePicADC(10, 10, 250, 40, 2, "", Color::White);

    static bool is_big = true;

    static char measureDC[TextString::MAX_LEN] = { '\0' };
    static char measureAC[TextString::MAX_LEN] = { '\0' };

    static void SetBig();

    static void SetSmall();

    // before - количество цифр (без учёта знака) перед запятой, after - количество цифр после запятой
    static void ConvertDoubleToText(float value, char buffer[TextString::MAX_LEN], int after, pchar suffix);

    static void WriteMeasures();

    namespace NeedSend
    {
        static int USB = 0;     // Столько раз нужно передавать данные наружу
        static int RS232 = 0;
    }
}


void Indicator::Init()
{
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

    windowDC.SetMeasure(measureDC);
    windowAC.SetMeasure(measureAC);
}


void Indicator::SetSmall()
{
    is_big = false;

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
    static TimeMeterMS meter;

    if (meter.ElapsedTime() < 500)
    {
        return;
    }

    meter.Reset();

    int range = Range::Current();

    static const int after[6]    = { 4, 3, 2, 4, 3, 3 };
    const pchar suffix = (range < 3) ? "mA" : "A";

    ConvertDoubleToText(dc, measureDC, after[range], suffix);
    ConvertDoubleToText(ac, measureAC, after[range], suffix);

    if (NeedSend::USB || NeedSend::RS232)
    {
        String<> messageDC("DC:%s", measureDC);
        String<> messageAC("AC:%s", measureAC + 1);

        if (NeedSend::USB)
        {
            NeedSend::USB--;

            Communicator::SendWith0D0A(Direction::USB, messageDC.c_str());
            Communicator::SendWith0D0A(Direction::USB, messageAC.c_str());
        }

        if (NeedSend::RS232)
        {
            NeedSend::RS232--;

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

    // Отбрасываем цифры, которых нет на экране - во избежание бага, когда 10 превращеется в 1 (при 9.999999)
    {
        for (int i = 0; i < after; i++)
        {
            value *= 10.0f;
        }

        value = (float)((int)value);

        for (int i = 0; i < after; i++)
        {
            value /= 10.0f;
        }
    }

    char buffer[TextString::MAX_LEN];

    char format[] = { '%', '0', (char)((before + 1) | 0x30), '.', (char)(after | 0x30), 'f', ' ', '%', 's', '\0'};

    std::sprintf(buffer, format, (double)value, suffix);

    std::strcat(out, buffer);
}


void Indicator::OnEvent::CnageRange()
{
}


void Indicator::OnEvent::SendDataToCommunicator(Direction::E dir, int num)
{
    if (dir & Direction::USB)
    {
        NeedSend::USB = num;
    }

    if (dir & Direction::RS232)
    {
        NeedSend::RS232 = num;
    }
}


void Indicator::SetStatisticsADC(int peak, int ave, int min, int max)
{
    static TimeMeterMS meter;

    if (meter.ElapsedTime() < 500)
    {
        return;
    }

    meter.Reset();

    textPeakADC.SetText(String<>("peak %d", peak).c_str());

    textAveADC.SetText(String<>("ave %d", ave).c_str());

    textMinADC.SetText(String<>("mn %d", min).c_str());

    textMaxADC.SetText(String<>("mx %d", max).c_str());

    textAvePicADC.SetText(String<>("ave peak %d", (min + max) / 2).c_str());
}
