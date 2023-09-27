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
    static Color Count;

    Color(uint8 v) : value(v) {}

    void SetAsCurrent() const;

    static uint16 MakeColor(float r, float g, float b);
    static uint16 MakeColor(uint16 v) { return v; }
    static Color Current() { return current; }
    static pchar CurrentValue();

    uint8 value;

#ifdef WIN32
    uint ToRaw() const;
#else
    uint16 ToRaw() const;
#endif

private:

    static Color current;
};
