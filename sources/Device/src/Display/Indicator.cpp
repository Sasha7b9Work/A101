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
    static WindowMeasure   windowAMPL("t23", "", "t10", "t27", "Iamp:");
    static WindowMeasure   windowPEAK("t20", "", "t9", "t26", "Ipp:");
    static WindowMeasure   windowMIN("t22", "", "t8", "t25", "Imin:");
    static WindowMeasure   windowMAX("t21", "", "t7", "t24", "Imax:");

    static bool is_big = true;

    static void SetSmall();

    // after - количество цифр после запятой
    void ConvertDoubleToText(float value, char buffer[TextString::MAX_LEN], int after, pchar suffix);

    namespace NeedSend
    {
        static int USB = 0;     // Столько раз нужно передавать данные наружу
        static int RS232 = 0;
    }
}


void Indicator::AutoSize()
{
    if (DiagramInput::IsEnabled() || DiagramFFT::IsEnabled())
    {
        SetSmall();
    }
    else
    {
    }
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
