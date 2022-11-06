// 2022/11/01 13:59:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Math
{
    float Random(float min, float max);

    template<class T> T Abs(T value)
    {
        return (value < T(0)) ? -value : value;
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
}
