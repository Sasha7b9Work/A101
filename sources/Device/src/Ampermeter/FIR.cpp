// 2022/11/15 16:06:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/FIR.h"


/*
Filter type: Low Pass
Filter model: Butterworth
Filter order: 5
Sampling Frequency: 100 KHz
Cut Frequency: 1.000000 KHz
Coefficents Quantization: float

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.939071 + j -0.000010
z = 0.949781 + j -0.035114
z = 0.949781 + j 0.035114
z = 0.979028 + j -0.058577
z = 0.979028 + j 0.058577
*/


int FIR::Step(int new_sample)
{
#define NCoef 5

    float ACoef[NCoef + 1] = {
        0.00127925860939995530f,
        0.00639629304699977640f,
        0.01279258609399955300f,
        0.01279258609399955300f,
        0.00639629304699977640f,
        0.00127925860939995530f
    };

    float BCoef[NCoef + 1] = {
         1.00000000000000000000f,
        -2.97542210974568280000f,
         3.80601811932040990000f,
        -2.54525286833046580000f,
         0.88113007543783606000f,
        -0.12543062215535555000f
    };

    static float y[NCoef + 1]; //output samples
    static float x[NCoef + 1]; //input samples
    int n;

    //shift the old samples
    for (n = NCoef; n > 0; n--) {
        x[n] = x[n - 1];
        y[n] = y[n - 1];
    }

    //Calculate the new output
    x[0] = (float)new_sample;
    y[0] = ACoef[0] * x[0];
    for (n = 1; n <= NCoef; n++)
        y[0] += ACoef[n] * x[n] - BCoef[n] * y[n];

    return (int)y[0];
}
