// 2022/10/18 09:00:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"


void Device::Init()
{
    HAL::Init();
}


void Device::Update()
{
    static float voltageDC = 0.0f;

    voltageDC += 1.0f;

    Display::SetDC(voltageDC);
}
