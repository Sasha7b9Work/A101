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
        Fall
    };

    Type type;

    int first;          // Индекс первой точки (до пересечения)
    int second;         // Индекс последней точки (после пересечения)
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
    FinderPeriodSamples(const BufferADC &buffer);
    const Period GetResult() const { return result; };
private:
    Period result;
};

