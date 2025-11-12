// 2025/11/12 14:36:28 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace ComPort
{
    // Возвращает массив портов. 0 == COM1, true - cуществует
    std::vector<bool> &GetComports();

    // Попытка приконнектиться в соотвествии с настройками
    bool TryConnect(int num_port);

    bool IsConnected();

    void Update();

    void Close();

    void SendCommand(const wxString &);

    void Send(void *buffer, int num_bytes);

    pchar NameOpenedPort();
};
