// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"


void Device::Init()
{
    HAL::Init();
}


void Device::Update()
{
    SCPI::Update();
}
