// 2022/11/22 08:45:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    struct Gain
    {
        Gain(int _range = -1, REAL _value = 1.0) : range(_range), value(_value) { }
        void Set(REAL v) { value = v; }
        REAL Get() const;
    private:
        int range;
        REAL value;
    };

    struct Zero
    {
        Zero(int _value = 0) : const_val(_value), var_val(0)  { }
        void SetConst(int v) { const_val = v; }
        void SetVar(int v) { var_val = v; }
        int GetFull() const { return const_val + var_val; }

        int GetConst() const { return const_val; }
        int GetVar() const { return var_val; }

    private:
        int const_val;      // Постоянное смещение, получается при калибровке
        int var_val;        // Плавающее смещение, измеряется каждый 10 секунд
    };

    uint size;              // Здесь размер настроек - для проверки того, что версии соответствуют
    uint crc32;             // Здесь контрольная сумма - для проверки правильности сохранения

    Gain gain[6];           // Растяжка
    Zero zero[6];           // Смещения нуля

    uint CalculateCRC32() const;
    bool IsEqual(const CalibrationSettings *) const;

    const uint8 *PointerToFirstData() const;
    uint SizeData() const;

    struct Storage
    {
        // Сохранить во внутреннем хранилище для последующего восстановления
        static void Store(const CalibrationSettings &);

        // Восстановить ранее сохранённые настройки
        static void Restore(CalibrationSettings &);

    private:

        static CalibrationSettings stored;
    };

    void Save();
    void Load();
    void Reset();
    void Reset(int range);
};


extern CalibrationSettings cal;
