// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct BufferADC
{
    static const int SIZE = 1024;

    BufferADC() : pointer(0) { } //-V730

    void Push(int word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    bool IsFull()     { return (pointer == SIZE); }

    void Clear(float sample_rate) { pointer = 0; sample_rate_HZ = sample_rate; }

    int Size() const  { return SIZE; }

    float At(int pos) { return volt[pos]; }

    void ConvertToVoltage();

    float MinReal() const { return min; }

    float MaxReal() const { return max; }

    float sample_rate_HZ;

private:
    int    pointer;         // ��������� ������������ ��� ������ ������ (������ raw)
    int    raw[SIZE];       // ������, ��������� � ���
    float  volt[SIZE];      // �������� �������� � ���
    int    min_raw;
    int    max_raw;
    float  min;
    float  max;
};
