// 2022/11/01 15:04:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/FFT.h"
#include "Hardware/Timer.h"
#include "Hardware/Timer.h"
#include <cmath>
#include <cstdlib>


FFT::FFT(const BufferADC &_data)
{
    float *in = (float *)std::malloc(sizeof(float) * BufferADC::SIZE); //-V829

    float *out = (float *)std::malloc(sizeof(float) * BufferADC::SIZE);

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        in[i] = (float)_data[i].Real(); //-V522
    }

    TimeMeterMS meter;

    CalculateFFT(in, out);

    LOG_WRITE("time fft %d ms", meter.ElapsedTime());

    for (int i = 0; i < SIZE; i++)
    {
        data[i] = (uint8)(255.0f * out[i]); //-V522
    }

    std::free(in);
    std::free(out);
}


int FFT::FindIndexFreq() const
{
    for (int i = 1; i < BufferADC::SIZE / 2; i++)
    {
        if (data[i] == 255)
        {
            return i;
        }
    }

    for (int i = 1; i < BufferADC::SIZE / 2; i++)
    {
        if (data[i] == 254)
        {
            return i;
        }
    }

    return -1;
}


int FFT::GetLogN()
{
    if (BufferADC::SIZE == 4096)
    {
        return 12;
    }
    else if (BufferADC::SIZE == 2048)
    {
        return 11;
    }

    return 10;
}


void FFT::CalculateFFT(float dataR[BufferADC::SIZE], float result[BufferADC::SIZE])
{
    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        result[i] = 0.0;
    }

    int logN = GetLogN();

    static const float Rcoef[14] =
    {
        -1.0000000000000000f, 0.0000000000000000f, 0.7071067811865475f,
         0.9238795325112867f, 0.9807852804032304f, 0.9951847266721969f,
         0.9987954562051724f, 0.9996988186962042f, 0.9999247018391445f,
         0.9999811752826011f, 0.9999952938095761f, 0.9999988234517018f,
         0.9999997058628822f, 0.9999999264657178f
    };

    static const float Icoef[14] =
    {
         0.0000000000000000f, -1.0000000000000000f, -0.7071067811865474f,
        -0.3826834323650897f, -0.1950903220161282f, -0.0980171403295606f,
        -0.0490676743274180f, -0.0245412285229122f, -0.0122715382857199f,
        -0.0061358846491544f, -0.0030679567629659f, -0.0015339801862847f,
        -0.0007669903187427f, -0.0003834951875714f
    };

    int nn = BufferADC::SIZE >> 1;
    int ie = BufferADC::SIZE;

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0;
        float iu = 0.0;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < BufferADC::SIZE; i += ie)
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

    for (int j = 1, i = 1; i < BufferADC::SIZE; i++)
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

    int num_points = BufferADC::SIZE / 2;

    for (int i = 0; i < num_points; i++)
    {
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]); //-V2563
    }

    result[0] = 0.0;       // \todo нулева€ составл€юща€ мешает посто€нно. надо еЄ убрать //-V2563

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
