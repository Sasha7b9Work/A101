// 2022/10/28 22:55:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#define WIN32_LEAN_AND_MEAN
#include  "wx/wx.h"


class Frame : public wxFrame
{
public:
    Frame(const wxString &title);

    static Frame *self;

private:

    void SetSizeAndPosition();
};
