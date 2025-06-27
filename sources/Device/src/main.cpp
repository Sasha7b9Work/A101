// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Device.h"

#ifndef WIN32
    #if __ARMCC_VERSION > 6180002
        // �� ����� ������� ������������ ������ �������� �� ������������ ���������� (20�) ��� fast ����������
        #error "Requires ARM Compiler V6.18 from uVision 5.37"
    #endif
#endif


int main()
{
    Device::Init();

    while (1)
    {
        Device::Update();
    }
}
