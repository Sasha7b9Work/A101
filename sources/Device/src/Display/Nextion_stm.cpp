// 2022/10/31 19:37:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Nextion.h"


void Nextion::DrawRect(int x, int y, int width, int height, const Color &color)
{
    color.SetAsCurrent();

    Nextion::SendCommandFormat("draw %d,%d,%d,%d,%s", x, y, width, height, Color::CurrentValue());
}
