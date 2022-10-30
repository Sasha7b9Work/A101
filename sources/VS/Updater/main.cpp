// 2022/04/29 13:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "rs232.h"
#pragma warning(push)
#pragma warning(disable : 4668)
#include <iostream>
#include <conio.h>
#include <string>
#include <cctype>
#include <fstream>
#pragma warning(pop)


using namespace std;


static void WriteErrorMessage();
static int  OpenPort(char *);
static int  ExtractNumberPort(char *);


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

    int port = OpenPort(argv[1]);

    getch();

    return 0;
}


static void WriteErrorMessage()
{
    cout << "Invalid syntx. Usage:" << endl;
    cout << "\t update101.exe com1 a101.bin" << endl;
}


static int OpenPort(char *name_port)
{
    int port = ExtractNumberPort(name_port);

    while (RS232_OpenComport(port, 115200, "8n1", 0) != 0)
    {
        cout << "Can not open com port number " << port << endl;
    }

    return port;
}


static int ExtractNumberPort(char *name_port)
{
    string name(name_port);

    int result = 0;

    for each (auto c in name)
    {
        if (isdigit(c))
        {
            result *= 10;
            result += c & 0x0F;
        }
        else
        {
            if (result != 0)
            {
                break;
            }
        }
    }

    return result;
}
