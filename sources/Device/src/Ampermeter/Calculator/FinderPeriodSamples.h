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
        Fall,
        Empty
    };

//    Intersection() : type(Type::Empty), first(0), second(0) {}

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

    // Найти последнее пересечение с уровнем zero, соответствующее первому пересечению first (они должны быть разного типа)
    Intersection FindLastIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero, const Intersection &first);
};

