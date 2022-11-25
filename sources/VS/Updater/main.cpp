// 2022/04/29 13:57:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "rs232.h"
#include "ComPort.h"
#include "Timer.h"
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
static void Update();
static void CallbackOnReceive(char);


static ComPort port;

static ifstream file;

static bool a101_connected = false;     // true, если с при бором установлена св€зь
static bool updated = false;            // true, если апдейт произведЄн
static uint sended_bytes = 0;           // переслано байт прошивки


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        WriteErrorMessage();

        getch();

        return -1;
    }

    file.open(argv[2], ifstream::in | ifstream::binary);

    if (!file.is_open())
    {
        cout << "Can not open file \"" << argv[2] << "\"" << endl;

        getch();

        return -1;
    }

    port.Open(argv[1], CallbackOnReceive);

    Update();

    cout << "Update succefull" << endl;

    file.close();

    getch();

    return 0;
}


static void WriteErrorMessage()
{
    cout << "Invalid syntx. Usage:" << endl;
    cout << "\t update101.exe com1 a101.bin" << endl;
}


static void Update()
{
    while (!updated)
    {
        if (!a101_connected)
        {
            TimeMeter().Wait(100);

            port.Send("a101?");

            cout << "Send request" << endl;
        }
    }
}


static void CallbackOnReceive(char symbol)
{
    static string received;

    static int size = 0;

    if (symbol == '\x0D')
    {
        if (received == "a101Y")
        {
            a101_connected = true;

            file.seekg(0, ios_base::end);

            size = (int)file.tellg();

            port.Send(to_string(size).c_str());

            file.seekg(0, ios_base::beg);
        }
        else if (received == "?")
        {
            uint8 buffer[1024];

            file.read((char *)buffer, 1024);

            port.SendBuffer(buffer, (int)file.gcount());

            cout << (float)size / (float)file.tellg() * 100.0f << " %%" << endl;

            if (file.eof())
            {
                updated = true;
            }
        }

        received.clear();
    }
    else
    {
        received.push_back(symbol);
    }
}
