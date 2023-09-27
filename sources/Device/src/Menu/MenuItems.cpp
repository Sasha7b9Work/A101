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


void Button::Press()
{
    funcOnPress(this);
}


void Button::SetText(pchar text) const
{
    Nextion::Button::SetText(name, text);
}


void Button::ChangeVisible(bool visible) const
{
    Nextion::Visible(name, visible);
}


void Button::SetValue(int _value)
{
    value = _value;

    Nextion::SendCommandFormat("%s.val=%d", name, value);
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
    current->funcOnEnable(false);

    current = this;

    current->funcOnEnable(true);
}

Button *Page::GetButton(pchar signal)
{
    Button **button = &buttons[0];

    while (*button)
    {
        if (std::strcmp((*button)->Signal(), signal) == 0)
        {
            return *button;
        }

        button++;
    }

    return nullptr;
}
