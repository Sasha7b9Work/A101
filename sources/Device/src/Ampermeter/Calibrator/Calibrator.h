// 2022/11/01 12:57:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Calibrator
{
    // ��������� ��������� ����������
    void ExecuteCalibration();

    // true ��������, ��� ��������� � �������� ����������
    bool InProcess();

    namespace OnEvent
    {
        void ButtonReady();
    }
}
