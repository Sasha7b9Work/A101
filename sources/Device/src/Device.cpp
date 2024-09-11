// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Nextion/Display.h"
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
    TimeMeterMS timer;
    
    Ampermeter::Update();
    
    volatile uint time = timer.ElapsedTime();

    Nextion::Update();
    
    time = timer.ElapsedTime();

    Display::Update();
    
    time = timer.ElapsedTime();

    Profiler::Update();
    
    time = timer.ElapsedTime();

    SCPI::Update();
    
    time = timer.ElapsedTime();

    set.Save();
}


void Device::Reset()
{
    PageMain::self->GetItem(3)->Press();
}
