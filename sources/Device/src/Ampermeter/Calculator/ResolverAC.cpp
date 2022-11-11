// 2022/11/10 10:47:13 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/ResolverAC.h"
#include <cmath>


//static float CalculateFiltr(float new_sample)
//{
//    const int Ntap = 31;
//
//    float FIRCoef[Ntap] = {
//    0.03023653529126812300f,
//    0.03056112171209973300f,
//    0.03087200402120585800f,
//    0.03116913300713195300f,
//    0.03145258704421011900f,
//    0.03172261580054078900f,
//    0.03197970674725595500f,
//    0.03222469068053961100f,
//    0.03245891765660298400f,
//    0.03268456866515347100f,
//    0.03290525173385174700f,
//    0.03312726297072489400f,
//    0.03336265542653085700f,
//    0.03363846319293176100f,
//    0.03403673732977624500f,
//    0.03513549744035179100f,
//    0.03403673732977624500f,
//    0.03363846319293176100f,
//    0.03336265542653085700f,
//    0.03312726297072489400f,
//    0.03290525173385174700f,
//    0.03268456866515347100f,
//    0.03245891765660298400f,
//    0.03222469068053961100f,
//    0.03197970674725595500f,
//    0.03172261580054078900f,
//    0.03145258704421011900f,
//    0.03116913300713195300f,
//    0.03087200402120585800f,
//    0.03056112171209973300f,
//    0.03023653529126812300f
//    };
//
//    static float x[Ntap]; //input samples
//    float y = 0;            //output sample
//    int n;
//
//    //shift the old samples
//    for (n = Ntap - 1; n > 0; n--)
//        x[n] = x[n - 1];
//
//    //Calculate the new output
//    x[0] = new_sample;
//    for (n = 0; n < Ntap; n++)
//        y += FIRCoef[n] * x[n];
//
//    return y;
//}


ResolverAC::ResolverAC(const Period &period)
{
    float sum = 0.0f;
    float dc = period.dc.Real();

    for (int i = period.first.first; i < period.last.first; i++)
    {
        float value = BufferADC::At(i).Real() - dc;

        sum += value * value;
    }

    result = std::sqrtf(sum / (period.last.first - period.first.first));
}
