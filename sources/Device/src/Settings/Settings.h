// 2022/11/14 13:39:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/CalibrationSettings.h"
#include "Settings/Settings.h"


struct Lang
{
    enum E
    {
        RU,
        EN,
        Count
    };

    static bool IsRU();
};


struct TypeMeasure
{
    enum E
    {
        DC,
        AC,
        Ampl,
        Peak,
        Min,
        Max,
        Frequency,
        Count
    };

    TypeMeasure(E v) : value(v) { }

    bool ExistFieldSign() const;

    pchar Title(Lang::E lang) const;

    bool IsShown() const;

    bool IsFrequency() const { return value == Frequency; }

    // ���������� ������� ���������, ��������� �� �����, ��� ������ �������
    static void GetUnitsForFrequnesy(REAL freq, char ru[32], char en[32]);

    // ���������� ���������� ������ ����� �������, ��������� �� �����, ��� ������ �������
    static int GetNumDigitsAfterComma(REAL freq);

private:

    E value;
};


// ��� ��������� �� �������
struct MeasuresOnDisplay
{
    enum E
    {
        AC_DC,
        AC,
        DC,
        Count
    };

    MeasuresOnDisplay(E v = AC_DC) : current(v) { }

    void Set(E v) { current = v; }

    bool IsAC_DC() { return current == AC_DC; }
    bool IsAC() { return current == AC; }
    bool IsDC() { return current == DC; }

    E Current() { return current; }

private:
    E current;
};


struct Baudrate
{
    enum E
    {
        _9600,
        _115200,
        Count
    };

    Baudrate(E v = _9600) : current(v) { }
    uint ToRaw() const;
private:
    E current;
};


struct Parity
{
    enum E
    {
        No,
        Yes
    };
};


struct StopBits
{
    enum E
    {
        _1,
        _1_5,
        _2
    };
};


struct ChangeRange
{
    enum E
    {
        Hand,
        Auto
    };
};


// ����� �������� ������ ������ ��� ������ AC
struct TypeSignal
{
    enum E
    {
        Full,       // �������� ������ ������
        AC,         // �������� ������ ���������� ������������
        Count
    };

    TypeSignal(E v = Full) : value(v) { }

    bool IsFull() const { return (value == Full); }

private:

    E value;
};


// ��� �������� ������ - � ���� �������� ��� � ���� FFT
struct TypeGraph
{
    enum E
    {
        Signal,
        FFT,
        Count
    };

    TypeGraph(E v = Signal) : value(v) { }

    void Increase()
    {
        value = (E)(value + 1);

        if (value >= Count)
        {
            value = Signal;
        }
    }

    bool IsSignal() const
    {
        return value == Signal;
    }

    E GetValue() const { return value; }

private:

    E value;
};



struct Settings
{
    uint size;          // ����� ������ �������� - ��� �������� ����, ��� ������ �������������
    uint crc32;         // ����� ����������� ����� - ��� �������� ������������ ����������

    bool _middle_of_3;   // �������� ���������� �������
    bool smooth;        // ��������� ����������� �� �������� ������
    bool firLPF;        // ������� �������� ���

    bool en_DC;         // �������� ��������� ��������� DC
    bool en_AC;         // �������� ��������� ��������� ��

    TypeMeasure::E en_add_meas[3];  // ����� �������� ���������, ������� ����� �������� �� �����. ������ ��������� - ������� ������ �����, �.�. ���
                                    // ����� �������, ��������� � ����� ����� ������ ���������

    MeasuresOnDisplay meas_on_display;
    Baudrate          baudrate;
    Parity::E         parity;
    StopBits::E       stop_bits;
    ChangeRange::E    change_range;
    TypeGraph         type_graph;
    TypeSignal        type_signal;
    int16             brightness;       // ������� �� 10 �� 100

    uint serial_number;

    Lang::E lang;

    void Save();
    void Load();

    uint CalculateCRC32() const;
    bool IsEqual(const Settings *) const;

    const uint8 *PointerToFirstData() const;
    uint SizeData() const;

    struct Storage
    {
        static void Store(const Settings &);
        static void Restore(Settings &);
    };
};


extern Settings set;
