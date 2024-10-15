// 2024/10/09 09:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Ampermeter/BufferADC.h"
#include "Nextion/DiagramInput.h"
#include <limits>


ResolverFrequency::ResolverFrequency(const Period &period)
{
    float sum[BufferADC::SIZE];

    {                                                                       // Заполняем массив, по которому будем считать интегралы
        for (int i = 1; i < BufferADC::SIZE; i++)
        {
            sum[i] = (float)BufferADC::At(i).Real();
        }
    }

    DiagramInput::InstallData(sum);

    frequency = period.GetFrequency();
}
