// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Calculator
{
    // ������� ���������� ������� ������������� ��� ���������� ����� ������
    SampleRate AppendData();

    void Reset();

    double GetAC(bool *correct);

    double GetDC(bool *correct);
}
