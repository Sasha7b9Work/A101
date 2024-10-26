// 2024/10/09 09:43:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


// Вычисляет амплутудное значение
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


// Вычисляет пиковое значение
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
    static const int NUM_POINTS = 1024;
    ResolverFFT();
    // Находит индекс массива частоты
    int FindIndexFreq() const;
private:
    static const int SIZE = NUM_POINTS / 2;
    uint8 data[SIZE];

    void CalculateFFT(float dataR[NUM_POINTS], float result[NUM_POINTS]);
    void Normalize(float *data, int);
};


//  Использует для нахождения периода БПФ
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

    // Найти разницу между максимальным и минимальным интегралом при изменении начала измеряемого отрезка от
    // минимального возможного до минимально возможного
    // Если 
    int FindDelta(int per, int delta_out);

    int FindDelta2(int per, int delta_out);

    // Найти интеграл для данного отрезка, начиная с точки index_start
    int FindIntegral(int line, int index_start);

    void CalculateSums();
};
