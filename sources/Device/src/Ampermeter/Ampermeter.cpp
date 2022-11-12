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
#include "Ampermeter/Calculator/Averager.h"
#include "stm_includes.h"



namespace Ampermeter
{
    // Медианный фильтр по трём
    struct MiddlerOf3
    {
        void Push(ValueADC value)
        {
            averager.Push(value.Real());
        }
        ValueADC Get()
        {
            if (averager.NumElements() > 2)
            {
                return ValueADC::FromReal(middle_of_3(averager.Pop(0), averager.Pop(1), averager.Pop(2)));
            }
            else if (averager.NumElements() == 2)
            {
                return ValueADC::FromReal(averager.Pop(1));
            }

            return ValueADC::FromReal(averager.Pop(0));
        }

    private:

        Averager <float, 3> averager;

        float middle_of_3(float a, float b, float c)
        {
            if ((a <= b) && (a <= c))
            {
                return (b <= c) ? b : c;
            }
            else if ((b <= a) && (b <= c))
            {
                return (a <= c) ? a : c;
            }
            return (a <= b) ? a : b;
        }
    };
}


void Ampermeter::Init()
{
    HAL_TIM4::Init();

    Calibrator::Init();

    AD7691::Init();

    Indicator::OnCnageRangeEvent();

    Calculator::OnChangeRangeEvent();
}


void Ampermeter::Update()
{
    BufferADC::Clear(SampleRate::Current::Get());

    uint period = SampleRate::Current::Get().TimeUS();

    HAL_TIM4::StartPeriodicUS(period);

    while (!BufferADC::IsFull())
    {
#ifndef WIN32
        while (TIM4->CNT < period)
        {
        }

        TIM4->CNT = 0;
#endif

        BufferADC::Push(AD7691::ReadValue());
    }

    HAL_TIM4::Stop();

    BufferADC::MiddleOf3();

    BufferADC::CalculateLimits();

    SampleRate::Current::Set(Calculator::AppendData());

    Indicator::SetMeasures(Calculator::GetDC(), Calculator::GetAC(), InputRelays::GetRange());

    DiagramInput::SetData();
}
