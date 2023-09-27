// 2022/10/29 22:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace PageMain
{
    extern Page *self;
}


namespace PageGraph
{
    extern Page *self;

    // ¬ключить страницу отладки
    void EnableDebug();

    // ¬ключить страницу калибровки
    void EnableCalibration();
}


namespace PageCalibration
{
    extern Page *self;
}


namespace PageDebug
{
    extern Page *self;
}
