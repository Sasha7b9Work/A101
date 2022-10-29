// 2022/10/28 23:17:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "wx_includes.h"


class Screen : public wxPanel
{
public:

    static const int WIDTH = 800;
    static const int HEIGHT = 480;

    Screen(wxWindow *parent);

    static Screen *self;

    void Init();

    void FillRectangle(int x, int y, int width, int height, const wxColor &);

    void DrawLine(int x1, int y1, int x2, int y2, const wxColor &);

    void DrawString(int x, int y, int font, const wxColor &, pchar text);

    struct Button
    {
        static void SetText(pchar name_button, pchar text);

        static void Highlight(pchar name_button, bool);

        static void Enable(pchar name_button);

        static void Disable(pchar name_button);
    };

private:
    static wxBitmap bitmap;

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);
};
