// 2022/11/06 17:18:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"
#include "Ampermeter/Calculator/ResolverFFT.h"


/*
*  ���������� ��� ���������� ������� ���
*/


class ResolverPeriodFFT
{
public:
    ResolverPeriodFFT(const ResolverFFT &);
    int ToPoints() const { return period; }
private:
    int period = -1;
    float sum[BufferADC::SIZE];

    // ����� ������� ����� ������������ � ����������� ���������� ��� ��������� ������ ����������� ������� ��
    // ������������ ���������� �� ���������� ����������
    // ���� 
    float FindDelta(int per, float delta_out);

    float FindDelta2(int per, float delta_out);

    // ����� �������� ��� ������� �������, ������� � ����� index_start
    float FindIntegral(int line, int index_start);

    void CalculateSums();
};
