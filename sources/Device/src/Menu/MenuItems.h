// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct ButtonOld
{
public:

    // _highlight - � ���� ��������� ������ ��������� ��� ������ ��������� �� ������
    ButtonOld(pchar _name, pchar _signal, void (*_funcOnPress)(), int _x = -1, int _y = -1) :
        name(_name), signal(_signal), funcOnPress(_funcOnPress), x(_x), y(_y)
    {
    }

    void Press();

    // ������, ������� ��������� ������ ��� �������
    pchar Signal() const { return signal; }

    void SetText(pchar) const;

    void SetVisible(bool);

    bool IsVisible() const {return is_visible; }

    // 1 - "������", 0 - "���������"
    void SetValue(int);

    int GetValue() const { return value; }

    void Draw();

    pchar Text() const { return name; }

private:

    pchar name;                     // ��� ������ � ���������
    pchar signal;                   // ����� ������ ��������� ������ ��� �������
    void (*funcOnPress)();

    int value = 0;

    const int x;
    const int y;

    bool is_visible = true;

    // ���������� true, ���� ��� "����������� ������" - �������������� �������, � �� ��������
    bool IsSoftware() const;
};


struct Page
{
    Page(ButtonOld **_buttons, void (*_funcOnEnter)(), void (*_funcOnDraw)()) :
        buttons(_buttons), funcOnEnter(_funcOnEnter), funcOnDraw(_funcOnDraw)
    {
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    ButtonOld *GetButton(int index);

    void SetButton(int index, ButtonOld *);

    void Draw();

    ButtonOld *GetButton(pchar signal);

private:

    ButtonOld **buttons;

    // ���������� ��� ��������� �� ������
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetButtonsCount();
};
