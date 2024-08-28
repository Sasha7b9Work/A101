// 2022/10/24 11:25:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Nextion/Nextion.h"
#include "Menu/Pages/Pages.h"
#include "Hardware/Timer.h"
#include "Utils/String.h"
#include <cstring>
#include <cstdio>


using namespace std;


Page *Page::current = PageMain::self;


namespace NSBC
{
    static const int MAX_BUTTONS = 128;                  // Здесь хранятся все созданные кнопки (со всех страниц)
    static ButtonCommon *buttons[MAX_BUTTONS];
    static int num_buttons = 0;                         // Столько создано кнопок в данный момент

    static void AppendNewButton(ButtonCommon *button)
    {
        buttons[num_buttons] = button;

        button->SetActive(false);

        if (num_buttons++ >= MAX_BUTTONS)
        {
            LOG_ERROR_TRACE("The button buffer is full");
        }
    }

    static ButtonCommon *GetButton(int x, int y)
    {
        for (int i = 0; i < num_buttons; i++)
        {
            ButtonCommon *button = buttons[i];

            if (button->GetRect().Intersect(x, y) && button->IsActive())
            {
                return button;
            }
        }

        return nullptr;
    }
}


void ButtonCommon::SetAllInactive()
{
    for (int i = 0; i < NSBC::num_buttons; i++)
    {
        NSBC::buttons[i]->SetActive(false);
    }
}


void ButtonCommon::Press()
{
    is_pressed = true;

    Draw();

    funcOnPress();
}


void ButtonCommon::Release()
{
    if (IsWithoutFixation())
    {
        is_pressed = false;

        return;
    }
}

ButtonCommon::ButtonCommon(pchar title_ru, pchar title_en, Font::E _f, int _x, int _y, int _w, int _h, void (*_funcOnPress)()) :
    font(_f), rect{(int16)_x, (int16)_y, (int16)_w, (int16)_h}, funcOnPress(_funcOnPress)
{
    title[Lang::RU] = title_ru;
    title[Lang::EN] = title_en;

    NSBC::AppendNewButton(this);
}


void ButtonCommon::OnEventPress(int x, int y)
{
    ButtonCommon *button = NSBC::GetButton(x, y);

    if (button)
    {
        button->Press();
    }
}


void ButtonCommon::OnEventRelease(int x, int y)
{
    ButtonCommon *button = NSBC::GetButton(x, y);

    if (button)
    {
        button->Release();
    }
}


void Menu::Init()
{
    PageMain::self->SetAsCurrent();

    PageMain::Init();
}


void ButtonOld::Press()
{
    funcOnPress();
}


void ButtonOld::SetText(pchar txt) const
{
    if (IsSoftware())
    {

    }
    else
    {
        Nextion::Button::SetText(name, txt);
    }
}


void ButtonOld::SetVisible(bool visible)
{
    if (IsSoftware())
    {

    }
    else
    {
        Nextion::SetVisible(name, visible);

        is_visible = visible;
    }
}


void ButtonOld::SetValue(int _value)
{
    value = _value;

    if (IsSoftware())
    {

    }
    else
    {
        Nextion::SendCommandFormat("%s.val=%d", name, value);
    }
}


ButtonCommon *Page::GetButton(int index)
{
    return buttons[index];
}


void Page::SetButton(int index, ButtonOld *button)
{
    buttons[index] = button;
}


void Page::SetAsCurrent()
{
    ButtonCommon::SetAllInactive();

    current = this;

    Timer::Delay(50);           // Эта задержка нужна для того, чтобы дисплей успел переключиться на новую страницу.
                                // Иначе сообщения элементам управления будут посылаться на старую страницу

    for (int i = 0; i < GetButtonsCount(); i++)
    {
        GetButton(i)->SetActive(true);
    }

    current->funcOnEnter();
}

ButtonCommon *Page::GetButton(pchar signal)
{
    ButtonCommon **button = &buttons[0];

    while (*button)
    {
        if (std::strcmp((*button)->Signal(), signal) == 0)
        {
            return *button;
        }

        button++;
    }

    return nullptr;
}


void Page::Draw()
{
    funcOnDraw();

    for (int i = 0; i < GetButtonsCount(); i++)
    {
        GetButton(i)->Draw();
    }
}


void ButtonOld::Draw()
{
    if (!IsSoftware())
    {
        return;
    }

    int width = 150;
    int height = 50;

    Nextion::DrawLine(x, y, x + width, y, Color::White);
    Nextion::DrawLine(x, y + 1, x + width, y + 1, Color::White);

    Nextion::DrawLine(x, y + height, x + width, y + height);
    Nextion::DrawLine(x, y + height + 1, x + width, y + height + 1);

    Nextion::DrawLine(x, y, x, y + height);
    Nextion::DrawLine(x + 1, y, x + 1, y + height);

    Nextion::DrawLine(x + width, y, x + width, y + height);
    Nextion::DrawLine(x + width + 1, y, x + width + 1, y + height);

    Nextion::DrawString(x + 2, y + height / 2 - 16, width - 4, height / 2, 2, Color::White, Color::Background, Text(), 1);
}


int Page::GetButtonsCount()
{
    int count = 0;

    ButtonCommon **button = &buttons[0];

    while (*button++)
    {
        count++;
    }

    return count;
}


bool ButtonOld::IsSoftware() const
{
    return x >= 0;
}


Button::Button(pchar title_ru, pchar title_en, Font::E _f, int _x, int _y, int _w, int _h, void (*_funcOnPress)()) :
    ButtonCommon(title_ru, title_en, _f, _x, _y, _w, _h, _funcOnPress)
{
}


void ButtonCommon::SetValue(int value)
{
    bool new_pressed = (value != 0);

    if (new_pressed != is_pressed)
    {
        is_pressed = new_pressed;

        Draw();
    }
}


pchar Button::Signal() const
{
    return "";
}


void Button::Draw()
{
    int x = rect.x;
    int y = rect.y;
    int width = rect.width;
    int height = rect.height;

    Nextion::DrawRect(x, y, width - 1, height - 1, Color::White);
    Nextion::DrawRect(x + 1, y + 1, width - 3, height - 3, Color::White);
    Nextion::DrawRect(x + 2, y + 2, width - 5, height - 5, Color::White);

    Nextion::DrawString(x + 3, y + 3, width - 7, height - 7, font,
        Color::White,
        is_pressed ? Color::ButtonPress : Color::Background,
        title[set.lang], 1, 1);

    SetActive(true);
}
