// 2023/10/16 10:59:36 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/RingBuffer.h"


void RingBuffer::Clear()
{
    index_in = 0;
    index_out = 0;
}

void RingBuffer::Append(uint8 byte)
{
    if (GetElementCount() == SIZE)
    {
        LOG_WRITE_TRACE("!!! ERRROR !!! Very small buffer");

        return;
    }

    buffer[index_in] = byte;
    index_in++;

    if (index_in == SIZE)
    {
        index_in = 0;
    }
}


int RingBuffer::GetElementCount() const
{
    if (index_in >= index_out)
    {
        return index_in - index_out;
    }

    return SIZE + index_in - index_out;
}


void RingBuffer::GetData(SCPI::InBuffer &out)
{
    if (GetElementCount() == 0)
    {
        return;
    }

    int index_new = index_in;

    if (index_new > index_out)
    {
        out.Append(buffer + index_out, GetElementCount());
    }
    else
    {
        out.Append(buffer + index_out, SIZE - index_out);
        out.Append(buffer, index_new);
    }

    index_out = index_new;
}
