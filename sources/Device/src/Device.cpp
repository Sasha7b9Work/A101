// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Display/Display.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/Ampermeter.h"
#include "Menu/Menu.h"
#include "Utils/Profiler.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"


void Device::Init()
{
    HAL::Init();

    HAL_TIM::Delay(500);

    cal.Load();
    set.Load();

    Ampermeter::Init();

    Display::Init();

    Menu::Init();
}


void Device::Update()
{
    Ampermeter::Update();

    Nextion::Update();

    Display::Update();

    Profiler::Update();

    SCPI::Update();

    set.Save();
}


void Device::Reset()
{
    PageMain::self->GetButton(3)->Press();
}
