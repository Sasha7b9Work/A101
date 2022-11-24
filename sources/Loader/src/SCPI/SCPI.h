// 2022/11/02 15:18:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Communicator.h"


namespace SCPI
{
    void CallbackOnReceive(Direction::E, uint8);

    void Update();
}
