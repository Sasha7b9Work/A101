// 2022/11/06 19:23:41 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <cstring>


template<class T, int size_buffer>
class Averager //-V730
{
public:
    void Push(T value)
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
    void Reset() { num_elements = 0; }
private:
    T buffer[size_buffer];
    int num_elements = 0;
    T sum = T(0);
};
