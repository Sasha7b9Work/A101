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
        0.00000002711726268756f,
        0.00000013558631343779f,
        0.00000027117262687557f,
        0.00000027117262687557f,
        0.00000013558631343779f,
        0.00000002711726268756f
    };

    float BCoef[NCoef + 1] = {
         1.00000000000000000000f,
        -4.79668159981780740000f,
         9.20724237509200980000f,
        -8.84036968250099160000f,
         4.24578647328991910000f,
        -0.81597668002427803000f
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
