// 2022/10/19 08:16:06 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Ampermeter/BufferADC.h"
#include "Ampermeter/Ampermeter.h"


namespace Calculator
{
    // Функция возвращает частоту дискретизации для следующего сбора данных
    SampleRate AppendData();

    void Reset();

    // Действующее значение переменной составляющей
    REAL GetAbsAC(bool *correct);
    Measure GetMeasureAC();

    // Действующее значение полного тока
    REAL GetAbsDC(bool *correct);
    Measure GetMeasureDC();

    // Минимальное установившееся значение тока
    REAL GetValueMinSteady(bool *correct);
    Measure GetMeasureMinSteady();

    // Максимальное установившееся значение тока
    REAL GetValueMaxSteady(bool *correct);
    Measure GetMeasureMaxSteady();

    // Разница между макс и мин
    Measure GetMeasureAmplSteady();

    // Разница между пиковыми значениями - мгновенными мин и макс
    Measure GetMeasurePeak();

    Measure GetMeasureFrequency();
}
