// 2022/10/24 11:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/Pages/PageMain.h"


void Menu::Init()
{
    PageMain::self->SetAsCurrent();

    Page::Current()->Init();

    Page::textDC.SetSize(Label::Size::Big);

    Page::labelDC.SetSize(Label::Size::Big);

    Page::textAC.SetSize(Label::Size::Big);

    Page::labelAC.SetSize(Label::Size::Big);
}
