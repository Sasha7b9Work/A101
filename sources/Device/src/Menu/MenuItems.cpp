// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageMain.h"
#include "Menu/Pages/PageTwo.h"
#include "Menu/Menu.h"
#include "Display/DInterface.h"


Button *Button::ForIndex(int index)
{
    static Button *buttons[2][6] =
    {
        {PageMain::btn2mA, PageMain::btn20mA, PageMain::btn200mA, PageMain::btn2A, PageMain::btn20A, PageMain::btn50A},
        {PageTwo::btnInput, PageTwo::btnFFT, PageTwo::btnEmpty1, PageTwo::btnEmpty2, PageTwo::btnEmpty3, PageTwo::btnEmpty4}
    };

    return buttons[Menu::IndexPage()][index - 1];
}


void Button::RemoveBacklight()
{
//    static pchar names[7] = { "bt2mA", "bt2mA", "bt20mA", "bt200mA", "bt2A", "bt20A", "bt50A" };

//    Display::Interface::SendCommandFormat("%s.val=0", names[index]);
}


void Button::SetBacklight()
{
//    static pchar names[7] = { "bt2mA", "bt2mA", "bt20mA", "bt200mA", "bt2A", "bt20A", "bt50A" };

//    Display::Interface::SendCommandFormat("%s.val=1", names[index]);
}
