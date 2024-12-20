// 2022/10/29 22:49:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#include "Ampermeter/AD7691.h"


namespace PageMain
{
    extern Page *self;

    void OnEventChangeRange(int new_range);

    extern LabelMeasure wndDC;
    extern LabelMeasure wndAC;

    extern ButtonToggle btnZeroAC;
    extern ButtonToggle btnZeroDC;

    // Форсировать действия (произвести их, даже если не требуется)
    void SetMeasuresOnDisplay(MeasuresOnDisplay::E, bool force = false);
    void SetRange(int);

    void EnableZero(MeasuresOnDisplay::E, bool);

    void SetSampleRate(SampleRate::E);

    // Подсветить выбор текущего диапазона
    void HightLightCurrentRange();

    // После изменения выводимых на экран измерений нужно вызывать эту функцию
    void RedrawAllMeasures();

    extern ButtonToggle btnAVP;

    namespace Star
    {
        void Draw();
    }
}


namespace PageMenu
{
    void Init();

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
