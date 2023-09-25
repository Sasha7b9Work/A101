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
    static WindowMeasureDC windowDC;
    static WindowMeasureAC windowAC;

    static WindowMeasureAMPL windowAMPL;
    static WindowMeasurePEAK windowPEAK;
    static WindowMeasureMIN windowMIN;
    static WindowMeasureMAX windowMAX;

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
    windowAC.Clear();
    windowDC.Clear();
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

    windowDC.SetMeasure(measureDC);
    windowAC.SetMeasure(measureAC);
}


void Indicator::SetSmall()
{
    is_big = false;
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

    windowPEAK.SetMeasure((float)peak);

    windowAMPL.SetMeasure((float)ave);

    windowMIN.SetMeasure((float)min);

    windowMAX.SetMeasure((float)max);
}
