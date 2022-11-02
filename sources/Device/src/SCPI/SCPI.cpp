// 2022/11/02 15:18:05 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "SCPI/SCPI.h"
#include "SCPI/Parser/Parser.h"
#include "Utils/Buffer.h"


namespace SCPI
{
    static Buffer2048<uint8> data_in;
}


void SCPI::Update()
{
    if (data_in.Size())
    {
        int i = data_in.Size();
        i = i;
    }
}


void SCPI::CallbackOnReceive(uint8 byte)
{
    data_in.Append(byte);
}
