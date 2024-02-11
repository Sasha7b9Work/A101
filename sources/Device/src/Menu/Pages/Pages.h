// 2022/10/29 22:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"


namespace PageMain
{
    extern Page *self;

    // �������� ����� ����� ���������
    void Init();

    void OnEventChangeRange();
}


namespace PageGraph
{
    extern Page *self;

    // �������� �������� �������
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
