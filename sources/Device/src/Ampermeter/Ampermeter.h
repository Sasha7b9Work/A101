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

    // Абсолютное значение. Перед выводом нужно приводить к пределу
    REAL value_abs = 0.0;
    bool out_of_range = false;
    bool correct = false;

    static REAL LimitFrequency(REAL value);

    // Возвращате макимальное измеряемое значение тока на данном диапазоне
    static REAL MaxIAbs(int range);
};


namespace Ampermeter
{
    void Init();

    void Update();

    // Полный цикл измерений - чтение данных и получение результата
    bool MeasurementCycle();

    void OnEventChangeRange();

    // Считанные значения выходят за пределы диапазона
    bool OutOfRange();

    Measure GetDC();
    Measure GetPeak();
    Measure GetMin();
    Measure GetMax();

    // Автоматический выбор предела
    namespace AVP
    {
        void Enable();

        void Disable();
    }

    // Режим "Ноль DC"
    namespace ZeroDC
    {
        void Enable();

        void Disable();

        // Установка нуля для пределов 20 и 50 А.
        // В связи со спецификой преобразователя
        namespace FloatingZero
        {
            void Process();
        }

        // Возвращает полное смещение - с учётом FloatingZero
        REAL LevelAbsFull();
    }

    // Режим "Ноль AC"
    namespace ZeroAC
    {
        void Enable();

        void Disable();

        REAL LevelAbs();
    }
}
