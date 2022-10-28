// 2022/10/28 22:55:57 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#define WIN32_LEAN_AND_MEAN
#include "wx/wx.h"


class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;

    static Application *self;

    void Update();

private:

    void Init();
};
