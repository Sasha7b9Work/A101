// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Ampermeter/BufferADC.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/AD7691.h"
#include "Ampermeter/Calculator/Calculator.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Display/Indicator.h"
#include "Ampermeter/InputRelays.h"
#include "Ampermeter/Calibrator.h"
#include "Hardware/Timer.h"
#include "Display/DiagramInput.h"
#include "stm_includes.h"


void Ampermeter::Init()
{
    HAL_TIM4::Init();

    Calibrator::Init();

    AD7691::Init();

    Indicator::Reset(InputRelays::GetRange());

    Calculator::Reset(InputRelays::GetRange());
}


void Ampermeter::Update()
{
    BufferADC::Clear(SampleRate::Current::Get());

    uint period = SampleRate::Current::Get().TimeUS();

    HAL_TIM4::StartPeriodicUS(period);

    while (!BufferADC::IsFull())
    {
        while (TIM4->CNT < period)
        {
        }

        TIM4->CNT = 0;

        BufferADC::Push(AD7691::ReadValue());
    }

    HAL_TIM4::Stop();

    BufferADC::CalculateLimits();

    SampleRate::Current::Set(Calculator::AppendData());

    Indicator::SetMeasures(Calculator::GetDC(), Calculator::GetAC(), InputRelays::GetRange());

    DiagramInput::SetData();
}
