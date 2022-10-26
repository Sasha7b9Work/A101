// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/Measurer.h"
#include "Measurer/BufferADC.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/AD7691.h"
#include "Measurer/Calculator.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"


namespace Measurer
{
    static BufferADC buffer;
}


void Measurer::Init()
{
    HAL_TIM4::Init();

    AD7691::Init();
}


void Measurer::Update()
{
    uint start_time = HAL_TIM::TimeMS();

    buffer.Clear();

    HAL_TIM4::StartPeriodicUS(6);

    while (!buffer.IsFull())
    {
        HAL_TIM4::WaitEvent();

        uint value = AD7691::ReadValue();

        buffer.Push(value);
    }

    HAL_TIM4::Stop();

    uint time = HAL_TIM::TimeMS() - start_time;
    time = time;

    buffer.ConvertToVoltage();

    Calculator::AppendData(buffer);

    Display::SetAC(Calculator::GetAC());

    Display::SetDC(Calculator::GetDC());

    Display::SetData(buffer);
}
