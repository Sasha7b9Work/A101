// 2022/11/10 22:26:12 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "TextString.h"


class WindowMeasure
{
public:
    WindowMeasure(int x, int y, bool is_signed, int font, pchar _title, const Color &back = Color::Count);
    void Init();
    void Enable();
    void Disable();
    void SetMeasure(const char [TextString::MAX_LEN]);
    void OnEventChangeRange();
private:
#ifdef WIN32
    static const int HEIGHT = 92;
    static const int WIDTH = 620;
    static const int WIDTH_DIGIT = 40;
#else
    static const int HEIGHT = 92;
    static const int WIDTH = 720;
    static const int WIDTH_DIGIT = 55;
#endif

    TextString title;

    TextString point;
    TextString digit1;
    TextString digit2;
    TextString digit3;
    TextString digit4;
    TextString digit5;

    TextString sign;
    TextString units;

    TextString *digits[5];

    Color colorBack;

    bool is_signed;
};


class WindowMeasureAC : public WindowMeasure
{
public:
    WindowMeasureAC(int x, int y, int font) : WindowMeasure(x, y, false, font, "AC:") {}
private:
};


class WindowMeasureDC : public WindowMeasure
{
public:
    WindowMeasureDC(int x, int y, int font) : WindowMeasure(x, y, true, font, "DC:") {}
private:
};
