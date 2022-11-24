// 2022/11/23 11:28:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  ¬заимодействие с внешними устройствами через RS232, USB
*/


struct Direction
{
    enum E
    {
        USB = 1 << 0,
        RS232 = 1 << 1,
        ALL = USB | RS232
    };
};


namespace Communicator
{
    void SendWith0D0A(Direction::E, pchar);

    void Send(Direction::E, pchar);
}
