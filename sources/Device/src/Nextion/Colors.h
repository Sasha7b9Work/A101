// 2022/10/19 10:00:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color
{
    static Color Black;
    static Color White;
    static Color Background;
    static Color ButtonPress;
    static Color MeasureDC;
    static Color MeasureAC;
    static Color Gray50;
    static Color Gray75;
    static Color Count;

    Color(uint8 v = 255) : value(v) {}

    static void Init();

    void SetAsCurrent() const;

    static uint16 MakeColor(float r, float g, float b);
    static uint16 MakeColor(uint16 v) { return v; }
    static Color Current() { return current; }
    pchar GetValue() const;

    uint8 Value() const;

    uint16 ToRaw() const;

private:

    static Color current;
    uint8 value;
};
