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


Button Page::btnMenu("btMenu", ">>", false, [](Button *btn)
    {
        if (strcmp(btn->GetText(), ">>") == 0)
        {
            btn->SetText("<<");

            PageTwo::self->SetAsCurrent();
        }
        else
        {
            if (Page::Current() == PageDebug::self)
            {
                PageTwo::self->SetAsCurrent();
            }
            else if (Page::Current() == PageCalibration::self)
            {
                PageTwo::self->SetAsCurrent();
            }
            else
            {
                btn->SetText(">>");

                PageMain::self->SetAsCurrent();
            }
        }
    });


void Button::SetText()
{
    Nextion::Button::SetText(name, text);
}


void Button::SetText(pchar _text)
{
    text = _text;

    SetText();
}


void Button::Enable()
{
    Nextion::Button::Eanble(name);
}


void Button::Disable()
{
    Nextion::Button::Disable(name);
}


void Button::SendHighlightState()
{
    Nextion::Button::Highligth(name, highlight);
}


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
    LOG_WRITE("Press button %s", text);

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
    buttons[6] = &btnMenu;
}


Button *Page::GetButton(int index)
{
    if (index >= 0 && index < 6)
    {
        return buttons[index];
    }
    else if (index == 6)
    {
        return &btnMenu;
    }

    return buttons[0];
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

    btnMenu.SetText();
}
