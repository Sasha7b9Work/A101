// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"


struct ButtonCommon
{
    virtual void Press() = 0;

    // 1 - "нажать", 0 - "отпустить"
    virtual void SetValue(int) = 0;

    // Сиганл, который присылает кнопка при нажатии
    virtual pchar Signal() const = 0;

    virtual void Draw() = 0;
};


struct ButtonOld : public ButtonCommon
{
public:

    // _highlight - в этом состоянии кнопка находится при первом появлении на экране
    ButtonOld(pchar _name, pchar _signal, void (*_funcOnPress)(), int _x = -1, int _y = -1) :
        name(_name), signal(_signal), funcOnPress(_funcOnPress), x(_x), y(_y)
    {
    }

    // Сиганл, который присылает кнопка при нажатии
    virtual pchar Signal() const override { return signal; }

    virtual void Press() override;

    void SetText(pchar) const;

    void SetVisible(bool);

    bool IsVisible() const {return is_visible; }

    virtual void SetValue(int) override;

    int GetValue() const { return value; }

    virtual void Draw() override;

    pchar Text() const { return name; }

private:

    pchar name;                     // Имя кнопки в редакторе
    pchar signal;                   // Такой сигнал присылает кнопка при нажатии
    void (*funcOnPress)();

    int value = 0;

    const int x;
    const int y;

    bool is_visible = true;

    // Возвращает true, если это "программная кнопка" - отрисовывается вручную, а не дисплеем
    bool IsSoftware() const;
};


struct Page
{
    Page(ButtonCommon **_buttons, void (*_funcOnEnter)(), void (*_funcOnDraw)()) :
        buttons(_buttons), funcOnEnter(_funcOnEnter), funcOnDraw(_funcOnDraw)
    {
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    ButtonCommon *GetButton(int index);

    void SetButton(int index, ButtonOld *);

    void Draw();

    ButtonCommon *GetButton(pchar signal);

private:

    ButtonCommon **buttons;

    // Вызывается при появлении на экране
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetButtonsCount();
};
