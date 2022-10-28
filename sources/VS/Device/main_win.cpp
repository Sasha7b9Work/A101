// 2022/10/28 22:52:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#define WIN32_LEAN_AND_MEAN
#include "defines.h"
#include "Application.h"
#include "Hardware/HAL/HAL.h"
#include "Measurer/Measurer.h"
#include "Menu/Menu.h"
#include "Display/Display.h"
#include "Display/DInterface.h"


void Application::Init()
{
    HAL::Init();

    Measurer::Init();

    Menu::Init();

    Display::Init();
}


void Application::Update()
{
    Measurer::Update();

    Display::Update();

    DInterface::Update();
}
