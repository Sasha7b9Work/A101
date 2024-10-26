// 2022/10/18 16:57:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct SampleRate
{
    enum E
    {
        _10us,      // Входная частота 40 Гц - 5 кГц
        _100us,     // Входная частота 4 Гц - 40 Гц
        _1000us,    // Входная частота 0.4 Гц - 4 Гц
        Count
    };


    // Время в микросекундах между точками дискретизации
    static uint TimeUSonPoint();

    static void Set(E v) { value = v; }

    static E Get()       { return value; }

    // Минимальная измеряемая частота на данном диапазоне
    static float MinFrequency();

    // Максимальная измеряемая частота на данном диапазоне
    static float MaxFrequency();

private:

    static E value;
};


struct ValueADC
{
    // В конструктор подаётся считанное с АЦП значение, которое подвергается корректировке
    explicit ValueADC(int _reading = 0);
    static ValueADC FromRaw(int raw) { ValueADC result; result.value = raw; return result; }
    REAL Real() const { return 5.0 / (1 << 17) * (REAL)value; }
    int Raw()    const { return value; }
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

    void ResetValue();

    // Читает готовое к употреблению значение
    ValueADC ReadValue();

    // Читает голое значение с АЦП
    int ReadValueRAW();

    // Возвращает усреднённое значение по максимальному количеству выборок
    int GetAverageValue();
}
