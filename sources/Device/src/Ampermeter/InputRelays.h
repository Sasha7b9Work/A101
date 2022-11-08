// 2022/10/31 12:42:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace InputRelays
{
    void SetRange(int);

    int GetRange();

    // �������� ����� "����" (��������� ����������)
    void EnableZero();

    // ��������� ����� "����" (��������� ����������)
    void DisableZero();

    bool IsEnabledZero();
}
