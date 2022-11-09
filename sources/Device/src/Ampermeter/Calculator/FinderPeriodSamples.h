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
        Fall
    };

    Type type;

    int first;          // ������ ������ ����� (�� �����������)
    int second;         // ������ ��������� ����� (����� �����������)
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
    FinderPeriodSamples(const BufferADC &buffer);
    const Period GetResult() const { return result; };
private:
    Period result;
};

