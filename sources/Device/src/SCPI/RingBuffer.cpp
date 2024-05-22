// 2023/10/16 10:59:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/RingBuffer.h"


void RingBuffer::Clear()
{
    index_in = 0;
    index_out = 0;
    is_busy = false;
}

void RingBuffer::Append(uint8 byte)
{
    is_busy = true;

    if (GetElementCount() == SIZE)
    {
        LOG_WRITE_TRACE("!!! ERRROR !!! Very small buffer");

        is_busy = false;

        return;
    }

    buffer[index_in] = byte;
    index_in++;

    if (index_in == SIZE)
    {
        index_in = 0;
    }

    is_busy = false;
}


int RingBuffer::GetElementCount() const
{
    int index_new = index_in;

    if (index_new >= index_out)
    {
        return index_new - index_out;
    }

    return SIZE + index_new - index_out;
}


void RingBuffer::GetData(SCPI::InBuffer &out)
{
    if (is_busy)
    {
        return;
    }

    if (GetElementCount() == 0)
    {
        return;
    }

    if (is_busy)
    {
        return;
    }

    int index_new = index_in;

    if (index_new >= index_out)
    {
        out.Append(buffer + index_out, index_new - index_out);
    }
    else
    {
        out.Append(buffer + index_out, SIZE - index_out);
        out.Append(buffer, index_new);
    }

    index_out = index_new;
}
