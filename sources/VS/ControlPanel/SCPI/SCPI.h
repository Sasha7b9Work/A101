// 2022/11/02 08:12:55 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace SCPI
{
    void Init();

    void DeInit();

    void Update();

    // «авершающий ноль замен€етс€ последовательностью \r\n
    void Send(pchar, ...);

    namespace Event
    {
        void ChangePort();
    }
}
