// 2022/10/19 09:02:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramFFT.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"


namespace DiagramFFT
{
    static bool enabled = false;
}


void DiagramFFT::Enable(bool _enable)
{
    if (enabled == _enable)
    {
        return;
    }

    enabled = _enable;

    Painter::WaveFFT::Disable(0);
    Painter::WaveFFT::Disable(1);
    Painter::WaveInput::Disable(0);

    if (enabled)
    {
        if (DiagramInput::IsEnabled())
        {
            Painter::WaveFFT::Enable(0);
            Painter::WaveInput::Enable(0);
        }
        else
        {
            Painter::WaveFFT::Enable(1);
        }
    }
}


bool DiagramFFT::IsEnabled()
{
    return enabled;
}


void DiagramFFT::Draw()
{

}
