// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Nextion/Colors.h"
#include "wx_includes.h"


class Screen : public wxPanel
{
public:

    static const int WIDTH = 800;
    static const int HEIGHT = 480;

    Screen(wxWindow *parent);

    static Screen *self;

    void Init();

    void FillRectangle(int x, int y, int width, int height, const Color &);

    void DrawLine(int x1, int y1, int x2, int y2, const Color &);

    void DrawString(int x, int y, int font, const Color &, pchar text);

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

private:
    static wxBitmap bitmap;

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);

    void OnMouseUp(wxMouseEvent &);
};
