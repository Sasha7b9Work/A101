// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"
#include "Nextion/Controls/CntrlText.h"


class WindowMeasure
{
public:
    WindowMeasure(pchar nameType, pchar nameSign, pchar nameDigits, pchar nameUnits) :
        cntrlType(nameType), cntrlSign(nameSign), cntrlDigits(nameDigits), cntrlUnits(nameUnits)
    {}
    void Clear();
    void SetMeasure(pchar);
    void SetMeasure(float);
protected:
    CntrlText cntrlType;        // DC или AC
    CntrlText cntrlSign;        // —юда выводим знак
    CntrlText cntrlDigits;      // —юда выводим числовое значение
    CntrlText cntrlUnits;       // «десь единицы измерени€
};


class WindowMeasureAC : public WindowMeasure
{
public:
    WindowMeasureAC() : WindowMeasure("t3", "", "t1", "t17") {}
    void Clear();
private:
};


class WindowMeasureDC : public WindowMeasure
{
public:
    WindowMeasureDC() : WindowMeasure("t2", "t19", "t0", "t18") {}
    void Clear();
private:
};


class WindowMeasureMAX : public WindowMeasure
{
public:
    WindowMeasureMAX() : WindowMeasure("t21", "", "t7", "t24") {}
    void Clear();
};
