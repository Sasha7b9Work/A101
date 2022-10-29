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

private:
    static wxBitmap bitmap;

    void OnPaint(wxPaintEvent &);

    void OnMouseDown(wxMouseEvent &);

    void OnMouseUp(wxMouseEvent &);

};
