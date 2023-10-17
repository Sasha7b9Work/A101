// 2022/11/22 08:45:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    struct Gain
    {
        Gain(int _range = -1, float _value = 1.0f) : range(_range), value(_value) { }
        void Set(float v) { value = v; }
        float Get() const;
    private:
        int range;
        float value;
    };

    uint size;                      // Здесь размер настроек - для проверки того, что версии соответствуют
    uint crc32;                     // Здесь контрольная сумма - для проверки правильности сохранения

    Gain gain[6];

    int zero[6];
    int GetZero(int range);
    void SetZero(int range, int);

    void Reset();

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
};


extern CalibrationSettings cal;
