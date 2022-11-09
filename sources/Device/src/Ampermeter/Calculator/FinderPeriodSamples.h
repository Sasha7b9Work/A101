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
    Intersection first; // Первое пересечение с нулём
    Intersection last;  // Второе пересечение с нулём

    ValueADC dc;        // Уровень нуля
};


class FinderPeriodSamples
{
public:
    FinderPeriodSamples(const BufferADC &);
    const Period GetResult() const { return period; }
private:
    Period period;

    // Найти первое пересечение с уровнем zero
    Intersection FindFirstIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero);

    // Найти последнее пересечение с уровнем zero, соответствующее первому пересечению first (они должны быть одного типа)
    Intersection FindLastIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero, const Intersection &first);

    bool BadIntersection(const Intersection &first, const Intersection &second);

    // Устанавливает результатом весь размер BufferADC
    void SetFullPeriod(ValueADC dc);
};

