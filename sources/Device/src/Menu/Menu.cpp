// 2022/10/24 11:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Pages.h"
#include <cstring>


void Menu::Init()
{
    PageMain::self->SetAsCurrent();
}


void Menu::Update(pchar name_button)
{
    Button *button = Page::Current()->GetButton(name_button);

    if (button)
    {
        button->Press();
    }
}
