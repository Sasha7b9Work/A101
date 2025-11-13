// 2022/11/23 14:55:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "stm_includes.h"


namespace Device
{
    static void JumpToMainApplication();

    // Есть ли новая прошивка для обновления
    static bool ExistFirmwareInROM();
}


void Device::Init()
{
    HAL::Init();
}


void Device::Update()
{
    JumpToMainApplication();
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

    jump_address = *(__IO uint32_t *)(APP_ADDRESS + 4);
    jump_to_app = (pFunction)jump_address;
    __enable_irq();

    jump_to_app();
}
