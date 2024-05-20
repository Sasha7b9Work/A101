// 2024/05/20 09:29:20 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/AVP.h"


namespace AVP
{
    static bool is_enabled = false;
}


void AVP::Enable()
{
    is_enabled = true;
}


void AVP::Disable()
{
    is_enabled = false;
}


bool AVP::IsEnabled()
{
    return is_enabled;
}
