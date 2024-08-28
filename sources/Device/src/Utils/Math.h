// 2022/11/01 13:59:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Rect
{
    int16 x;
    int16 y;
    int16 width;
    int16 height;

    bool Intersect(int x, int y) const;
};


namespace Math
{
    float Random(float min, float max);

    template<class T> T Abs(T value)
    {
        return (value < T(0)) ? -value : value;
    }

    template<class T> T Sign(T value)
    {
        return (value < T(0)) ? (T)-1 : (T)1;
    }

    template<class T> T Limitation(T *value, T min, T max)
    {
        if (*value < min)       { *value = min; }
        else if (*value > max)  { *value = max; }
        return *value;
    }

    template<class T> void FindMinMax(T value, T &min, T &max)
    {
        if (value < min)        { min = value; }
        if (value > max)        { max = value; }
    }

    uint CalculateCRC32(const void *buffer, int size);
}
