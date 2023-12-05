// 2022/10/18 16:57:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SampleRate
{
    SampleRate(uint _time_points_us = current.TimeUS()) : time_points_us(_time_points_us) {}
    uint TimeUS() const { return time_points_us; }
    float Freq() const { return 1.0f / ((float)time_points_us * 1e-6f); }

    struct Current
    {
        static SampleRate Get() { return current; }
        static void Set(const SampleRate &rate) { current = rate; }
    };

private:
    uint time_points_us;
    static SampleRate current;
};


struct ValueADC
{
    // В конструктор подаётся считанное с АЦП значение, которое подвергается корректировке
    explicit ValueADC(int _reading = 0);
    static ValueADC FromRaw(int raw) { ValueADC result; result.value = raw; return result; }
    REAL Real() const { return 5.0 / (1 << 17) * (REAL)value; }
    int Raw()    const { return value; }
    static const ValueADC MIN;
    static const ValueADC MAX;
    bool operator <(const ValueADC &rhs) const { return value < rhs.value; }
    bool operator >(const ValueADC &rhs) const { return value > rhs.value; }
    operator int() const { return value; }
private:
    int value;        // Прочитанное значение, уже преобразованное
};


// АЦП
namespace AD7691
{
    void Init();

    ValueADC ReadValue();

    // Возвращает усреднённое значение по максимальному количеству выборок
    int GetAverageValue();
}
