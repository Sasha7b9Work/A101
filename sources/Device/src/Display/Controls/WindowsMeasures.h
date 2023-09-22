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


class WindowMeasureMIN : public WindowMeasure
{
public:
    WindowMeasureMIN() : WindowMeasure("t22", "", "t8", "t25") {}
    void Clear();
};


class WindowMeasurePEAK : public WindowMeasure
{
public:
    WindowMeasurePEAK() : WindowMeasure("t20", "", "t9", "t26") {}
    void Clear();
};


class WindowMeasureAMPL : public WindowMeasure
{
public:
    WindowMeasureAMPL() : WindowMeasure("t23", "", "t10", "t27") {}
    void Clear();
};
