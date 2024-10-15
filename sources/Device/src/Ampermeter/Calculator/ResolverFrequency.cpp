// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/DiagramInput.h"
#include "Ampermeter/Calculator/Averager.h"
#include <limits>


ResolverFrequency::ResolverFrequency(const Period &period)
{
    float sum[BufferADC::SIZE];

    sum[0] = (float)BufferADC::At(0).Real();

    for (int i = 1; i < BufferADC::SIZE; i++)
    {
        sum[i] = sum[i - 1] + (float)BufferADC::At(i).Real();
    }

    DiagramInput::InstallData(sum);

    frequency = period.GetFrequency();
}
