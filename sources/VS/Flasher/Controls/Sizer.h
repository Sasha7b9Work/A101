// 2024/08/21 21:20:25 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class Sizer : public wxBoxSizer
{
public:
    Sizer(int orient) : wxBoxSizer(orient) { }
    void Enable(bool);
private:
};
