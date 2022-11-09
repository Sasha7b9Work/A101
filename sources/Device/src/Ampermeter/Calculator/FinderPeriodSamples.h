// 2022/11/09 20:49:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"
#include "Ampermeter/BufferADC.h"

/*
*   ƒл€ нахождени€ периода использует только считанные точки
*/


struct Intersection
{
    enum class Type
    {
        Rise,           // ѕересечение из отрицательной области в положительную
        Fall,           // ѕересечение из положительной области в отрицательную
        Empty
    };

    void Set(Type _type, int _first, int _second)
    {
        type = _type;
        first = _first;
        second = _second;
    }

    Type type = Type::Empty;

    int first = 0;      // »ндекс первой точки (до пересечени€)
    int second = 0;     // »ндекс последней точки (после пересечени€)
};


struct Period
{
    Period() {}
    Period(Intersection &_first, Intersection _last, ValueADC _dc) :
        first(_first), last(_last), dc(_dc)   { }

    Intersection first; // ѕервое пересечение с нулЄм
    Intersection last;  // ¬торое пересечение с нулЄм

    ValueADC dc;        // ”ровень нул€
};


class FinderPeriodSamples
{
public:
    FinderPeriodSamples(const BufferADC &);
    const Period GetResult() const { return period; }
private:
    Period period;

    // Ќайти первое пересечение с уровнем zero
    Intersection FindFirstIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero);

    // Ќайти последнее пересечение с уровнем zero, соответствующее первому пересечению first (они должны быть одного типа)
    Intersection FindLastIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero, const Intersection &first);

    bool BadIntersection(const Intersection &first, const Intersection &second);

    // ”станавливает результатом весь размер BufferADC
    void SetFullPeriod(ValueADC dc);

    // Ќайти интеграл положительной части сигнала (над нулевой линией Period::dc)
    int FindPositiveIntegral(const Period &);

    // Ќайти интеграл отрицательной части сигнала (под нулевой линией Period::ac)
    int FindNegativeIntegral(const Period &);
};

