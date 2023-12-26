#pragma once
#include <cstring>


template<int MAX_SIZE, class T = uint8 >
class Buffer
{
public:

    Buffer() : size(0) {} //-V730

    void Fill(T value)
    {
        for (int i = 0; i < size; i++)
        {
            buffer[i] = value;
        }
    }

    T *Data() { return buffer; }

    const T *DataConst() const { return buffer; }

    T *Last()
    {
        return buffer + Size();
    }

    // Возвращает количество элементов в буфере
    int Size() const
    {
        return size;
    }

    int Capacity() const
    {
        return MAX_SIZE;
    }

    void RemoveFirst(int num_elements)
    {
        if (num_elements > size)
        {
            LOG_ERROR("Too many elements");
            num_elements = size;
        }

        if (num_elements == size)
        {
            size = 0;
        }
        else
        {
            std::memmove(buffer, buffer + num_elements, (uint)num_elements);
            size -= num_elements;
        }
    }

    void Append(const uint8 *data, int _size)
    {
        if (Size() + _size > Capacity())
        {
            LOG_ERROR("Нет места в буфере");
        }
        else
        {
            std::memcpy(&buffer[size], data, (uint)_size * sizeof(buffer[0]));
            size += _size;
        }
    }

    void Append(uint8 value)
    {
        if (Size() + 1 > Capacity())
        {
            LOG_ERROR("Buffer is full");
        }
        else
        {
            buffer[size++] = value;
        }
    }

    T &operator[](uint i)
    {
        if ((int)i >= 0 && (int)i < Size())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

    T &operator[](int i)
    {
        if (i >= 0 && i < Size())
        {
            return buffer[i];
        }

        static T null(0);

        return null;
    }

    // Содержимое эквалентно данной строке
    bool IsEquals(pchar)
    {
        return false;
    }

    // Содержимое начинается с данной строки
    bool BeginWith(char *)
    {
        return false;
    }

protected:

    int size;

    T buffer[MAX_SIZE];
};


class Buffer1024 : public Buffer<1024>
{
public:
    Buffer1024() : Buffer<1024>() { }
};
