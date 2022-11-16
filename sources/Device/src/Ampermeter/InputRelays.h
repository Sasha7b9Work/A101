// 2022/10/31 12:42:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace InputRelays
{
    namespace Range
    {
        void Set(int);

        // ������� ��������
        int Current();

        // ���������� ��������
        int Prev();
    }

    // �������� ����� "����" (��������� ����������)
    void EnableZero();

    // ��������� ����� "����" (��������� ����������)
    void DisableZero();

    bool IsEnabledZero();
}
