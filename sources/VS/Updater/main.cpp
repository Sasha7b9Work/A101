// 2022/04/29 13:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "rs232.h"
#include "ComPort.h"
#pragma warning(push)
#pragma warning(disable : 4668)
#include <iostream>
#include <conio.h>
#include <string>
#include <cctype>
#include <fstream>
#pragma warning(pop)

/*
    ***�������� ���������� ***

    ������: update101.exe com5 A101.bin

    ����� ������� ��������� �������� ���������� �������� com - ���� �� �������� 115200 �������� "a101?" � ����������� 0x0D.
    ����������� ���������� ����� ������ ������� ��������� ������ ������������ ���� � ��� ��������� ������� ����������
    ����� "a101Y" � ��������� � ����� ����� ��������.
    �������� ��������� ��� :
    1. update101.exe �������� ������ �������� � ���� "163" � ����������� 0x0D � ��������� � ����� �������������
    2. ���������� �������� ������ "?".������� ������, updater101.exe �������� 1024 ����� ��������.
    3. ������� 1024 �����, ���������� ���������� �� �� ���� - ������
    4. ���� �������� �� ��� �����, ���������� ������� � ������ 2.
    5. ������� �� �������� ��������� � ���������� ������ update101.exe
*/


using namespace std;


static void WriteErrorMessage();
static void Update(const ifstream &);
static void CallbackOnReceive(pchar);


static ComPort port;


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        WriteErrorMessage();

        getch();

        return -1;
    }

    ifstream file(argv[2], ifstream::in | ifstream::binary);

    if (!file.is_open())
    {
        cout << "Can not open file \"" << argv[2] << "\"" << endl;

        getch();

        return -1;
    }

    port.Open(argv[1]);

    Update(file);

    getch();

    return 0;
}


static void WriteErrorMessage()
{
    cout << "Invalid syntx. Usage:" << endl;
    cout << "\t update101.exe com1 a101.bin" << endl;
}


static void Update(const ifstream &file)
{
    while (true)
    {

    }
}
