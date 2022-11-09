// 2022/11/09 20:49:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/FinderPeriodSamples.h"


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
}


bool FinderPeriodSamples::BadIntersection(const Intersection &first, const Intersection &second)
{
    return (first.type == Intersection::Type::Empty) || (second.type == Intersection::Type::Empty);
}


void FinderPeriodSamples::SetFullPeriod(ValueADC _dc)
{
    period.dc = _dc;
    period.first.Set(Intersection::Type::Rise, 0, 1);
    period.last.Set(Intersection::Type::Fall, BufferADC::SIZE - 2, BufferADC::SIZE - 1);
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
            result.type = Intersection::Type::Rise;
            result.first = prev;
            result.second = current;
            break;
        }

        if (prev > zero && current <= zero)
        {
            result.type = Intersection::Type::Fall;
            result.first = prev;
            result.second = current;
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

            if (prev > zero && current <= zero)
            {
                result.type = Intersection::Type::Fall;
                result.first = prev;
                result.second = current;
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

            if (prev < zero && current >= zero)
            {
                result.type = Intersection::Type::Rise;
                result.first = prev;
                result.second = current;
                break;
            }
        }
    }

    return result;
}
