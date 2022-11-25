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
    switch (State::Current())
    {
    case State::WaitUpdate:
        SCPI::Update();
        if (HAL_TIM::TimeMS() > 1000)
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
    __disable_irq();

    pFuncVV JumpToApplication;

    JumpToApplication = (pFuncVV)(*(__IO uint *)(0x8020000 + 4)); //-V566

    __set_MSP(*(__IO uint *)0x8020000);

    __enable_irq();

    JumpToApplication();
}
