// 2022/10/18 15:54:07 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/Measurer.h"
#include "Measurer/BufferRAW.h"
#include "Hardware/HAL/HAL.h"


namespace Measurer
{
    static BufferRAW buffer;
}


void Measurer::Init()
{
    HAL_TIM4::Init();
}


void Measurer::Update()
{
    HAL_TIM4::Start(6);


}
