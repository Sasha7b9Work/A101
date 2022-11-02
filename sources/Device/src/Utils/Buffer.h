#pragma once
#include <cstring>


template<class T, int MAX_SIZE>
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

    void Append(const T *data, int _size)
    {
        if (Size() + _size > Capacity())
        {
            LOG_ERROR("Нет места в буфере");
        }
        else
        {
            std::memcpy(&buffer[size], data, (uint)_size * sizeof(T));
            size += _size;
        }
    }

    void Append(T value)
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

protected:

    int size;

    T buffer[MAX_SIZE];
};


template<class T>
class Buffer1024 : public Buffer<T, 1024>
{
public:
    Buffer1024() : Buffer<T, 1024>() { }
    Buffer1024(int size) : Buffer<T, 1024>(size) { }
    Buffer1024(int size, T value) : Buffer<T, 1024>(size, value) { }
};


template<class T>
class Buffer2048 : public Buffer<T, 2048>
{
public:
    Buffer2048() : Buffer<T, 2048>() { }
    Buffer2048(int size) : Buffer<T, 2048>(size) { }
    Buffer2048(int size, T value) : Buffer<T, 2048>(size, value) { }
};
