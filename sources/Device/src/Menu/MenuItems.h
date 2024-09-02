// 2022/10/24 11:25:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/Log.h"
#include "Nextion/Controls.h"
#include "Utils/Math.h"
#include "Nextion/Colors.h"
#include "Settings/Settings.h"
#include "Ampermeter/Ampermeter.h"


struct Page;
struct ButtonRange;
struct ButtonOld;


struct TypeItem
{
    enum E
    {
        Another,
        ButtonPress,    // Кнопка без фиксации
        ButtonToggle,   // Кнопка с фиксацией
        ButtonOld,      // Старая кнопка - реализована в прошивке дисплея
        Label,          // Текстовая строка
        LabelMeasure,
        Count
    };
};


struct Item
{
    Item(TypeItem::E = TypeItem::Count, const Rect & = Rect(), void (*_funcOnPress)() = nullptr);

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
    ButtonCommon(TypeItem::E, pchar title_ru, pchar title_en, Font::E, const Rect &, void (*_funcOnPress)());

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
    ButtonOld(pchar _name, pchar _signal, void (*_funcOnPress)(), int16 _x = -1, int16 _y = -1) :
        ButtonCommon(TypeItem::ButtonOld, _name, _name, Font::_1, { _x, _y, 0, 0 }, _funcOnPress),
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

    const int16 x;
    const int16 y;

    bool is_visible = true;

    // Возвращает true, если это "программная кнопка" - отрисовывается вручную, а не дисплеем
    bool IsSoftware() const;
};


// Кнопка без фиксации (возвращается в отжатое состояние при отпускании)
struct ButtonPress : public ButtonCommon
{
    ButtonPress(pchar title_ru, pchar title_en, Font::E f, const Rect &, void (*_funcOnPress)(), TypeItem::E = TypeItem::ButtonPress);

    virtual pchar Signal() const override;

    virtual void Draw() override;

    virtual bool IsWithoutFixation() const override { return true; }
};


// Кнопка с фиксацией (при нажатии переключается в противоположное состояние, на отпускание реакциии нет)
struct ButtonToggle : public ButtonPress
{
    ButtonToggle(pchar title_ru, pchar title_en, Font::E f, const Rect &_rect, void (*_funcOnPress)()) :
        ButtonPress(title_ru, title_en, f, _rect, _funcOnPress, TypeItem::ButtonToggle)
    {
    }

    virtual bool IsWithoutFixation() const override { return false; }
};


struct ButtonRange : public ButtonToggle
{
    ButtonRange(pchar title_ru, pchar title_en, int x, int y, void (*_funcOnPress)()) :
        ButtonToggle(title_ru, title_en, Font::_1, { x, y, 127, 74 }, _funcOnPress)
    {
    }
};


struct Label : public Item
{
    static const int MAX_LEN = 32;

    Label(pchar _textRU = "", pchar _textEN = "", const Rect & = Rect(), Font::E = Font::_0, void (*_funcOnPress)() = EmptyFuncVV,
        const Color &_colorText = Color::White, const Color &_colorBack = Color::Count, bool _h_aligned = false);

    Label(const Label &);

    void SetText(const char _textRU[MAX_LEN], const char _textEN[MAX_LEN]);

    void SetX(int _x)
    {
        rect.x = (int16)_x;
    }
    int GetX() const
    {
        return rect.x;
    }
    int GetY() const
    {
        return rect.y;
    }
    int GetWidth() const
    {
        return rect.width;
    }
    pchar Text() const;
    pchar Text(Lang::E lang) const
    {
        return text[lang];
    }
    virtual void Draw() override;
    virtual bool IsWithoutFixation() const override { return true; }
private:
    char  text[Lang::Count][MAX_LEN];
    int   font;
    bool  h_aligned;
    Color colorText;
    Color colorBack;
};


// Условный размер поля для измерения
struct SizeMeasure
{
    enum E
    {
        Big,
        Small,
        Calibration,
        Count
    };
};


struct TypeLabelMeasure
{
    enum E
    {
        Name,           // Метка поля измерения - DC, F, etc
        Count
    };
};


struct LabelMeasure : public Label
{
    LabelMeasure(TypeMeasure::E, SizeMeasure::E, int _x, int _y, void (*_funcOnPress)() = EmptyFuncVV);

    void Reset();

    void Flash();

    void SetMeasure(const Measure &, int range);

    pchar GetSign() const;

    pchar GetDigits() const;

    pchar GetUnits() const;

    virtual void Draw() override;

private:

    char buf_measure[Label::MAX_LEN];

    Label labels[TypeLabelMeasure::Count];

    TypeMeasure type_measure;

    void SetMeasure(pchar);

    // after - количество цифр после запятой
    void ConvertRealToText(REAL value, char buffer[Label::MAX_LEN], int after, pchar suffix);

    void DrawSign(pchar);

    void DrawDigits(pchar);

    void DrawUnits(pchar, pchar);
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
