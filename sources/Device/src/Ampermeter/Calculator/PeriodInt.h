// 2022/11/06 17:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"
#include "Ampermeter/Calculator/FFT.h"


class PeriodInt
{
public:
    PeriodInt(const BufferADC &, const FFT &);
    int ToPoints() const { return period; }
private:
    int period = -1;
    int sum[BufferADC::SIZE];

    // ����� ������� ����� ������������ � ����������� ���������� ��� ��������� ������ ����������� ������� ��
    // ������������ ���������� �� ���������� ����������
    // ���� 
    int FindDelta(const BufferADC &, int per, int delta_out);

    int FindDelta2(const BufferADC &, int per, int delta_out);

    // ����� �������� ��� ������� �������, ������� � ����� index_start
    int FindIntegral(const BufferADC &, int line, int index_start);

    void CalculateSums(const BufferADC &);
};
