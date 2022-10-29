// 2022/10/24 11:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageMain.h"


namespace Menu
{
    static void FuncPressButtonMenu(int state)
    {
    }

    Button btnMenu("btMenu", ">>", false, FuncPressButtonMenu);
}


void Menu::Init()
{
    PageMain::self->SetAsCurrent();

    Page::Current()->Init();

    btnMenu.SetText();
}
