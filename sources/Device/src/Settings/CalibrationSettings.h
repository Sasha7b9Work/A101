// 2022/11/22 08:45:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    struct Gain
    {
        Gain(float v = 1.0f) : value(v) { }
        void Set(float v) { value = v; }
        float Get() const { return value; }
    private:
        float value;
    };

    uint size;                      // ����� ������ �������� - ��� �������� ����, ��� ������ �������������
    uint crc32;                     // ����� ����������� ����� - ��� �������� ������������ ����������

    Gain gain[6];
    float GetGain(int range);       // ����������� ��������
    void SetGainK(int range, float);

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
        // ��������� �� ���������� ��������� ��� ������������ ��������������
        static void Store(const CalibrationSettings &);

        // ������������ ����� ���������� ���������
        static void Restore(CalibrationSettings &);

    private:

        static CalibrationSettings stored;
    };

    void Save();
    void Load();
};


extern CalibrationSettings cal;
