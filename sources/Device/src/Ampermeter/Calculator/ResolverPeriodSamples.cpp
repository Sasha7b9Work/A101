// 2022/11/09 20:49:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Hardware/Timer.h"
#include <cmath>


// Структура для расчётa положительного и отрицательного интегралов
struct DualIntegral
{
    DualIntegral(const Period &);

    void Recalculate(const Period &);

    int64 Positive() const { return positive; }

    int64 Negative() const { return negative; }

    // Возвращает абсолютное значение разницы между positive и negative. 
    int64 Delta() const { return delta; }

private:

    uint CalculatePositive(const Period &);
    uint CalculateNegative(const Period &);

    int64 positive = 0;
    int64 negative = 0;
    int64 delta = 0;
};


class ResolverDC
{
public:

    ResolverDC(const Period &);

    ValueADC GetResult() const { return result; }

private:

    ValueADC result;
};


ResolverPeriodSamples::ResolverPeriodSamples()
{
    int sum = 0;

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        sum += BufferADC::At(i);
    }

    int averaging = sum / BufferADC::SIZE;

    Period period;

    if (!CalculateRoughly(ValueADC::FromRaw(averaging), period))
    {
        SetFullPeriod(ValueADC::FromRaw(averaging));

        return;
    }

    ValueADC dc = ResolverDC(period).GetResult();

    CalculateAccuracy(dc);
}


bool ResolverPeriodSamples::BadIntersection(const Intersection &first, const Intersection &second)
{
    return (first.type == Intersection::Type::Empty) || (second.type == Intersection::Type::Empty);
}


void ResolverPeriodSamples::SetFullPeriod(ValueADC _dc)
{
    result_period.dc = _dc;
    result_period.first.Set(Intersection::Type::Rise, 0, 1);
    result_period.last.Set(Intersection::Type::Fall, BufferADC::SIZE - 2, BufferADC::SIZE - 1);
}


Intersection ResolverPeriodSamples::FindFirstIntersectionRelativeAverage(const ValueADC &zero)
{
    Intersection result;

    for (int i = 1; i < BufferADC::SIZE; i++)
    {
        int current = BufferADC::At(i);
        int prev = BufferADC::At(i - 1);

        if (prev < zero && current >= zero)
        {
            result.Set(Intersection::Type::Rise, i - 1, i);
            break;
        }

        if (prev > zero && current <= zero)
        {
            result.Set(Intersection::Type::Fall, i - 1, i);
            break;
        }
    }

    return result;
}


Intersection ResolverPeriodSamples::FindLastIntersectionRelativeAverage(const ValueADC &zero, const Intersection &first)
{
    Intersection result;

    if (first.type == Intersection::Type::Rise)
    {
        for (int i = BufferADC::SIZE - 1; i > 0; i--)
        {
            int current = BufferADC::At(i);
            int prev = BufferADC::At(i - 1);

            if (prev < zero && current >= zero)
            {
                result.Set(Intersection::Type::Rise, i - 1, i);
                break;
            }
        }
    }
    else
    {
        for (int i = BufferADC::SIZE - 1; i > 0; i--)
        {
            int current = BufferADC::At(i);
            int prev = BufferADC::At(i - 1);

            if (prev > zero && current <= zero)
            {
                result.Set(Intersection::Type::Fall, i - 1, i);
                break;
            }
        }
    }

    return result;
}


bool ResolverPeriodSamples::CalculateRoughly(const ValueADC &dc, Period &period)
{
    Intersection first_around = FindFirstIntersectionRelativeAverage(dc);

    Intersection last_around = FindLastIntersectionRelativeAverage(dc, first_around);

    period.Set(first_around, last_around, dc);

    if (BadIntersection(first_around, last_around))
    {
        return false;
    }

    return true;
}


void ResolverPeriodSamples::CalculateAccuracy(const ValueADC &dc)
{
    Intersection first = FindFirstIntersectionRelativeAverage(dc);

    Intersection last = FindLastIntersectionRelativeAverage(dc, first);

    result_period.Set(first, last, dc);

    if (BadIntersection(first, last))
    {
        SetFullPeriod(dc);
    }
}


DualIntegral::DualIntegral(const Period &period)
{
    Recalculate(period);
}


void DualIntegral::Recalculate(const Period &period)
{
    positive = CalculatePositive(period);
    negative = CalculateNegative(period);

    delta = positive - negative;
}


uint DualIntegral::CalculatePositive(const Period &period)
{
    int i_first = period.first.first;
    int i_last = period.last.second;

    int zero = period.dc;

    uint sum = 0;

    for (int i = i_first; i < i_last; i++)
    {
        if (BufferADC::At(i) > zero)
        {
            sum += (uint)(BufferADC::At(i) - zero);
        }
    }

    return sum;
}


uint DualIntegral::CalculateNegative(const Period &period)
{
    int i_first = period.first.first;
    int i_last = period.last.second;

    int zero = period.dc;

    uint sum = 0;

    for (int i = i_first; i < i_last; i++)
    {
        if (BufferADC::At(i) < zero)
        {
            sum += (uint)(zero - BufferADC::At(i));
        }
    }

    return sum;
}


ResolverDC::ResolverDC(const Period &_period)
{
    int min = BufferADC::Min();
    int max = BufferADC::Max();

    Period period = _period;

    DualIntegral integral(period);

    int dc_value = period.dc.Raw();

    int counter = 0;

    while (max - min > 1)
    {
        if (integral.Delta() > 0)
        {
            min = dc_value;
        }
        else
        {
            max = dc_value;
        }

        dc_value = (max + min) / 2;

        period.dc = ValueADC::FromRaw(dc_value);

        integral.Recalculate(period);

        counter++;
    }

    LOG_WRITE("counter %d, delta %d", counter, max - min);

    result = period.dc;
}
