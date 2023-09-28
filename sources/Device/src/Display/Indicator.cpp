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
    static WindowMeasure   windowAMPL(TypeMeasure::Ampl, "t23", "", "t10", "t27", "Iamp:");
    static WindowMeasure   windowPEAK(TypeMeasure::Peak, "t20", "", "t9", "t26", "Ipp:");
    static WindowMeasure   windowMIN(TypeMeasure::Min, "t22", "", "t8", "t25", "Imin:");
    static WindowMeasure   windowMAX(TypeMeasure::Max, "t21", "", "t7", "t24", "Imax:");

    static bool is_big = true;

    static void SetSmall();

    namespace NeedSend
    {
        static int USB = 0;     // —только раз нужно передавать данные наружу
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
