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
    Period() {}
    Period(Intersection &_first, Intersection _last, ValueADC _dc) :
        first(_first), last(_last), dc(_dc)   { }

    Intersection first; // ������ ����������� � ����
    Intersection last;  // ������ ����������� � ����

    ValueADC dc;        // ������� ����
};


class ResolverPeriodSamples
{
public:
    ResolverPeriodSamples(const BufferADC &);
    const Period GetResult() const { return result_period; }
private:
    Period result_period;

    // ����� ������ ����������� � ������� zero
    Intersection FindFirstIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero);

    // ����� ��������� ����������� � ������� zero, ��������������� ������� ����������� first (��� ������ ���� ������ ����)
    Intersection FindLastIntersectionRelativeAverage(const BufferADC &, const ValueADC &zero, const Intersection &first);

    bool BadIntersection(const Intersection &first, const Intersection &second);

    // ������������� ����������� ���� ������ BufferADC
    void SetFullPeriod(ValueADC dc);

    // ���������� �����
    void CalculateRoughly(const BufferADC &, const ValueADC &dc);

    // ���������� �����
    void CalculateAccuracy(const BufferADC &, const ValueADC &zero);
};

