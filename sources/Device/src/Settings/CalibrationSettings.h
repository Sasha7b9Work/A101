// 2022/11/22 08:45:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct CalibrationSettings
{
    uint size;                      // ����� ������ �������� - ��� �������� ����, ��� ������ �������������
    uint crc32;                     // ����� ����������� ����� - ��� �������� ������������ ����������

    float gain[6];
    int zero[6];

    float GetGain(int range);       // ����������� ��������
    void SetGainK(int range, float);
    float GetGainK(int range);

    void Save();
    void Load();

    void Reset();

    int GetZero(int range);
    void SetZero(int range, int);

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
};


extern CalibrationSettings cal;
