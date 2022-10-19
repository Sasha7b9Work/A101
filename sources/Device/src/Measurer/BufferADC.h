// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct BufferADC
{
    static const int SIZE = 1024;

    BufferADC() : pointer(0) { } //-V730

    void Push(uint16 word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    bool IsFull()
    {
        return (pointer == SIZE);
    }

    void Clear() { pointer = 0; }

    void ConvertToVoltage();

private:
    int pointer;            // ��������� ������������ ��� ������ ������ (������ raw)
    uint   raw[SIZE];       // ������, ��������� � ���
    float  volt[SIZE];      // �������� �������� � ���
};
