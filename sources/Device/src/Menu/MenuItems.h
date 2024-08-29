// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"
#include "Nextion/Controls.h"
#include "Utils/Math.h"


struct Page;
struct ButtonRange;
struct ButtonOld;


struct TypeItem
{
    enum E
    {
        Another,
        Button,
        ButtonToggle,
        ButtonOld,
        Count
    };
};


struct Item
{
    Item(TypeItem::E, int x, int y, int w, int h, void (*_funcOnPress)());

    void SetParent(Page *page)
    {
        parent = page;
    }

    const Rect &GetRect() const
    {
        return rect;
    }

    void SetShown(bool show);
    bool IsShown() const;

    virtual void Draw() = 0;
    virtual bool IsWithoutFixation() const = 0;

    virtual void Press();
    virtual void Release();

    static void SetAllInactive();

    static void OnEventPress(int, int);
    static void OnEventRelease(int, int);

    ButtonRange *ToButtonRange();

    ButtonOld *ToButtonOld();

protected:

    TypeItem::E type;

    Rect rect;
    Page *parent;

    void (*funcOnPress)();

    bool is_pressed = false;        // Для обычной кнопки переходит в состяние false сразу после отпускания, для кнопки с фиксацией -
                                    // после повторного нажатия

    bool is_shown = true;           // Если true, то надо отрисовывать
};


struct ButtonCommon : public Item
{
    ButtonCommon(TypeItem::E, pchar title_ru, pchar title_en, Font::E, int x, int y, int w, int h, void (*_funcOnPress)());

    // 1 - "нажать", 0 - "отпустить"
    virtual void SetValue(int);

    // Сиганл, который присылает кнопка при нажатии
    virtual pchar Signal() const = 0;

protected:

    pchar title[2];

    Font::E font;
};


struct ButtonOld : public ButtonCommon
{
public:

    // _highlight - в этом состоянии кнопка находится при первом появлении на экране
    ButtonOld(pchar _name, pchar _signal, void (*_funcOnPress)(), int _x = -1, int _y = -1) :
        ButtonCommon(TypeItem::ButtonOld, _name, _name, Font::_1, _x, _y, 0, 0, _funcOnPress),
        name(_name), signal(_signal), x(_x), y(_y)
    {
    }

    // Сиганл, который присылает кнопка при нажатии
    virtual pchar Signal() const override { return signal; }

    virtual void Press() override;

    virtual void Release() override { }

    void SetText(pchar) const;

    void SetVisible(bool);

    bool IsVisible() const {return is_visible; }

    virtual void SetValue(int) override;

    int GetValue() const { return value; }

    virtual void Draw() override    ;

    pchar Text() const { return name; }

    virtual bool IsWithoutFixation() const override { return true; }

private:

    pchar name;                     // Имя кнопки в редакторе
    pchar signal;                   // Такой сигнал присылает кнопка при нажатии

    int value = 0;

    const int x;
    const int y;

    bool is_visible = true;

    // Возвращает true, если это "программная кнопка" - отрисовывается вручную, а не дисплеем
    bool IsSoftware() const;
};


struct Button : public ButtonCommon
{
    Button(pchar title_ru, pchar title_en, Font::E f, int x, int y, int w, int h, void (*_funcOnPress)());

    virtual pchar Signal() const override;

    virtual void Draw() override;

    virtual bool IsWithoutFixation() const override { return true; }
};


struct ButtonToggle : public Button
{
    ButtonToggle(pchar title_ru, pchar title_en, Font::E f, int x, int y, int w, int h, void (*_funcOnPress)()) :
        Button(title_ru, title_en, f, x, y, w, h, _funcOnPress)
    {
    }

    virtual bool IsWithoutFixation() const override { return false; }
};


struct ButtonRange : public ButtonToggle
{
    ButtonRange(pchar title_ru, pchar title_en, int x, int y, void (*_funcOnPress)()) :
        ButtonToggle(title_ru, title_en, Font::_1, x, y, 127, 74, _funcOnPress)
    {
    }
};


struct Page
{
    Page(Item **_items, void (*_funcOnEnter)(), void (*_funcOnDraw)()) :
        items(_items), funcOnEnter(_funcOnEnter), funcOnDraw(_funcOnDraw)
    {
        for (int i = 0; i < GetItemCount(); i++)
        {
            GetItem(i)->SetParent(this);
        }
    }

    static Page *Current() { return current; }

    void SetAsCurrent();

    Item *GetItem(int index);

    void SetItem(int index, Item *);

    void Draw();

    ButtonCommon *GetButton(pchar signal);

private:

    Item **items;

    // Вызывается при появлении на экране
    void (*funcOnEnter)();

    void (*funcOnDraw)();

    static Page *current;

    int GetItemCount();
};
