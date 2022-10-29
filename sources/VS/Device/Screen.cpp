// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Screen.h"
#include "Display/Painter.h"
#include "Display/DInterface.h"
#include <string>
#include <map>
#include <algorithm>


wxBitmap Screen::bitmap(Screen::WIDTH, Screen::HEIGHT);


Screen *Screen::self = nullptr;

static const int width_button = 130;
static const int height_button = 72;
static const int y_button = 405;


struct Button
{
    Button(int _index, int _x, int _y, int _width, int _height)
        : pressed(false), index(_index), text(""), x(_x), y(_y), width(_width), height(_height) {}

    void Draw();

    // Возвращает true, если пикслеь внутри кнопки
    bool PixelInside(int x, int y);

    // "Нажать" кнопку
    void Press();

    // "Отпустить" кнопку
    void Release();

    void SetText(pchar);

    bool pressed;

private:
    int index;
    std::string text;
    int x;
    int y;
    int width;
    int height;
};


struct Font
{
    static wxFont Get(int num_font);
};


static Button btn0(0, 3,   y_button, width_button, height_button);
static Button btn1(1, 136, y_button, width_button, height_button);
static Button btn2(2, 269, y_button, width_button, height_button);
static Button btn3(3, 402, y_button, width_button, height_button);
static Button btn4(4, 535, y_button, width_button, height_button);
static Button btn5(5, 668, y_button, width_button, height_button);
static Button btMenu(6, 725, 7, 69, 69);


static std::map<std::string, Button *> buttons
{
    {"button0", &btn0},
    {"button1", &btn1},
    {"button2", &btn2},
    {"button3", &btn3},
    {"button4", &btn4},
    {"button5", &btn5},
    {"btMenu", &btMenu}
};


Screen::Screen(wxWindow *parent) : wxPanel(parent)
{
    self = this;

    SetMinSize({ Screen::WIDTH, Screen::HEIGHT });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &Screen::OnMouseDown, this);
    Bind(wxEVT_LEFT_UP, &Screen::OnMouseUp, this);
}


void Screen::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.DrawBitmap(bitmap, 0, 0);
}


void Screen::OnMouseDown(wxMouseEvent &event)
{
    int x = event.GetX();
    int y = event.GetY();

    for (auto &elem : buttons)
    {
        Button *button = elem.second;

        if (button->PixelInside(x, y))
        {
            button->Press();
        }
    }
}


void Screen::OnMouseUp(wxMouseEvent &)
{
    for (auto &elem : buttons)
    {
        elem.second->Release();
    }
}


void Screen::Init()
{
    Rect(WIDTH, HEIGHT).Fill(0, 0, Color::Background);

    for (auto &elem : buttons)
    {
        elem.second->Draw();
    }
}


void Screen::FillRectangle(int x, int y, int width, int height, const wxColor &color)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(color);
    dc.SetPen(color);
    dc.DrawRectangle({ x, y, width, height });
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void Screen::DrawLine(int x1, int y1, int x2, int y2, const wxColor &color)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetPen(color);
    dc.DrawLine(x1, y1, x2, y2);
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void Screen::DrawString(int x, int y, int num_font, const wxColor &color, pchar text)
{
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetTextForeground(color);
    dc.SetFont(Font::Get(num_font));
    dc.DrawText(text, x, y);
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void Screen::SetTextButton(pchar name_button, pchar text)
{
    buttons[name_button]->SetText(text);
}


void Screen::SetValButton(pchar name_button, int val)
{
    buttons[name_button]->pressed = val > 0;
    buttons[name_button]->Draw();
}


void Button::Draw()
{
    Rect(width, height).Draw(x, y, Color::White);

    Color color_fill = pressed ? Color::ButtonPress : Color::Background;

    Rect(width - 2, height - 2).Fill(x + 1, y + 1, color_fill);

    int d = 20;
    Painter::DrawString(x + d, y + d, width - 2 * d, height - 2 * d, 3, Color::White, color_fill, text.c_str());
}


void Button::SetText(pchar _text)
{
    text = _text;
    Draw();
}


bool Button::PixelInside(int pixel_x, int pixel_y)
{
    if (pixel_x < x)          { return false; }
    if (pixel_x > x + width)  { return false; }
    if (pixel_y < y)          { return false; }
    if (pixel_y > y + height) { return false; }

    return true;
}


void Button::Press()
{
    pressed = true;

    DInterface::CallbackOnReceive((uint8)(0x30 + index));
    DInterface::CallbackOnReceive(0x31);
    DInterface::CallbackOnReceive('Z');

    Draw();
}


void Button::Release()
{
    Draw();

    if (pressed)
    {
        DInterface::CallbackOnReceive((uint8)(0x30 + index));
        DInterface::CallbackOnReceive(0x30);
        DInterface::CallbackOnReceive('Z');
    }

    pressed = false;
}


wxFont Font::Get(int num_font)
{
    static const int sizes[10] = { 30, 30, 30, 20, 30, 30, 30, 60, 30, 30 };

    return wxFont(sizes[num_font], wxFONTFAMILY_ROMAN, wxNORMAL, wxNORMAL);
}
