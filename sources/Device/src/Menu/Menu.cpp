// 2022/10/24 11:25:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Menu.h"


namespace Menu
{
    static int index_page = 0;
}


int Menu::IndexPage()
{
    return index_page;
}