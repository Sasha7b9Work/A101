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

    void OnPaint(wxPaintEvent &);

    void Init();

    void DrawRectangle(int x, int y, int width, int height, const wxColor &);

private:
    static wxBitmap bitmap;
};
