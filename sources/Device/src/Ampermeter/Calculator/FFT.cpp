// 2022/11/01 15:04:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/FFT.h"
#include <cmath>


FFT::FFT(const BufferADC &_data)
{
    float in[1024];

    float out[1024];

    for (int i = 0; i < 1024; i++)
    {
        in[i] = _data[i].Real();
    }

    CalculateFFT(in, out);

    for (int i = 0; i < SIZE; i++)
    {
        data[i] = (uint8)(255.0f * in[i]);
    }

    LOG_WRITE("index freq - %d", FindFreq());
}


int FFT::FindFreq()
{
    for (int i = 0; i < 512; i++)
    {
        if (data[i] == 255)
        {
            return i;
        }
    }

    return -1;
}


void FFT::CalculateFFT(float dataR[1024], float result[1024])
{
    for (int i = 0; i < 1024; i++)
    {
        result[i] = 0.0F;
    }

    int logN = 10;

    static const float Rcoef[14] =
    {
        -1.0000000000000000F, 0.0000000000000000F, 0.7071067811865475F,
        0.9238795325112867F, 0.9807852804032304F, 0.9951847266721969F,
        0.9987954562051724F, 0.9996988186962042F, 0.9999247018391445F,
        0.9999811752826011F, 0.9999952938095761F, 0.9999988234517018F,
        0.9999997058628822F, 0.9999999264657178F
    };

    static const float Icoef[14] =
    {
        0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    int nn = 1024 >> 1;
    int ie = 1024;

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0F;
        float iu = 0.0F;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < 1024; i += ie)
            {
                int io = i + in;
                float dRi = dataR[i]; //-V2563
                float dRio = dataR[io]; //-V2563
                float ri = result[i]; //-V2563
                float rio = result[io]; //-V2563
                dataR[i] = dRi + dRio; //-V2563
                result[i] = ri + rio; //-V2563
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu; //-V2563
                result[io] = itq * ru + rtq * iu; //-V2563
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < 1024; i++)
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in]; //-V2563
            float itp = result[in]; //-V2563
            dataR[in] = dataR[io]; //-V2563
            result[in] = result[io]; //-V2563
            dataR[io] = rtp; //-V2563
            result[io] = itp; //-V2563
        }

        int k = nn;

        while (k < j)
        {
            j = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    int num_points = 512;

    for (int i = 0; i < num_points; i++)
    {
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]); //-V2563
    }

    result[0] = 0.0F;       // \todo нулева€ составл€юща€ мешает посто€нно. надо еЄ убрать //-V2563

    Normalize(result, num_points);
}


void FFT::Normalize(float *in, int num_points)
{
    float max = 0.0;

    for (int i = 0; i < num_points; i++)
    {
        if (in[i] > max) //-V2563
        {
            max = in[i]; //-V2563
        }
    }

    for (int i = 0; i < num_points; i++)
    {
        in[i] /= max; //-V2563
    }
}
