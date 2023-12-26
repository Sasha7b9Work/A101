// 2022/10/28 22:52:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define WIN32_LEAN_AND_MEAN
#include "defines.h"
#include "GUI/Application.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Display/Display.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/Ampermeter.h"
#include "GUI/Screen.h"
#include "SCPI/SCPI.h"


void Application::Init()
{
    HAL::Init();

    Ampermeter::Init();

    Menu::Init();

    Screen::self->Init();

    Display::Init();
}


void Application::Update()
{
    Ampermeter::Update();

    Display::Update();

    Nextion::Update();

    SCPI::Update();
}
