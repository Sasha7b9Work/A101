// 2022/11/15 16:06:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/FIR.h"


/*
Filter type: Low Pass
Filter model: Chebyshev
Filter order: 6
Sampling Frequency: 100 KHz
Cut Frequency: 25.000000 KHz
Pass band Ripple: 1.000000 dB
Coefficents Quantization: float

Z domain Zeros
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000
z = -1.000000 + j 0.000000

Z domain Poles
z = 0.233088 + j -0.766518
z = 0.233088 + j 0.766518
z = 0.004382 + j -0.939354
z = 0.004382 + j 0.939354
z = 0.550903 + j -0.335068
z = 0.550903 + j 0.335068
*/


int FIR::Step(int new_sample)
{
#define Ntap 10

    static const float FIRCoef[Ntap] = {
         0.02065669729811002800f,
        -0.10052613592106527000f,
        -0.02458774331870564400f,
         0.31633576202006775000f,
         0.52573151785445693000f,
         0.31633576202006775000f,
        -0.02458774331870564400f,
        -0.10052613592106527000f,
         0.02065669729811002800f,
         0.05051132198872929100f
    };

    static float x[Ntap];   //input samples
    float y = 0.0;            //output sample
    int n;

    //shift the old samples
    for (n = Ntap - 1; n > 0; n--)
        x[n] = x[n - 1];

    //Calculate the new output
    x[0] = (float)new_sample;
    for (n = 0; n < Ntap; n++)
        y += FIRCoef[n] * x[n];

    return (int)y;
}
