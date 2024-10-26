// 2022/11/06 19:23:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <Utils/Math.h>
#include <cstring>


template<class T, int size_buffer>
class Averager //-V730
{
public:
    T Push(T value)
    {
        if (num_elements == size_buffer)
        {
            sum -= buffer[0];

            if (size_buffer != 1)
            {
                std::memmove(buffer, &buffer[1], sizeof(T) * (size_buffer - 1));
            }

            num_elements--;
        }

        buffer[num_elements++] = value;
        sum += value;

        return Get();
    }

    T Pop(int index)
    {
        return buffer[index];
    }

    T Get()
    {
        return sum / (T)num_elements;
    }
    int NumElements() const { return num_elements; }
    void Reset() { num_elements = 0; sum = 0; }
private:
    T buffer[size_buffer];
    int num_elements = 0;
    T sum = T(0);
};


template<int size_buffer>
class AveragerReal : public Averager <REAL, size_buffer>
{
public:
    REAL Push(REAL value)
    {
        if (!Math::RealIsCorrect(value))
        {
            Averager<REAL, size_buffer>::Reset();

            return 0.0;
        }

        return Averager<REAL, size_buffer>::Push(value);
    }
private:
};
