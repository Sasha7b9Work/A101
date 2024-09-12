// 2022/10/29 22:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


namespace PageMain
{
    extern Page *self;

    void OnEventChangeRange(int new_range);

    extern LabelMeasure wndDC;
    extern LabelMeasure wndAC;

    void SetRange(MeasuresOnDisplay::E, int);

    void EnableZero(MeasuresOnDisplay::E, bool);

    // ���������� ����� �������� ���������
    void HightLightCurrentRange();

//    extern Label labelZeroDC;
//    extern Label labelZeroAC;
}


namespace PageMenu
{
    extern Page *self;
}


namespace PageCOM
{
    extern Page *self;
}


namespace PageInformation
{
    extern Page *self;
}


namespace PageGraph
{
    extern Page *self;
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
