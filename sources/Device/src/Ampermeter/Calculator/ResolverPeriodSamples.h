// 2022/11/09 20:49:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"
#include "Ampermeter/BufferADC.h"

/*
*   Для нахождения периода использует только считанные точки
*/


struct Intersection
{
    enum class Type
    {
        Rise,           // Пересечение из отрицательной области в положительную
        Fall,           // Пересечение из положительной области в отрицательную
        Empty
    };

    void Set(Type _type, int _first, int _second)
    {
        type = _type;
        first = _first;
        second = _second;
    }

    Type type = Type::Empty;

    int first = 0;      // Индекс первой точки (до пересечения)
    int second = 0;     // Индекс последней точки (после пересечения)
};


struct Period
{
    Period() = default;
    Period(Intersection &_first, Intersection _last, ValueADC _dc) :
        first(_first), last(_last), dc(_dc)   { }

    void Set(const Intersection &_first, Intersection _last, ValueADC _dc)
    {
        first = _first;
        last = _last;
        dc = _dc;
    }

    int Lenght() const { return last.first - first.first; }

    REAL GetFrequency() const;

    Intersection first; // Первое пересечение с нулём
    Intersection last;  // Второе пересечение с нулём

    ValueADC dc;        // Уровень нуля
};


class ResolverPeriodSamples
{
public:
    ResolverPeriodSamples();
    const Period GetResult() const { return result_period; }
private:
    Period result_period;

    // Найти первое пересечение с уровнем zero
    Intersection FindFirstIntersectionRelativeAverage(const ValueADC &zero);

    // Найти последнее пересечение с уровнем zero, соответствующее первому пересечению first (они должны быть одного типа)
    Intersection FindLastIntersectionRelativeAverage(const ValueADC &zero, const Intersection &first);

    bool BadIntersection(const Intersection &first, const Intersection &second);

    // Устанавливает результатом весь размер BufferADC
    void SetFullPeriod(ValueADC dc);

    // Рассчитать грубо
    bool CalculateRoughly(const ValueADC &dc, Period &);

    // Рассчитать точно
    void CalculateAccuracy(const ValueADC &zero);
};

