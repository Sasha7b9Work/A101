// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"
#include "Nextion/Controls/CntrlText.h"
#include "Ampermeter/Ampermeter.h"


struct TypeMeasure
{
    enum E
    {
        DC, 
        AC,
        Ampl,
        Peak,
        Min,
        Max
    };
};


class WindowMeasure
{
public:
    WindowMeasure(TypeMeasure::E _type, pchar nameType, pchar nameSign, pchar nameDigits, pchar nameUnits, pchar _label_type = "") :
        cntrlType(nameType), cntrlSign(nameSign), cntrlDigits(nameDigits), cntrlUnits(nameUnits), __label_type(_label_type), type(_type)
    {
    }
    void Draw(const Measure &, int range);
    // ������� �������, ���� ���� ����� ������� ��� ��������
    void Flash();
    // �������� ��� ������� ����� ������������ �� ������ ��������, ����� �������� �����������
    void Reset();

    const CntrlText &GetControlSign() const { return cntrlSign; }
    const CntrlText &GetControlDigits() const { return cntrlDigits; }
    const CntrlText &GetControlUnits() const { return cntrlUnits; }

    void SetVisible(bool);

protected:
    CntrlText cntrlType;        // DC ��� AC
    CntrlText cntrlSign;        // ���� ������� ����
    CntrlText cntrlDigits;      // ���� ������� �������� ��������
    CntrlText cntrlUnits;       // ����� ������� ���������
    pchar __label_type = "";
    TypeMeasure::E type;

    char buf_measure[TextString::MAX_LEN];

    void Clear();
    void SetMeasure(pchar);
    void SetMeasure(REAL);
    // after - ���������� ���� ����� �������
    void ConvertRealToText(REAL value, char buffer[TextString::MAX_LEN], int after, pchar suffix);
};
