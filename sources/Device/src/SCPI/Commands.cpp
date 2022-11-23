// 2022/11/23 12:38:18 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/Commands.h"


namespace SCPI
{
    void Send(pchar);
}


bool SCPI::Command::Execute()
{
    return false;
}


bool SCPI::CommandIDN::Execute()
{
    Send("OAO MNIPI, A101");
    return true;
}
