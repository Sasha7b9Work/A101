// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Nextion/Nextion.h"
#include "Menu/Pages/Pages.h"
#include <cstring>
#include <cstdio>


using namespace std;


Page *Page::current = PageMain::self;


void Button::SetHighlight(bool _higthligth)
{
    highlight = _higthligth;

    SendHighlightState();
}


void Button::ToggleHighlight()
{
    SetHighlight(!highlight);
}


void Button::Press()
{
//    LOG_WRITE("Press button %s", text);

    funcOnPress(this);
}


Page::Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5)
{
    buttons[0] = btn0;
    buttons[1] = btn1;
    buttons[2] = btn2;
    buttons[3] = btn3;
    buttons[4] = btn4;
    buttons[5] = btn5;
//    buttons[6] = &btnMenu;
}


Button *Page::GetButton(int index)
{
    return buttons[index];
}


void Page::SetButton(int index, Button *button)
{
    buttons[index] = button;
}


void Page::SetAsCurrent()
{
    current = this;

    for (int i = 0; i < 6; i++)
    {
        Button *button = GetButton(i);

        if (button->IsExist())
        {
            button->Init();
            button->Enable();
            button->SetText();
            button->SendHighlightState();
        }
        else
        {
            button->Disable();
        }
    }
}
