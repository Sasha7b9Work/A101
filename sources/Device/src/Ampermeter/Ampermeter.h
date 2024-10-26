// 2022/10/18 15:54:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Measure
{
    Measure(REAL _value, bool _out_of_range, bool _correct) :
        value_abs(_value),
        out_of_range(_out_of_range),
        correct(_correct)
    {
    }

    bool IsValid() const
    {
        return !out_of_range && correct;
    }

    // ���������� ��������. ����� ������� ����� ��������� � �������
    REAL value_abs = 0.0;
    bool out_of_range = false;
    bool correct = false;

    static REAL LimitFrequency(REAL value);

    // ���������� ����������� ���������� �������� ���� �� ������ ���������
    static REAL MaxIAbs(int range);
};


namespace Ampermeter
{
    void Init();

    void Update();

    // ������ ���� ��������� - ������ ������ � ��������� ����������
    bool MeasurementCycle();

    void OnEventChangeRange();

    // ��������� �������� ������� �� ������� ���������
    bool OutOfRange();

    Measure GetDC();
    Measure GetPeak();
    Measure GetMin();
    Measure GetMax();

    // �������������� ����� �������
    namespace AVP
    {
        void Enable();

        void Disable();
    }

    // ����� "���� DC"
    namespace ZeroDC
    {
        void Enable();

        void Disable();

        // ��������� ���� ��� �������� 20 � 50 �.
        // � ����� �� ���������� ���������������
        namespace FloatingZero
        {
            void Process();
        }

        // ���������� ������ �������� - � ������ FloatingZero
        REAL LevelAbsFull();
    }

    // ����� "���� AC"
    namespace ZeroAC
    {
        void Enable();

        void Disable();

        REAL LevelAbs();
    }
}
