// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"


class WindowMeasure
{
public:
    WindowMeasure(int x, int y, int font);
    void Enable();
    void Disable();
    void SetText(char [TextString::MAX_LEN]);
private:
};


class WindowMeasureAC : public WindowMeasure
{
public:
    WindowMeasureAC(int x, int y, int font) : WindowMeasure(x, y, font) {}
private:
};


class WindowMeasureDC : public WindowMeasure
{
public:
    WindowMeasureDC(int x, int y, int font) : WindowMeasure(x, y, font) {}
private:
};
