// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "../../Device/src/Display/Colors.h"


class Screen : public wxPanel
{
public:

    static const int WIDTH = 800;
    static const int HEIGHT = 480;

    Screen(wxWindow *parent);

    static Screen *self;

    void Init();

private:

    static wxBitmap bitmap;

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);

public:

    void FillRectangle(int x, int y, int width, int height, const Color & = Color::Count);

    void DrawRectangle(int x, int y, int width, int height, const Color & = Color::Count);

    void DrawLine(int x1, int y1, int x2, int y2, const Color & = Color::Count);

    void DrawLineV(int x, int y1, int y2, const Color & = Color::Count);

    void DrawLineH(int y, int x1, int x2, const Color & = Color::Count);

    void DrawString(int x, int y, int font, const Color &, pchar);

    void DrawString(int x, int y, int width, int height, int font, const Color &color_draw, const Color &color_back, pchar);

    struct Button
    {
        static void SetText(pchar name_button, pchar text);

        static void Highlight(pchar name_button, bool);

        static void Enable(pchar name_button);

        static void Disable(pchar name_button);
    };

    struct WaveInput
    {
        static void Draw(uint8 *, int);

        static void Enable(int size);

        static void Disable(int size);
    };

    struct WaveFFT
    {
        static void Enable(int size);

        static void Disable(int size);
    };
};
