// 2022/10/18 16:18:44 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct BufferADC
{
    static const int SIZE = 1024;

    BufferADC() : pointer(0) { } //-V730

    void Push(int word)
    {
        if (pointer < SIZE)
        {
            raw[pointer++] = word;
        }
    }

    bool IsFull()     { return (pointer == SIZE); }

    void Clear(double sample_rate) { pointer = 0; sample_rate_HZ = sample_rate; }

    int Size() const  { return SIZE; }

    double At(int pos) { return volt[pos]; }

    void ConvertToVoltage();

    double MinReal() const { return min; }

    double MaxReal() const { return max; }

    double sample_rate_HZ;

private:
    int    pointer;         // Указатель используется при чтении данных (массив raw)
    int    raw[SIZE];       // Данные, считанные с АЦП
    double volt[SIZE];      // Реальные значения с АЦП
    int    min_raw;
    int    max_raw;
    double min;
    double max;
};
