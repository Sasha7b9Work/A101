// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Ampermeter.h"
#include "Ampermeter/BufferADC.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/AD7691.h"
#include "Ampermeter/Calculator.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"
#include "Display/Indicator.h"
#include "Ampermeter/InputRelays.h"


namespace Measurer
{
    static BufferADC buffer;

    static SampleRate sampleRate(10);
}


void Measurer::Init()
{
    HAL_TIM4::Init();

    AD7691::Init();
}


void Measurer::Update()
{
    buffer.Clear(sampleRate);

    HAL_TIM4::StartPeriodicUS(sampleRate.Time());

    while (!buffer.IsFull())
    {
        HAL_TIM4::WaitEvent();

        buffer.Push(AD7691::ReadValue());
    }

    HAL_TIM4::Stop();

    buffer.ConvertToVoltage();

    Calculator::AppendData(buffer);

    Indicator::SetMeasures(Calculator::GetDC(), Calculator::GetAC());

    Display::SetData(buffer);
}
