// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"
#include "Nextion/Controls/CntrlText.h"


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
        cntrlType(nameType), cntrlSign(nameSign), cntrlDigits(nameDigits), cntrlUnits(nameUnits), label_type(_label_type), type(_type)
    {
        Clear();
    }
    void Clear();
    void SetMeasure(pchar);
    void SetMeasure(float);
    void Draw();
protected:
    CntrlText cntrlType;        // DC или AC
    CntrlText cntrlSign;        // —юда выводим знак
    CntrlText cntrlDigits;      // —юда выводим числовое значение
    CntrlText cntrlUnits;       // «десь единицы измерени€
    pchar label_type = "";
    TypeMeasure::E type;

    char buf_measure[TextString::MAX_LEN];
};
