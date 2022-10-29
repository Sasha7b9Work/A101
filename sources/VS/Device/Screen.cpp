// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Screen.h"
#include "Display/Painter.h"
#include <string>


wxBitmap Screen::bitmap(Screen::WIDTH, Screen::HEIGHT);


Screen *Screen::self = nullptr;

static const int width_button = 130;
static const int height_button = 72;
static const int y_button = 405;


struct Button
{
    Button(pchar _name, int _x, int _y, int _width, int _height)
        : name(_name), text(""), x(_x), y(_y), width(_width), height(_height) {}

    std::string name;
    std::string text;
    int x;
    int y;
    int width;
    int height;
};

static Button btn0("button0", 3,   y_button, width_button, height_button);
static Button btn1("button1", 136, y_button, width_button, height_button);
static Button btn2("button2", 269, y_button, width_button, height_button);
static Button btn3("button3", 402, y_button, width_button, height_button);
static Button btn4("button4", 535, y_button, width_button, height_button);
static Button btn5("button5", 668, y_button, width_button, height_button);

Screen::Screen(wxWindow *parent) : wxPanel(parent)
{
    self = this;

    SetMinSize({ Screen::WIDTH, Screen::HEIGHT });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Screen::OnPaint, this);
}


void Screen::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.DrawBitmap(bitmap, 0, 0);
}


void Screen::Init()
{
    Rect(WIDTH, HEIGHT).Fill(0, 0, Color::Background);
}


void Screen::FillRectangle(int x, int y, int width, int height, const wxColor &color)
{
    wxMemoryDC memDC;
    memDC.SelectObject(bitmap);
    wxBrush brush(color);
    memDC.SetBrush(brush);
    memDC.DrawRectangle({ x, y, width, height });
    memDC.SelectObject(wxNullBitmap);
    Refresh();
}
