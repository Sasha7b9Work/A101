// 2022/10/29 22:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Display/Controls/WindowsMeasures.h"


namespace PageMain
{
    extern Page *self;

    // Вызываем сразу после включения
    void Init();

    void OnEventChangeRange();

    extern WindowMeasure wndDC;
    extern WindowMeasure wndAC;
}


namespace PageGraph
{
    extern Page *self;

    // Включить страницу отладки
    void EnableDebug();
}


namespace PageCalibration
{
    extern Page *self;

    void OnEventChangeRange();
}


namespace PageDebug
{
    extern Page *self;
}


namespace PageSettings
{
    extern Page *self;
}
