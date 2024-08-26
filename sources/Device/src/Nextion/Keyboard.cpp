// 2024/08/22 16:42:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/Keyboard.h"
#include "Nextion/Display.h"
#include "Nextion/Controls.h"
#include "Menu/MenuItems.h"


namespace Keyboard
{
    static int x = 0;
    static int y = 0;
}


int Keyboard::CurrentX()
{
    return x;
}


int Keyboard::CurrentY()
{
    return y;
}


void Keyboard::CallbackOnPress(int _x, int _y)
{
    x = _x;
    y = _y;

    ButtonCommon::OnEventPress(x, y);
}


void Keyboard::CallbackOnRelease(int _x, int _y)
{
    x = _x;
    y = _y;

    ButtonCommon::OnEventRelease(x, y);
}
