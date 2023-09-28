// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"
#include "Nextion/Controls/CntrlText.h"


class WindowMeasure
{
public:
    WindowMeasure(pchar nameType, pchar nameSign, pchar nameDigits, pchar nameUnits, pchar _label_type = "") :
        cntrlType(nameType), cntrlSign(nameSign), cntrlDigits(nameDigits), cntrlUnits(nameUnits), label_type(_label_type)
    {}
    void Clear();
    void SetMeasure(pchar);
    void SetMeasure(float);
protected:
    CntrlText cntrlType;        // DC ��� AC
    CntrlText cntrlSign;        // ���� ������� ����
    CntrlText cntrlDigits;      // ���� ������� �������� ��������
    CntrlText cntrlUnits;       // ����� ������� ���������
    pchar label_type = "";
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
