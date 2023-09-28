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
