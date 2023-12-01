// 2022/10/18 15:54:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Measure
{
    Measure(float _value, bool _out_of_range, bool _correct) :
        value(_value),
        out_of_range(_out_of_range),
        correct(_correct)
    {
    }

    bool IsValid() const
    {
        return !out_of_range && correct;
    }

    float value = 0.0;
    bool out_of_range = false;
    bool correct = false;
};


namespace Ampermeter
{
    void Init();

    void Update();

    // ������ ���� ��������� - ������ ������ � ��������� ����������
    void MeasurementCycle();

    void OnEventChangeRange();

    // ���� out_of_range == true - ����� �� ������� ���������
    Measure GetDC();
    Measure GetAC();
    Measure GetAmpl();
    Measure GetPeak();
    Measure GetMin();
    Measure GetMax();

    namespace Set
    {
        namespace ZeroDC
        {
            void Enable();

            void Disable();

            float Level();
        }

        namespace ZeroAC
        {
            void Enable();

            void Disable();

            float Level();
        }
    }
}
