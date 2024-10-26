// 2022/10/28 23:17:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "GUI/Screen.h"
#include "Nextion/Nextion.h"
#include "Nextion/Keyboard.h"
#include <string>
#include <map>
#include <algorithm>


static wxColor ColorToWx(const Color &color)
{
    uint16 raw = color.ToRaw();

    float blue = (float)(raw & 31) / 31.0f;
    float green = (float)((raw >> 5) & 63) / 63.0f;
    float red = (float)((raw >> 11) & 31) / 31.0f;

    return wxColor((uint8)(red * 255), (uint8)(green * 255), (uint8)(blue * 255));
}


wxBitmap Screen::bitmap(Screen::WIDTH, Screen::HEIGHT);


Screen *Screen::self = nullptr;

static const int width_button = 130;
static const int height_button = 72;
static const int y_button = 405;
static const int x_wave = 9;
static const int y_wave = 10;
static const int width_wave = 390;
static const int height_wave = 196;


struct Wave
{
    Wave(int _x, int _y, int _w, int _h) : x(_x), y(_y), width(_w), height(_h), enabled(false) {};

    void Draw();

    void DrawData(const uint8 *, int);

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


struct FontGUI
{
    static wxFont Get(int num_font);
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
    Bind(wxEVT_LEFT_UP, &Screen::OnMouseUp, this);
}


void Screen::OnPaint(wxPaintEvent &)
{
    wxPaintDC dc(this);

    dc.DrawBitmap(bitmap, 0, 0);
}


void Screen::OnMouseDown(wxMouseEvent & event) //-V2009
{
    Keyboard::CallbackOnPress(event.GetX(), event.GetY());
}


void Screen::OnMouseUp(wxMouseEvent &event)
{
    Keyboard::CallbackOnRelease(event.GetX(), event.GetY());
}


void Screen::Init()
{
    Nextion::FillRect({ 0, 0, WIDTH, HEIGHT }, Color::Background);
}


void Screen::FillRectangle(int x, int y, int width, int height, const Color &color)
{
    wxColor colour = ColorToWx(color);

    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBrush(colour);
    dc.SetPen(colour);
    dc.DrawRectangle({ x, y, width, height });
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void Screen::DrawLine(int x1, int y1, int x2, int y2, const Color &color)
{
    wxColor colour = ColorToWx(color);

    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetPen(colour);
    dc.DrawLine(x1, y1, x2, y2);
    dc.SelectObject(wxNullBitmap);
    Refresh();
}


void Screen::DrawString(int x, int y, int num_font, const Color &color, pchar text)
{
    wxColor colour = ColorToWx(color);

    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetTextForeground(colour);
    dc.SetFont(FontGUI::Get(num_font));
    dc.DrawText(text, x, y);
    dc.SelectObject(wxNullBitmap);
    Refresh();
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


void Wave::DrawData(const uint8 *data, int num_points)
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
//    Color color = enabled ? Color::ButtonPress : Color::Background;

//    Screen::self->FillRectangle(x, y, width, height, wxColor(color.ToRaw()));
}


wxFont FontGUI::Get(int num_font)
{
    static const int sizes[10] = { 30, 30, 30, 20, 30, 30, 30, 60, 30, 30 };

    return wxFont(sizes[num_font], wxFONTFAMILY_ROMAN, wxNORMAL, wxNORMAL);
}
