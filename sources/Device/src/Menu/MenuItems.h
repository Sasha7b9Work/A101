// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Button
{
public:

    Button(int _index, void (*_funcOnPress)()) : index(_index), funcOnPress(_funcOnPress) {};

    // Возвращает кнопку для данного присланного из дисплея индекса
    static Button *ForIndex(int);

    // Применить пришедшее с дисплея воздействие
    void Press() { funcOnPress(); }

    int GetIndex() const { return index; }

    // Удалить подсветку
    void RemoveBacklight();

    // Подсветить
    void SetBacklight();

private:

    int index;

    void (*funcOnPress)();
};
