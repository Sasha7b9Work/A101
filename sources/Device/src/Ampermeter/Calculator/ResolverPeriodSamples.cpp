// 2022/11/09 20:49:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverPeriodSamples.h"


// —труктура дл€ расчЄт положительного и отрицательного интегралов
struct DualIntegral
{
    DualIntegral(const BufferADC &, const Period &);

    void Recalculate(const BufferADC &, const Period &);

    // ¬озвращает абсолютное значение разницы между positive и negative. 
    int64 Delta() const { return delta; }

private:

    uint CalculatePositive(const BufferADC &, const Period &);
    uint CalculateNegative(const BufferADC &, const Period &);

    int64 positive = 0;
    int64 negative = 0;
    int64 delta = 0;
};


FinderPeriodSamples::FinderPeriodSamples(const BufferADC &buffer)
{
    int sum = 0;

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        sum += buffer[i];
    }

    int averaging = sum / BufferADC::SIZE;

    Intersection first_around = FindFirstIntersectionRelativeAverage(buffer, ValueADC::FromRaw(averaging));

    Intersection last_around = FindLastIntersectionRelativeAverage(buffer, ValueADC::FromRaw(averaging), first_around);

    if (BadIntersection(first_around, last_around))
    {
        SetFullPeriod(ValueADC::FromRaw(averaging));

        return;
    }

    Period period(first_around, last_around, ValueADC::FromRaw(averaging));

    DualIntegral integral(buffer, period);

    if (integral.Delta() == 0)
    {
        CalculateAccuracy(buffer, period.dc);
    }
    else
    {
        if (integral.Delta() > 0)
        {
            while (integral.Delta() > 0)
            {
                period.dc = ValueADC::FromRaw(period.dc - 1);

                integral.Recalculate(buffer, period);
            }

            CalculateAccuracy(buffer, period.dc);
        }
        else
        {
            while (integral.Delta() < 0)
            {
                period.dc = ValueADC::FromRaw(period.dc + 1);

                integral.Recalculate(buffer, period);
            }

            CalculateAccuracy(buffer, period.dc);
        }
    }
}


bool FinderPeriodSamples::BadIntersection(const Intersection &first, const Intersection &second)
{
    return (first.type == Intersection::Type::Empty) || (second.type == Intersection::Type::Empty);
}


void FinderPeriodSamples::SetFullPeriod(ValueADC _dc)
{
    result_period.dc = _dc;
    result_period.first.Set(Intersection::Type::Rise, 0, 1);
    result_period.last.Set(Intersection::Type::Fall, BufferADC::SIZE - 2, BufferADC::SIZE - 1);
}


Intersection FinderPeriodSamples::FindFirstIntersectionRelativeAverage(const BufferADC &buffer, const ValueADC &zero)
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


Intersection FinderPeriodSamples::FindLastIntersectionRelativeAverage(const BufferADC &buffer, const ValueADC &zero, const Intersection &first)
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


void FinderPeriodSamples::CalculateAccuracy(const BufferADC &, const ValueADC &)
{

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
