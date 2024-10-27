// 2024/10/09 09:43:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Utils/Buffer.h"


// ��������� ����������� ��������
class ResolverAmpl
{
public:

    ResolverAmpl(const Period &);

    REAL GetAmplitude() const
    {
        return max - min;
    }

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

    REAL CalculateMax(int first, int last, REAL ave) const;

    REAL CalculateMin(int first, int last, REAL ave) const;
};


// ��������� ������� ��������
class ResolverPeak
{
public:

    ResolverPeak(const Period &);

    REAL GetResult() const {  return max - min; }

private:

    REAL min = 0.0;
    REAL max = 0.0;
};


class ResolverFrequency
{
public:

    ResolverFrequency(const Period &period);

    REAL GetFrequency() const { return frequency; }

private:

    REAL frequency = 0.0;
};


class ResolverFFT
{
public:
    static const int NUM_POINTS = 8192;             // ������� ����� �� ������� �������
    static const int LOG_N = 13;                    // ��� 2 * LOG_N, ����� ���������� NUM_POINTS
    static const int SIZE_DATA = NUM_POINTS / 2;    // ������� ����� � �������� �������

    // ����� delta ����� ����� �����
    ResolverFFT(int delta);

    // ������� ������ ������� �������
    int FindIndexFreq() const;

    uint8 At(int i) const
    {
        return (i < SIZE_DATA) ? data[i] : 0U;
    }

private:

    uint8 data[SIZE_DATA];

    void CalculateFFT(float dataR[NUM_POINTS], float result[NUM_POINTS]);
    void Normalize(Buffer<NUM_POINTS, float> &);

    void ApplyHamming(Buffer<NUM_POINTS, float> &);
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
