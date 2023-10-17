// 2022/10/31 12:42:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Range
{
    static void Set(int);

    // ������� ��������
    static int Current();

    // ���������� ��������
    static int Prev();

    static void Load() { Set(current); }

    static float Max(int);

private:

    static int current;
    static int prev;
};


namespace InputRelays
{
    // �������� ����� "����" (��������� ����������)
    void EnableZero();

    // ��������� ����� "����" (��������� ����������)
    void DisableZero();

    bool ZeroIsEanbled();
}
