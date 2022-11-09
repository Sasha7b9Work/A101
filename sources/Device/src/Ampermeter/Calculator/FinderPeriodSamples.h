// 2022/11/09 20:49:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/AD7691.h"
#include "Ampermeter/BufferADC.h"

/*
*   ��� ���������� ������� ���������� ������ ��������� �����
*/


struct Intersection
{
    enum class Type
    {
        Rise,           // ����������� �� ������������� ������� � �������������
        Fall,           // ����������� �� ������������� ������� � �������������
        Empty
    };

    void Set(Type _type, int _first, int _second)
    {
        type = _type;
        first = _first;
        second = _second;
    }

    Type type = Type::Empty;

    int first = 0;      // ������ ������ ����� (�� �����������)
    int second = 0;     // ������ ��������� ����� (����� �����������)
};


struct Period
{
    Intersection first; // ������ ����������� � ����
    Intersection last;  // ������ ����������� � ����

    ValueADC dc;        // ������� ����
};


class FinderPeriodSamples
{
public:
    FinderPeriodSamples(const BufferADC &);
    const Period GetResult() const { return period; }
private:
    Period period;

    // ����� ������ ����������� � ������� zero
    Intersection FindFirstIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero);

    // ����� ��������� ����������� � ������� zero, ��������������� ������� ����������� first (��� ������ ���� ������ ����)
    Intersection FindLastIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero, const Intersection &first);

    bool BadIntersection(const Intersection &first, const Intersection &second);

    // ������������� ����������� ���� ������ BufferADC
    void SetFullPeriod(ValueADC dc);
};

