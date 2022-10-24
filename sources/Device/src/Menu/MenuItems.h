// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Button
{
public:
    Button(void (*_applyAction)(int)) : applyAction(_applyAction) {};

    // ���������� ������ ��� ������� ����������� �� ������� �������
    static Button *ForIndex(int);

    // ��������� ��������� � ������� �����������
    void ApplyAction(int action) { applyAction(action); }

private:

    void (*applyAction)(int);
};
