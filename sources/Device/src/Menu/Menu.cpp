// 2022/10/24 11:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include <cstring>


void Menu::Init()
{
    PageMain::self->SetAsCurrent();

    PageMain::Init();
}


void Menu::Update(pchar signal)
{
    ButtonOld *button = Page::Current()->GetButton(signal);

    if (button)
    {
        button->Press();
    }
}


void Menu::Press(int , int )
{

}
