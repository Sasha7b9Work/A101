// 2022/10/17 15:30:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Display
{
    namespace Interface
    {
        void SendCommand(pchar);

        void CallbackOnReceive(uint8);
    }
}
