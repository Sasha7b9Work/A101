// 2022/10/19 09:02:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramFFT.h"


namespace DiagramFFT
{
    static bool enabled = false;
}


void DiagramFFT::Enable(bool _enable)
{
    enabled = _enable;
}


bool DiagramFFT::IsEnabled()
{
    return enabled;
}
