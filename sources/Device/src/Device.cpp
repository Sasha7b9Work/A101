// 2022/10/18 09:00:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/Measurer.h"


void Device::Init()
{
    HAL::Init();

    Measurer::Init();
}


void Device::Update()
{
    Measurer::Update();

    Display::Update();
}
