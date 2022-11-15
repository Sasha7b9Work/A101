// 2022/11/15 16:06:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/FIR.h"


/*
    Filter type: Low Pass
    Filter model: Chebyshev
    Filter order: 2
    Sampling Frequency: 4 KHz
    Cut Frequency: 1.000000 KHz
    Pass band Ripple: 0.000000 dB
    Coefficents Quantization: float
    
    Z domain Zeros
    z = -1.000000 + j 0.000000
    z = -1.000000 + j 0.000000
    
    Z domain Poles
    z = -0.986147 + j -0.013664
    z = -0.986147 + j 0.013664
*/


int FIR::Step(int new_sample)
{
#define Ntap 10

    double FIRCoef[Ntap] = {
        -0.00526783972344176120,
        0.00527555359482286420,
        -0.00528110408207756260,
        0.00528445205476381870,
        0.99471987388510108000,
        0.00528445205476381870,
        -0.00528110408207756260,
        0.00527555359482286420,
        -0.00526783972344176120,
        0.00525800242676436120
    };

    static double x[Ntap];   //input samples
    double y = 0.0;            //output sample
    int n;

    //shift the old samples
    for (n = Ntap - 1; n > 0; n--)
        x[n] = x[n - 1];

    //Calculate the new output
    x[0] = (double)new_sample;
    for (n = 0; n < Ntap; n++)
        y += FIRCoef[n] * x[n];

    return (int)y;
}
