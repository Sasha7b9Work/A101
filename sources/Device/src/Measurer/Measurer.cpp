// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/Measurer.h"
#include "Measurer/BufferADC.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/AD7691.h"
#include "Measurer/Calculator.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/MenuItems.h"


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
    buffer.Clear();

    HAL_TIM4::StartPeriodicUS(10);

    for (int i = 0; i < 100; i++)
    {
        AD7691::ReadValue();
    }

    while (!buffer.IsFull())
    {
        HAL_TIM4::WaitEvent();

        buffer.Push(AD7691::ReadValue());
    }

    HAL_TIM4::Stop();

    buffer.ConvertToVoltage();

    Calculator::AppendData(buffer);

    Page::textDC.SetValue(Calculator::GetAC());

    Page::textAC.SetValue(Calculator::GetDC());

    Display::SetData(buffer);
}
