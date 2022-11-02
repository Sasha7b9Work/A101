// 2022/11/02 08:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


class ComPort
{
public:

    bool Open();

    bool IsOpened() const;

    void Send(uint8);

private:

    int number = -1;    // Если -1 - порт не открыт
};
