// 2022/11/02 15:18:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SCPI
{
    void CallbackOnReceive(uint8);

    void Update();

    namespace OnEvent
    {
        // Updater вызывает эту функцию после того, как записал очередную порцию байт
        void WriteBuffer();
    }
}
