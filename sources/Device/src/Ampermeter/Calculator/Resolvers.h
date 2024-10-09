// 2024/10/09 09:43:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


// ��������� ����������� ��������
class ResolverAmpl
{
public:

    ResolverAmpl(const Period &, REAL min, REAL max);

    REAL GetResult() const
    {
        return _ampl;
    }

private:

    REAL _ampl = 0.0f;

    REAL CalculateMax(int first, int last, REAL ave, REAL min, REAL max) const;

    REAL CalculateMin(int first, int last, REAL ave, REAL min, REAL max) const;
};


// ��������� ����������� � ������������ ��������
class ResolverMinMax
{
public:

    ResolverMinMax(const Period &);

    REAL GetMin() const
    {
        return min;
    }

    REAL GetMax() const
    {
        return max;
    }

private:

    REAL min = 0.0;
    REAL max = 0.0;
};


class ResolverFrequency
{
public:

    ResolverFrequency(const Period &);

    REAL GetFrequency() const { return frequency; }

private:

    REAL frequency = 0.0;
};


class ResolverFFT
{
public:
    static const int NUM_POINTS = 1024;
    ResolverFFT();
    // ������� ������ ������� �������
    int FindIndexFreq() const;
private:
    static const int SIZE = NUM_POINTS / 2;
    uint8 data[SIZE];

    void CalculateFFT(float dataR[NUM_POINTS], float result[NUM_POINTS]);
    void Normalize(float *data, int);
};


//  ���������� ��� ���������� ������� ���
class ResolverPeriodFFT
{
public:
    ResolverPeriodFFT(const ResolverFFT &);
    int ToPoints() const
    {
        return period;
    }
private:
    int period = -1;
    int sum[BufferADC::SIZE];

    // ����� ������� ����� ������������ � ����������� ���������� ��� ��������� ������ ����������� ������� ��
    // ������������ ���������� �� ���������� ����������
    // ���� 
    int FindDelta(int per, int delta_out);

    int FindDelta2(int per, int delta_out);

    // ����� �������� ��� ������� �������, ������� � ����� index_start
    int FindIntegral(int line, int index_start);

    void CalculateSums();
};
