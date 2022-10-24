// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Button
{
public:

    Button(int _index, void (*_applyAction)(int)) : index(_index), applyAction(_applyAction) {};

    // ���������� ������ ��� ������� ����������� �� ������� �������
    static Button *ForIndex(int);

    // ��������� ��������� � ������� �����������
    void ApplyAction(int action) { applyAction(action); }

    int GetIndex() const { return index; }

    // ������� ���������
    void RemoveBacklight();

private:

    int index;

    void (*applyAction)(int);
};
