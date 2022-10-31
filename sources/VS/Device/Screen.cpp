// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Screen.h"
#include "Display/Nextion.h"
#include <string>
#include <map>
#include <algorithm>


wxBitmap Screen::bitmap(Screen::WIDTH, Screen::HEIGHT);


Screen *Screen::self = nullptr;

static const int width_button = 130;
static const int height_button = 72;
static const int y_button = 405;
static const int x_wave = 9;
static const int y_wave = 10;
static const int width_wave = 390;
static const int height_wave = 196;


struct ButtonGUI
{
    ButtonGUI(int _index, int _x, int _y, int _width, int _height)
        : index(_index), text(), x(_x), y(_y), width(_width), height(_height), enabled(true), highlight(false) {}

    void Draw();

    // Возвращает true, если пикслеь внутри кнопки
    bool PixelInside(int x, int y);

    // "Нажать" кнопку
    void Press();

    void SetText(pchar);

    void Enable();

    void Disable();

    void SetHightlight(bool);

private:
    int index;
    std::string text;
    int x;
    int y;
    int width;
    int height;
    bool enabled;
    bool highlight;
};


struct Wave
{
    Wave(int _x, int _y, int _w, int _h) : x(_x), y(_y), width(_w), height(_h), enabled(false) {};

    void Draw();

    void DrawData(uint8 *, int);

    void Enable();

    void Disable();

    bool IsEnabled() const { return enabled; }

private:
    int x;
    int y;
    int width;
    int height;
    bool enabled;
};


struct Font
{
    static wxFont Get(int num_font);
};


static ButtonGUI btn0(0, 3,   y_button, width_button, height_button);
static ButtonGUI btn1(1, 136, y_button, width_button, height_button);
static ButtonGUI btn2(2, 269, y_button, width_button, height_button);
static ButtonGUI btn3(3, 402, y_button, width_button, height_button);
static ButtonGUI btn4(4, 535, y_button, width_button, height_button);
static ButtonGUI btn5(5, 668, y_button, width_button, height_button);
static ButtonGUI btMenu(6, 725, 7, 67, 67);


static std::map<std::string, ButtonGUI *> buttons
{
    {"button0", &btn0},
    {"button1", &btn1},
    {"button2", &btn2},
    {"button3", &btn3},
    {"button4", &btn4},
    {"button5", &btn5},
    {"btMenu", &btMenu}
};


static Wave waveLeft(x_wave, y_wave, width_wave, height_wave);
static Wave waveRight(401, y_wave, width_wave, height_wave);
static Wave waveBig(x_wave, y_wave, 783, height_wave);


Screen::Screen(wxWindow *parent) : wxPanel(parent)
{
    self = this;

    SetMinSize({ Screen::WIDTH, Screen::HEIGHT });
    SetDoubleBuffered(true);
    Bind(wxEVT_PAINT, &Screen::OnPaint, this);
    Bind(wxEVT_LEFT_DOWN, &Screen::OnMouseDown, this);
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
        ButtonGUI *button = elem.second;

        if (button->PixelInside(x, y))
        {
            button->Press();
        }
    }
}


void Screen::Init()
{
    Nextion::FillRect(0, 0, WIDTH, HEIGHT, Color::Background);

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


void Screen::Button::SetText(pchar name_button, pchar text)
{
    buttons[name_button]->SetText(text);
}


void Screen::Button::Highlight(pchar name_button, bool hightlight)
{
    buttons[name_button]->SetHightlight(hightlight);
}


void ButtonGUI::Draw()
{
    if (!enabled)
    {
        Nextion::FillRect(x, y, width, height, Color::Background);
    }
    else
    {
        Nextion::DrawRect(x, y, width, height, Color::White);

        Color color_fill = highlight ? Color::ButtonPress : Color::Background;

        Nextion::FillRect(x + 1, y + 1, width - 2, height - 2, color_fill);

        int d = 20;
        Nextion::DrawString(x + d, y + d, width - 2 * d, height - 2 * d, 3, Color::White, color_fill, text.c_str());
    }
}


void ButtonGUI::SetText(pchar _text)
{
    text = _text;
    Draw();
}


bool ButtonGUI::PixelInside(int pixel_x, int pixel_y)
{
    if (pixel_x < x)          { return false; }
    if (pixel_x > x + width)  { return false; }
    if (pixel_y < y)          { return false; }
    if (pixel_y > y + height) { return false; }

    return true;
}


void ButtonGUI::Press()
{
    Nextion::CallbackOnReceive((uint8)(0x30 + index));
    Nextion::CallbackOnReceive('Z');
}


void ButtonGUI::Enable()
{
    enabled = true;

    Draw();
}


void ButtonGUI::Disable()
{
    enabled = false;

    Draw();
}


void ButtonGUI::SetHightlight(bool _highlight)
{
    highlight = _highlight;

    Draw();
}


void Screen::Button::Enable(pchar name_button)
{
    buttons[name_button]->Enable();
}


void Screen::Button::Disable(pchar name_button)
{
    buttons[name_button]->Disable();
}


void Screen::WaveInput::Enable(int size)
{
    if (size == 0)
    {
        waveLeft.Enable();
    }
    else
    {
        waveBig.Enable();
    }
}


void Screen::WaveInput::Disable(int size)
{
    if (size == 0)
    {
        waveLeft.Disable();
    }
    else
    {
        waveBig.Disable();
    }
}


void Screen::WaveFFT::Enable(int size)
{
    if (size == 0)
    {
        waveRight.Enable();
    }
    else
    {
        waveBig.Enable();
    }
}


void Screen::WaveFFT::Disable(int size)
{
    if (size == 0)
    {
        waveRight.Disable();
    }
    else
    {
        waveBig.Disable();
    }
}


void Wave::Enable()
{
    enabled = true;
    Draw();
}


void Wave::Disable()
{
    enabled = false;
    Draw();
}


void Screen::WaveInput::Draw(uint8 *points, int num_points)
{
    if (waveLeft.IsEnabled())
    {
        waveLeft.DrawData(points, num_points);
    }
    if(waveBig.IsEnabled())
    {
        waveBig.DrawData(points, num_points);
    }
}


void Wave::DrawData(uint8 *data, int num_points)
{
    Draw();

    if (num_points > width)
    {
        num_points = width;
    }

    float scale = (float)height / 256.0f;

    for (int i = 0; i < num_points; i++)
    {
        Nextion::DrawLineV(x + i, (int)(y + scale * data[i]), (int)(y + scale * data[i] + 1), Color::White);
    }
}


void Wave::Draw()
{
    Color color = enabled ? Color::ButtonPress : Color::Background;

    Screen::self->FillRectangle(x, y, width, height, wxColor(color.ToRaw()));
}


wxFont Font::Get(int num_font)
{
    static const int sizes[10] = { 30, 30, 30, 20, 30, 30, 30, 60, 30, 30 };

    return wxFont(sizes[num_font], wxFONTFAMILY_ROMAN, wxNORMAL, wxNORMAL);
}
