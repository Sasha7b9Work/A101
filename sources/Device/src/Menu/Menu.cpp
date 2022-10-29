// 2022/10/24 11:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageMain.h"
#include <cstring>


namespace Menu
{
    static void FuncPressButtonMenu(int state)
    {
        if (state == 1)
        {
            if (strcmp(btnMenu.GetText(), ">>") == 0)
            {
                btnMenu.SetText("<<");
            }
            else
            {
                btnMenu.SetText(">>");
            }
        }
    }

    Button btnMenu("btMenu", ">>", false, FuncPressButtonMenu);
}


void Menu::Init()
{
    PageMain::self->SetAsCurrent();

    Page::Current()->Init();

    btnMenu.SetText();
}
