// 2022/11/09 20:49:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"
#include "Hardware/Timer.h"
#include <cmath>


// Структура для расчётa положительного и отрицательного интегралов
struct DualIntegral
{
    DualIntegral(const BufferADC &, const Period &);

    void Recalculate(const BufferADC &, const Period &);

    int64 Positive() const { return positive; }

    int64 Negative() const { return negative; }

    // Возвращает абсолютное значение разницы между positive и negative. 
    int64 Delta() const { return delta; }

private:

    uint CalculatePositive(const BufferADC &, const Period &);
    uint CalculateNegative(const BufferADC &, const Period &);

    int64 positive = 0;
    int64 negative = 0;
    int64 delta = 0;
};


class ResolverDC
{
public:

    ResolverDC(const BufferADC &, const Period &);

    ValueADC GetResult() const { return result; }

private:

    ValueADC result;
};


ResolverPeriodSamples::ResolverPeriodSamples(const BufferADC &buffer)
{
    int sum = 0;

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        sum += buffer[i];
    }

    int averaging = sum / BufferADC::SIZE;

    Period period;

    if (!CalculateRoughly(buffer, ValueADC::FromRaw(averaging), period))
    {
        SetFullPeriod(ValueADC::FromRaw(averaging));

        return;
    }

    ValueADC dc = ResolverDC(buffer, period).GetResult();

    CalculateAccuracy(buffer, dc);
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


Intersection ResolverPeriodSamples::FindFirstIntersectionRelativeAverage(const BufferADC &buffer, const ValueADC &zero)
{
    Intersection result;

    for (int i = 1; i < BufferADC::SIZE; i++)
    {
        int current = buffer[i];
        int prev = buffer[i - 1];

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


Intersection ResolverPeriodSamples::FindLastIntersectionRelativeAverage(const BufferADC &buffer, const ValueADC &zero, const Intersection &first)
{
    Intersection result;

    if (first.type == Intersection::Type::Rise)
    {
        for (int i = BufferADC::SIZE - 1; i > 0; i--)
        {
            int current = buffer[i];
            int prev = buffer[i - 1];

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
            int current = buffer[i];
            int prev = buffer[i - 1];

            if (prev > zero && current <= zero)
            {
                result.Set(Intersection::Type::Fall, i - 1, i);
                break;
            }
        }
    }

    return result;
}


bool ResolverPeriodSamples::CalculateRoughly(const BufferADC &buffer, const ValueADC &dc, Period &period)
{
    Intersection first_around = FindFirstIntersectionRelativeAverage(buffer, dc);

    Intersection last_around = FindLastIntersectionRelativeAverage(buffer, dc, first_around);

    period.Set(first_around, last_around, dc);

    if (BadIntersection(first_around, last_around))
    {
        return false;
    }

    return true;
}


void ResolverPeriodSamples::CalculateAccuracy(const BufferADC &buffer, const ValueADC &dc)
{
    Intersection first = FindFirstIntersectionRelativeAverage(buffer, dc);

    Intersection last = FindLastIntersectionRelativeAverage(buffer, dc, first);

    result_period.Set(first, last, dc);

    if (BadIntersection(first, last))
    {
        SetFullPeriod(dc);
    }
}


DualIntegral::DualIntegral(const BufferADC &buffer, const Period &period)
{
    Recalculate(buffer, period);
}


void DualIntegral::Recalculate(const BufferADC &buffer, const Period &period)
{
    positive = CalculatePositive(buffer, period);
    negative = CalculateNegative(buffer, period);

    delta = positive - negative;
}


uint DualIntegral::CalculatePositive(const BufferADC &buffer, const Period &period)
{
    int i_first = period.first.first;
    int i_last = period.last.second;

    int zero = period.dc;

    uint sum = 0;

    for (int i = i_first; i < i_last; i++)
    {
        if (buffer[i] > zero)
        {
            sum += (uint)(buffer[i] - zero);
        }
    }

    return sum;
}


uint DualIntegral::CalculateNegative(const BufferADC &buffer, const Period &period)
{
    int i_first = period.first.first;
    int i_last = period.last.second;

    int zero = period.dc;

    uint sum = 0;

    for (int i = i_first; i < i_last; i++)
    {
        if (buffer[i] < zero)
        {
            sum += (uint)(zero - buffer[i]);
        }
    }

    return sum;
}


ResolverDC::ResolverDC(const BufferADC &buffer, const Period &_period)
{
    int min = buffer.Min();
    int max = buffer.Max();

    Period period = _period;

    DualIntegral integral(buffer, period);

    int dc_value = period.dc.Raw();

    int counter = 0;

    while (max - min > 2)
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

        integral.Recalculate(buffer, period);

        counter++;
    }

    LOG_WRITE("counter %d, delta %d", counter, max - min);

    result = period.dc;
}
