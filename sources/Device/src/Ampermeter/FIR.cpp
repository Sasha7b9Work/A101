// 2022/11/15 16:06:51 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/FIR.h"


/*
    Filter type: Low Pass
    Filter model: Chebyshev
    Filter order: 2
    Sampling Frequency: 4 KHz
    Cut Frequency: 1.000000 KHz
    Pass band Ripple: 1.000000 dB
    Coefficents Quantization: float
    
    Z domain Zeros
    z = -1.000000 + j 0.000000
    z = -1.000000 + j 0.000000
    
    Z domain Poles
    z = -0.032032 + j -0.559413
    z = -0.032032 + j 0.559413
*/


int FIR::Step(int new_sample)
{
#define Ntap 31

    double FIRCoef[Ntap] = {
        0.00000722935744402699,
        -0.00005005843732972412,
        -0.00001382507572602752,
        0.00017592806947777754,
        0.00000861943066253864,
        -0.00061768926620859471,
        0.00011198921451194402,
        0.00217866986852250200,
        -0.00093288365564642226,
        -0.00779322899968421920,
        0.00559702404769674140,
        0.02883242436274710600,
        -0.03272744541739570900,
        -0.11647319476762476000,
        0.27794553656611787000,
        0.68750180940486982000,
        0.27794553656611787000,
        -0.11647319476762476000,
        -0.03272744541739570900,
        0.02883242436274710600,
        0.00559702404769674140,
        -0.00779322899968421920,
        -0.00093288365564642226,
        0.00217866986852250200,
        0.00011198921451194402,
        -0.00061768926620859471,
        0.00000861943066253864,
        0.00017592806947777754,
        -0.00001382507572602752,
        -0.00005005843732972412,
        0.00000722935744402699
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
