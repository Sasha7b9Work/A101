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
    ***јлгоритм обновлени€ ***

    «апуск: update101.exe com5 A101.bin

    ѕосле запуска программа начинате опрашивать заданный com - порт на скорости 115200 запросом "a101?" с завершающим 0x0D.
    ѕоключЄнное устройство после подачи питани€ несколько секунд прослушивает порт и при получении запроса возвращает
    ответ "a101Y" и переходит в режим приЄма прошивки.
    ѕрошивка происходт так :
    1. update101.exe посылает размер прошивки в виде "163" с завершающим 0x0D и переходит в режим прослушивани€
    2. устройство посылает запрос "?".ѕолучив запрос, updater101.exe посылает 1024 байта прошивки.
    3. получив 1024 байта, устройство записывает их во флеш - пам€ть
    4. если получены не все байты, происходит переход к пункту 2.
    5. переход на основную программу и завершение работы update101.exe
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
