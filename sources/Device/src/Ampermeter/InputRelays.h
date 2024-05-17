// 2022/10/31 12:42:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Range
{
    static void Set(int, bool reset_measures = true);

    // ������� ��������
    static int Current();

    static void Load(bool reset_measures) { Set(current, reset_measures); }

    static REAL Max(int);

private:

    static int current;
    static int prev;
};


namespace InputRelays
{
    // �������� ����� "����" (��������� ����������)
    void EnableZero(bool reset_measures);

    // ��������� ����� "����" (��������� ����������)
    void DisableZero(bool reset_measures);

    bool ZeroIsEanbled();
}
