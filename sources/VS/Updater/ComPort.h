// 2022/10/30 16:57:54 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ComPort
{
public:
    void Open(char *name_port, void (*func_callback)(char));
    void Send(pchar);
    void SendBuffer(uint8 *, int size);

private:
    int port = -1;
    int ExtractNumberPort(char *name_port);
};
