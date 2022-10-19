// 2022/10/19 10:00:22 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Color
{
    static Color None;
    static Color Black;
    static Color White;

    Color(uint8 v) : value(v) {}

    void SetAsCurrent() const;

    static uint16 MakeColor(float r, float g, float b);
    static Color Current() { return current; }
    static pchar CurrentValue();

    uint8 value;

private:

    static Color current;
};
