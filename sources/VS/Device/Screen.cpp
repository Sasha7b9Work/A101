// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Screen.h"
#include "Display/Painter.h"
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
    Button(int _x, int _y, int _width, int _height)
        : text(""), x(_x), y(_y), width(_width), height(_height) {}

    void Draw();

    std::string text;
    int x;
    int y;
    int width;
    int height;
};

static Button btn0(3,   y_button, width_button, height_button);
static Button btn1(136, y_button, width_button, height_button);
static Button btn2(269, y_button, width_button, height_button);
static Button btn3(402, y_button, width_button, height_button);
static Button btn4(535, y_button, width_button, height_button);
static Button btn5(668, y_button, width_button, height_button);
static Button btMenu(725, 7, 69, 69);


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


void Screen::OnMouseDown(wxMouseEvent &)
{

}


void Screen::OnMouseUp(wxMouseEvent &)
{

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


void Button::Draw()
{
    Rect(width, height).Draw(x, y, Color::White);
    Rect(width - 2, height - 2).Draw(x + 1, y + 1, Color::Background);
}
