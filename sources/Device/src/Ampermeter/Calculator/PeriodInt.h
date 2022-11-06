// 2022/11/06 17:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"


class PeriodInt
{
public:
    PeriodInt(const BufferADC &);
    int ToPoints() const { return period; }
private:
    int period = -1;
    // � ���� ������� �������� ����� �������� �� 1-� ����� �� ���������, ������� � �������
    int sum[BufferADC::SIZE];

    // ����� ������� ����� ������������ � ����������� ���������� ��� ��������� ������ ����������� ������� ��
    // ������������ ���������� �� ���������� ����������
    int FindDelta(int per);
};
