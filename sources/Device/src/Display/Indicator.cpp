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
