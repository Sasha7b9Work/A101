// 2022/10/29 10:28:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Measurer/AD7691.h"
#include <cstdlib>


void AD7691::Init()
{

}


int AD7691::ReadValue()
{
    return std::rand() - 100000;
}
