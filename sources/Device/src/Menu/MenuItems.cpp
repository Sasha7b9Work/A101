// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/PageMain.h"
#include "Menu/Pages/PageTwo.h"
#include "Menu/Menu.h"
#include "Display/DInterface.h"


Page *Page::current = PageMain::self;


static void EmptyFunc(int)
{
}


Button Button::empty("null", "", false, EmptyFunc);


void Button::SetText()
{
    DInterface::SendCommandFormat("%s.txt=\"%s\"", name_button, text);
}


void Button::Highlight()
{
    DInterface::SendCommandFormat("%s.val=%d", name_button, highlight ? 1 : 0);
}


void Button::ToState(int state)
{
    LOG_WRITE("Button %s to state %d", text, state);

    funcOnPress(state);
}


Page::Page(Button *btn0, Button *btn1, Button *btn2, Button *btn3, Button *btn4, Button *btn5)
{
    buttons[0] = btn0;
    buttons[1] = btn1;
    buttons[2] = btn2;
    buttons[3] = btn3;
    buttons[4] = btn4;
    buttons[5] = btn5;
}


Button *Page::GetButton(int index)
{
    if (index >= 0 && index < 6)
    {
        return buttons[index];
    }
    else
    {
        return &Button::empty;
    }
}


void Page::SetAsCurrent()
{
    current = this;

    for (int i = 0; i < 6; i++)
    {
        GetButton(i)->SetText();
    }
}


void Page::Init()
{
    for (int i = 0; i < 6; i++)
    {
        GetButton(i)->Highlight();
    }
}
