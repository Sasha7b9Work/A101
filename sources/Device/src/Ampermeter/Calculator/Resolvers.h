// 2024/10/09 09:43:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Utils/Buffer.h"


/*
*   Imax - максимальное мгновенное значение отсчёта
*   Imin - минимальное мгновенное значение отсчёта
*   Ipeak - разница между Imax и Imin
*
*   Iampl - усреднённое по десяти периодам значение Ipeak
*/


// Вычисляет все измерения
class ResolverMeasures
{
public:

    ResolverMeasures(const Period &, REAL frequency);

    REAL GetPeak() const {  return max - min; }
    REAL GetMin() const { return min; }
    REAL GetMax() const { return max; }
    REAL GetAmplitude() const { return amplitude; }

private:

    REAL min = 0.0;         // Минимальное мгновенное значение по всей выборке
    REAL max = 0.0;         // Максимальное мгновенное значение по всей выборке
    REAL amplitude = 0.0;   // Разица между макс и мин по 10 периодам

    REAL CalculateAmplitudeSteady(int first, int num_poinst, int num_periods) const;
    REAL CalculateAmplitudeSteadyPeriod(int first, int num_points) const;

    void CalculateNumPoints(REAL frequency, int *out_num_points, int *out_num_periods) const;
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
    static const int NUM_POINTS = 2048;             // Столько точек во входном массиве
    static const int LOG_N = 11;                    // Это 2 * LOG_N, чтобы получилось NUM_POINTS
    static const int SIZE_DATA = NUM_POINTS / 2;    // Столько точек в выходном массиве
    static const float minDB;

    // Через delta нужно брать точки
    ResolverFFT(int delta);

    // Находит индекс массива частоты
    int FindIndexFreq() const;

    uint8 At(int i) const
    {
        return (i < SIZE_DATA) ? data[i] : 0U;
    }

private:

    uint8 data[SIZE_DATA];

    void CalculateFFT(float dataR[NUM_POINTS], float result[NUM_POINTS]);
    void Normalize(float *, uint);
    void TransformToLogarifm(float *, uint);

    void ApplyWindowHamming(float *, uint);
    void ApplyWindowBlackman(float *, uint);
    void ApplyWindowHann(float *, uint);
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
