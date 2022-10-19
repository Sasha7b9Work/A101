// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct BufferRAW
{
    static const int SIZE = 1024;

    BufferRAW() : pointer(0) { } //-V730

    void Push(uint16 word)
    {
        if (pointer < SIZE)
        {
            data[pointer++] = word;
        }
    }

    uint16 data[SIZE];

    bool IsFull()
    {
        return (pointer == SIZE);
    }

    void Clear() { pointer = 0; }

private:
    int pointer;
};
