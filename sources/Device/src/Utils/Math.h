// 2022/11/01 13:59:48 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Math
{
    template<class T> T Limitation(T *value, T min, T max)
    {
        if (*value < min)
        {
            *value = min;
        }
        else if (*value > max)
        {
            *value = max;
        }
        return *value;
    }

    float Random(float min, float max);
}
