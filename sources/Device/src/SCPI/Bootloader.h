// 2024/10/27 15:32:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Bootloader
{
    void Run(uint version_build_new, uint size, uint crc32);

    void Update();

    bool InProgress();

    void OnReceiveByte(char);

    void DisplayFunc();
}
