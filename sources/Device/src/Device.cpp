// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Nextion/Display.h"
#include "Nextion/Nextion.h"
#include "Hardware/HAL/HAL.h"
#include "Ampermeter/Ampermeter.h"
#include "Menu/Menu.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Pages.h"


void Device::Init()
{
    cal.Load();
    set.Load();

    HAL::Init();

    HAL_TIM::Delay(500);

    Ampermeter::Init();

    Display::Init();

    Timer::Delay(1000);         // Задержка на стартовый экран

    Menu::Init();
}


void Device::Update()
{
    Display::Update();

    Ampermeter::Update();

    Nextion::Update();

    Nextion::Update();

    Nextion::Update();

    SCPI::Update();

    Nextion::Update();

    set.Save();
}


void Device::Reset()
{
    PageMain::self->GetItem(3)->Press();
}
