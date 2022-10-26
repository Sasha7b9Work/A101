// 2022/10/18 09:00:32 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "Display/DInterface.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/Measurer.h"
#include "Menu/Menu.h"


void Device::Init()
{
    HAL::Init();

    Measurer::Init();

    Menu::Init();
}


void Device::Update()
{
    Measurer::Update();

    Display::Update();

    DInterface::Update();
}
