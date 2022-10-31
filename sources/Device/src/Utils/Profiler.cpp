// 2022/10/31 13:51:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Utils/Profiler.h"
#include "Hardware/Timer.h"


namespace Profiler
{
    static int num_bytes = 0;
    static int num_commands = 0;
    static TimeMeterMS meter;
}


void Profiler::AddCommand()
{
    num_commands++;
}


void Profiler::AddByte()
{
    num_bytes++;
}


void Profiler::AddBytes(int num)
{
    num_bytes += num;
}


void Profiler::Update()
{
//    if (meter.ElapsedTime() >= 1000)
//    {
//        LOG_WRITE("Send : %d commands, %d bytes", num_commands, num_bytes);
//        meter.Reset();
//    }
}
