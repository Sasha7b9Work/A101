// 2022/10/19 09:02:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramFFT.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"
#include "Display/Nextion.h"


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

    Nextion::WaveFFT::Disable(0);
    Nextion::WaveFFT::Disable(1);
    Nextion::WaveInput::Disable(0);

    if (enabled)
    {
        if (DiagramInput::IsEnabled())
        {
            Nextion::WaveFFT::Enable(0);
            Nextion::WaveInput::Enable(0);
        }
        else
        {
            Nextion::WaveFFT::Enable(1);
        }
    }
    else
    {
        if (DiagramInput::IsEnabled())
        {
            Nextion::WaveInput::Enable(1);
        }
    }

    DiagramInput::Repaint();
}


bool DiagramFFT::IsEnabled()
{
    return enabled;
}


void DiagramFFT::Draw()
{

}
