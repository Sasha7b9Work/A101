// 2023/10/16 10:59:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "SCPI/SCPI.h"


class RingBuffer
{
public:
    RingBuffer() { Clear(); }
    static const int SIZE = 1024;
    void Clear();
    void Append(uint8 byte);
    void GetData(SCPI::InBuffer &);
    int GetElementCount() const;
private:
    uint8 buffer[SIZE];
    int index_in;
    int index_out;
};
