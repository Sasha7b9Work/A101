// 2022/11/01 13:59:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Rect
{
    Rect(int _x = 0, int _y = 0, int _width = 1, int _height = 1) :
        x((int16)_x), y((int16)_y), width((int16)_width), height((int16)_height)
    {
    }
    int16 x;
    int16 y;
    int16 width;
    int16 height;

    bool Intersect(int x, int y) const;
};


struct Coord
{
    int16 x;
    int16 y;
    
    Coord operator-(const Coord &rhs) const
    {
        Coord result{ x, y };
        result.x -= rhs.x;
        result.y -= rhs.y;
        return result;
    }

    Coord operator+(const Coord &rhs) const
    {
        Coord result{ x, y };
        result.x += rhs.x;
        result.y += rhs.y;
        return result;
    }
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

    template<class T> T middle_of_3(T a, T b, T c)
    {
        if ((a <= b) && (a <= c))
        {
            return (b <= c) ? b : c;
        }
        else if ((b <= a) && (b <= c))
        {
            return (a <= c) ? a : c;
        }
        return (a <= b) ? a : b;
    }

    uint CalculateCRC32(const void *buffer, int size);
}
