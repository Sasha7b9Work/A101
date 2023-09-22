// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"


class WindowMeasure
{
public:
    WindowMeasure(pchar nameType, pchar nameSign, pchar nameDigits, pchar nameUnits) :
        cntrlType(nameType), cntrlSign(nameSign), cntrlDigits(nameDigits), cntrlUnits(nameUnits)
    {}
    void SetMeasure(pchar);
private:
    pchar cntrlType;        // DC или AC
    pchar cntrlSign;        // —юда выводим знак
    pchar cntrlDigits;      // —юда выводим числовое значение
    pchar cntrlUnits;       // «десь единицы измерени€
};


class WindowMeasureAC : public WindowMeasure
{
public:
    WindowMeasureAC() : WindowMeasure("t3", "", "t1", "t17") {}
private:
};


class WindowMeasureDC : public WindowMeasure
{
public:
    WindowMeasureDC() : WindowMeasure("t2", "t19", "t0", "t18") {}
private:
};
