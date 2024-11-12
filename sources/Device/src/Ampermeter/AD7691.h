// 2022/10/18 16:57:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SampleRate
{
    enum E
    {
        _10us,      // ������� ������� 40 �� - 5 ���
        _100us,     // ������� ������� 4 �� - 400 ��
        _1000us,    // ������� ������� 0.4 �� - 40 ��
        Count
    };


    // ����� � ������������� ����� ������� �������������
    static uint TimeUSonPoint();

    static void Set(E v) { value = v; }

    static E Get()       { return value; }

    // ���������� false, ���� ���������� ������� ������� �� ������� �������� ���������
    static bool InRange(REAL, pchar *title_ru, pchar *title_en);

    // ����� ������� ���������� ������
    static uint TimeFullRead();

private:

    static E value;
};


struct ValueADC
{
    // � ����������� ������� ��������� � ��� ��������, ������� ������������ �������������
    explicit ValueADC(int _reading = 0);
    static ValueADC FromRaw(int raw) { ValueADC result; result.value = raw; return result; }
    REAL Real() const { return 5.0 / (1 << 17) * (REAL)value; }
    int Raw()    const { return value; }
    bool operator <(const ValueADC &rhs) const { return value < rhs.value; }
    bool operator >(const ValueADC &rhs) const { return value > rhs.value; }
    operator int() const { return value; }
private:
    int value;        // ����������� ��������, ��� ���������������
};


// ���
namespace AD7691
{
    void Init();

    // ������ ������� � ������������ ��������
    ValueADC ReadValue();

    // ������ ����� �������� � ���
    int ReadValueRAW();

    // ���������� ���������� �������� �� ������������� ���������� �������
    int GetAverageValue();
}
