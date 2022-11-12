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

    float Positive() const { return positive; }

    float Negative() const { return negative; }

    // Возвращает абсолютное значение разницы между positive и negative. 
    float Delta() const { return delta; }

private:

    float CalculatePositive(const Period &);
    float CalculateNegative(const Period &);

    float positive = 0;
    float negative = 0;
    float delta = 0;
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
    float sum = 0.0f;

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        sum += BufferADC::At(i).Real();
    }

    float averaging = sum / (float)BufferADC::SIZE;

    Period period;

    if (!CalculateRoughly(ValueADC::FromReal(averaging), period))
    {
        SetFullPeriod(ValueADC::FromReal(averaging));

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
        float current = BufferADC::At(i).Real();
        float prev = BufferADC::At(i - 1).Real();

        if (prev < zero.Real() && current >= zero.Real())
        {
            result.Set(Intersection::Type::Rise, i - 1, i);
            break;
        }

        if (prev > zero.Real() && current <= zero.Real())
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
            float current = BufferADC::At(i).Real();
            float prev = BufferADC::At(i - 1).Real();

            if (prev < zero.Real() && current >= zero.Real())
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
            float current = BufferADC::At(i).Real();
            float prev = BufferADC::At(i - 1).Real();

            if (prev > zero.Real() && current <= zero.Real())
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


float DualIntegral::CalculatePositive(const Period &period)
{
    int i_first = period.first.first;
    int i_last = period.last.second;

    float zero = period.dc.Real();

    float sum = 0.0f;

    for (int i = i_first; i < i_last; i++)
    {
        if (BufferADC::At(i).Real() > zero)
        {
            sum += (uint)(BufferADC::At(i).Real() - zero);
        }
    }

    return sum;
}


float DualIntegral::CalculateNegative(const Period &period)
{
    int i_first = period.first.first;
    int i_last = period.last.second;

    float zero = period.dc.Real();

    float sum = 0.0f;

    for (int i = i_first; i < i_last; i++)
    {
        if (BufferADC::At(i).Real() < zero)
        {
            sum += (uint)(zero - BufferADC::At(i).Real());
        }
    }

    return sum;
}


ResolverDC::ResolverDC(const Period &_period)
{
    float min = BufferADC::Min().Real();
    float max = BufferADC::Max().Real();

    Period period = _period;

    DualIntegral integral(period);

    float dc_value = period.dc.Real();

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

        period.dc = ValueADC::FromReal(dc_value);

        integral.Recalculate(period);

        counter++;
    }

    period.dc = ValueADC::FromReal(period.dc.Real());

    result = period.dc;

    {
//        int64 sum = 0;
//
//        int num_elements = 0;
//
//        for (int i = _period.first.first; i < _period.last.first; i++)
//        {
//            sum += BufferADC::At(i);
//
//            num_elements++;
//        }
//
//        result = ValueADC::FromRaw((int)(sum / num_elements));
    }
}
