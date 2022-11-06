// 2022/11/06 17:18:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/PeriodInt.h"
#include <limits>


PeriodInt::PeriodInt(const BufferADC &buffer)
{
    sum[0] = buffer[0].Raw();

    for (int i = 1; i < BufferADC::SIZE; i++)
    {
        sum[i] = sum[i - 1] + buffer[i].Raw();
    }

    for (int per = BufferADC::SIZE / 2; per < BufferADC::SIZE; per++)
    {

    }
}


int PeriodInt::FindDelta(int per)
{
    int max = std::numeric_limits<int>::min();
    int min = std::numeric_limits<int>::max();
}
