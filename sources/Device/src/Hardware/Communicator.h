// 2022/11/23 11:28:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
*  ¬заимодействие с внешними устройствами через RS232, USB
*/


namespace Communicator
{
    void SendWith0D0A(pchar);

    void Send(pchar);
}
