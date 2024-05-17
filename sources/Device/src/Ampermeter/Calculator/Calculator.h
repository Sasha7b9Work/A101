// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


namespace Calculator
{
    // ������� ���������� ������� ������������� ��� ���������� ����� ������
    SampleRate AppendData();

    void Reset();

    // ��������, ���������� � ������� - mA ��� 2,20,200��, ������ ��� 2A, 20A, 50A
    REAL GetRelativeAC(bool *correct);
    REAL GetRelativeDC(bool *correct);

    // ����������� �������� ����� �������, ����� �C�� ��������� ������������ �� �������
    REAL CastRealToDisplay(REAL, int range);
}
