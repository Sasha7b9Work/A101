// 2022/11/02 08:51:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <vector>
#include <mutex>


using namespace std;


class ComPort
{
public:

    ~ComPort();

    bool Open();

    void Close();

    bool IsOpened() const;

    void Send(uint8);

    int NumPort() const { return number; }

    void ReadByte();

    vector<uint8> PopAll() { return received.PopAll(); }

private:

    int number = -1;                    // ���� -1 - ���� �� ������

    struct Buffer
    {
        void Push(uint8);
        // ���������� ��, ��� ���� � ������ � ������� ������ �����
        vector<uint8> PopAll();
    private:
        mutex mutex_buffer;
        vector<uint8> buffer;        // ����� �������� �����
    };

    Buffer received;

    static void FuncReceive(ComPort *);
};
