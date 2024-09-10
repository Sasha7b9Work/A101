// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Nextion/Nextion.h"
#include "Menu/Pages/Pages.h"
#include "Hardware/Timer.h"
#include "Utils/String.h"
#include "Nextion/Display.h"
#include "Ampermeter/InputRelays.h"
#include <cstring>
#include <cstdio>


using namespace std;


Page *Page::current = PageMain::self;


namespace PoolItems
{
    static const int MAX_ITEMS = 1024;                  // Здесь хранятся все созданные кнопки (со всех страниц)
    static Item *items[MAX_ITEMS];
    static int num_items = 0;                         // Столько создано кнопок в данный момент

    static void SetAllInactive()
    {
        for (int i = 0; i < num_items; i++)
        {
            items[i]->SetShown(false);
        }
    }

    static void AppendNewItem(Item *item)
    {
        items[num_items] = item;

        if (num_items++ >= MAX_ITEMS)
        {
            LOG_ERROR_TRACE("The items buffer is full");
        }
    }

    static bool Consist(const Item *item)
    {
        for (int i = 0; i < num_items; i++)
        {
            if (item == items[i])
            {
                return true;
            }
        }

        return false;
    }

    static Item *GetItem(int x, int y)
    {
        for (int i = 0; i < num_items; i++)
        {
            Item *item = items[i];

            if (item->IsShown() && item->GetRect().Intersect(x, y))
            {
                return item;
            }
        }

        return nullptr;
    }
}


bool Item::IsShown() const
{
    if (PoolItems::Consist(this))
    {
        return (Page::Current() == parent) && is_shown;
    }
    else
    {
        return is_shown;
    }
}


void Item::Press()
{
    if (type == TypeItem::ButtonToggle)
    {
        is_pressed = !is_pressed;
    }
    else
    {
        is_pressed = true;
    }

    need_draw = true;

    funcOnPress(this, is_pressed);
}


void Item::Release()
{
    if (type != TypeItem::ButtonToggle)
    {
        is_pressed = false;

        need_draw = true;

        funcOnPress(this, false);
    }
}


void ButtonToggle::SetToggled(bool toggled)
{
    is_pressed = toggled;

    need_draw = true;

    funcOnPress(this, is_pressed);
}


Item::Item(TypeItem::E _type, const Rect &_rect, void (*_funcOnPress)(Item *, bool), bool append) :
    type(_type), rect(_rect), funcOnPress(_funcOnPress)
{
    if (append)
    {
        PoolItems::AppendNewItem(this);
    }
}


Item &Item::operator=(const Item &rhs)
{
    type = rhs.type;
    rect = rhs.rect;
    parent = rhs.parent;
    funcOnPress = rhs.funcOnPress;
    is_pressed = rhs.is_pressed;
    is_shown = rhs.is_shown;

    return *this;
}


ButtonCommon::ButtonCommon(TypeItem::E _type, pchar title_ru, pchar title_en, Font::E _f, const Rect &_rect, void (*_funcOnPress)(Item *, bool)) :
    Item(_type, _rect, _funcOnPress),
    font(_f)
{
    title[Lang::RU] = title_ru;
    title[Lang::EN] = title_en;
}


void ButtonCommon::SetText(pchar title_ru, pchar title_en)
{
    title[Lang::RU] = title_ru;
    title[Lang::EN] = title_en;
}


void Item::OnEventPress(int x, int y)
{
    Item *item = PoolItems::GetItem(x, y);

    if (item)
    {
        item->Press();
    }
}


void Item::OnEventRelease(int x, int y)
{
    Item *item = PoolItems::GetItem(x, y);

    if (item)
    {
        item->Release();
    }
}


void Menu::Init()
{
    PageMain::SetRange(MeasuresOnDisplay::AC_DC, 2);
    PageMain::SetRange(MeasuresOnDisplay::AC_DC, 3);

    PageMain::self->SetAsCurrent();
}


Item *Page::GetItem(int index)
{
    return items[index];
}


void Page::SetItem(int index, Item *item)
{
    items[index] = item;
}


void Page::SetAsCurrent()
{
    PoolItems::SetAllInactive();

    current = this;

    Timer::Delay(50);           // Эта задержка нужна для того, чтобы дисплей успел переключиться на новую страницу.
                                // Иначе сообщения элементам управления будут посылаться на старую страницу

    Display::Clear();

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->SetShown(true);
    }

    current->funcOnEnter();
}


void Page::Draw()
{
    funcOnDraw();

    for (int i = 0; i < GetItemCount(); i++)
    {
        GetItem(i)->Draw();
    }
}


int Page::GetItemCount()
{
    int count = 0;

    Item **item = &items[0];

    while (*item++)
    {
        count++;
    }

    return count;
}


ButtonPress::ButtonPress(pchar title_ru, pchar title_en, Font::E _f, const Rect &_rect, void (*_funcOnPress)(Item *, bool), TypeItem::E _type) :
    ButtonCommon(_type, title_ru, title_en, _f, _rect, _funcOnPress)
{
}


bool ButtonCommon::IsPressed() const
{
    return is_pressed;
}


bool ButtonPress::Draw()
{
    if (need_draw)
    {
        int16 x = rect.x;
        int16 y = rect.y;
        int16 width = rect.width;
        int16 height = rect.height;

        if (IsShown())
        {
            Nextion::DrawRect({ x, y, width - 1, height - 1 }, Color::White);
            Nextion::DrawRect({ x + 1, y + 1, width - 3, height - 3 }, Color::White);
            Nextion::DrawRect({ x + 2, y + 2, width - 5, height - 5 }, Color::White);

            Nextion::DrawString({ x + 3, y + 3, width - 7, height - 7 }, font,
                Color::White,
                is_pressed ? Color::ButtonPress : Color::Background,
                title[set.lang], 1, 1);
        }
        else
        {
            Nextion::FillRect(rect, Color::Background);
        }

        need_draw = false;

        return true;
    }

    return false;
}


void Item::SetShown(bool show)
{
    is_shown = show;

    need_draw = true;

    if (!is_shown)
    {
        Draw();
    }
}


ButtonToggle *Item::ToButtonToggle()
{
    return (type == TypeItem::ButtonToggle) ? (ButtonToggle *)this : nullptr;
}


Label::Label(bool append, pchar _textRU, pchar _textEN, const Rect &_rect, Font::E _font, void (*_funcOnPress)(Item *, bool),
    const Color &_colorText, const Color &_colorBack, bool _h_aligned, bool _v_aligned) :
    Item(TypeItem::Label, _rect, _funcOnPress, append),
    font(_font), h_aligned(_h_aligned), v_aligned(_v_aligned), colorText(_colorText),
    colorBack(_colorBack.value == Color::Count.value ? Color::Background : _colorBack)
{
    std::strcpy(text[0], _textRU);
    std::strcpy(text[1], _textEN);
}


Label &Label::operator=(const Label &rhs)
{
    Item *item = (Item *)this;
    Item *item_rhs = (Item *)&rhs;

    *item = *item_rhs;

    std::strcpy(text[0], rhs.text[0]);
    std::strcpy(text[1], rhs.text[1]);
    font = rhs.font;
    h_aligned = rhs.h_aligned;
    v_aligned = rhs.v_aligned;
    colorText = rhs.colorText;
    colorBack = rhs.colorBack;

    return *this;
}


pchar Label::Text() const
{
    return text[set.lang];
}


bool Label::Draw()
{
    if (need_draw)
    {
        if (IsShown())
        {
            Nextion::DrawString(rect, font, colorText, colorBack, Text(), h_aligned, v_aligned);
        }
        else
        {
            Nextion::FillRect(rect, colorBack);
        }

        need_draw = false;

        return true;
    }

    return false;
}


void Label::SetText(const char _textRU[MAX_LEN], const char _textEN[MAX_LEN])
{
    if (std::strcmp(_textRU, text[Lang::RU]) != 0)
    {
        std::strcpy(text[Lang::RU], _textRU);

        need_draw = true;
    }

    if (std::strcmp(_textEN, text[Lang::EN]) != 0)
    {
        std::strcpy(text[Lang::EN], _textEN);

        need_draw = true;
    }
}


int Item::GetCoordX(int col)
{
    int dx = 10;

    return dx + (ButtonCommon::WIDTH_MENU + dx) * col;
}


int Item::GetCoordY(int row)
{
    int dy = 10;

    return dy + (ButtonCommon::HEIGHT_MENU + dy) * row;
}
