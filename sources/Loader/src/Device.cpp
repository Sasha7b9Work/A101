// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include "stm_includes.h"


State::E State::current = State::WaitUpdate;


namespace Device
{
    static void JumpToMainApplication();
}


void Device::Init()
{
    HAL::Init();
}


void Device::Update()
{
    JumpToMainApplication();

    switch (State::Current())
    {
    case State::WaitUpdate:
        SCPI::Update();
        if (HAL_TIM::TimeMS() > 2000)
        {
            State::Set(State::Completed);
        }
        break;

    case State::InProcessUpdate:
        SCPI::Update();
        break;

    case State::Completed:
        JumpToMainApplication();
        break;
    }
}


void Device::JumpToMainApplication()
{
#define APP_ADDRESS 0x8020000
    
    typedef void (*pFunction)(void);
    pFunction jump_to_app;
    uint32_t jump_address;
    
    __disable_irq();

    SCB->VTOR = APP_ADDRESS;

    __set_MSP(*(__IO uint *)APP_ADDRESS);

    jump_address = *(__IO uint32_t*)(APP_ADDRESS + 4);
    jump_to_app = (pFunction)jump_address;
    __enable_irq();

    jump_to_app();
}
