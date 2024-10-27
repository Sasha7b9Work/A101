// 2022/11/01 15:04:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/Resolvers.h"
#include "Hardware/Timer.h"
#include "Hardware/Timer.h"
#include <cmath>
#include <cstdlib>


ResolverFFT::ResolverFFT(int delta)
{
    Buffer<NUM_POINTS, float> in;
    Buffer<NUM_POINTS, float> out;

    for (int i = 0; i < NUM_POINTS; i += delta)
    {
        float value = (float)BufferADC::At(i).Real(); //-V522

        in.Append(value);
    }

    TimeMeterMS meter;

    CalculateFFT(in.Data(), out.Data());

    LOG_WRITE("time fft %d ms", meter.ElapsedTime());

    for (int i = 0; i < SIZE_DATA; i++)
    {
        data[i] = (uint8)(255.0f * out[(uint)i]); //-V522
    }
}


int ResolverFFT::FindIndexFreq() const
{
    for (int i = 1; i < SIZE_DATA / 2; i++)
    {
        if (data[i] == 255)
        {
            return i;
        }
    }

    for (int i = 1; i < SIZE_DATA / 2; i++)
    {
        if (data[i] == 254)
        {
            return i;
        }
    }

    return -1;
}


void ResolverFFT::CalculateFFT(float dataR[NUM_POINTS], float result[NUM_POINTS])
{
    for (int i = 0; i < NUM_POINTS; i++)
    {
        result[i] = 0.0;
    }

    ApplyHamming(dataR, NUM_POINTS);

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

    int nn = NUM_POINTS >> 1;
    int ie = NUM_POINTS;

    for (int n = 1; n <= LOG_N; n++)
    {
        float rw = Rcoef[LOG_N - n];
        float iw = Icoef[LOG_N - n];
        int in = ie >> 1;
        float ru = 1.0;
        float iu = 0.0;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < NUM_POINTS; i += ie)
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

    for (int j = 1, i = 1; i < NUM_POINTS; i++)
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

    for (int i = 0; i < SIZE_DATA; i++)
    {
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]); //-V2563
    }

    Normalize(result, NUM_POINTS / 2);

    TransformToLogarifm(result, NUM_POINTS / 2);
}


void ResolverFFT::Normalize(float *buf, uint num_points)
{
    float max = 0.0;

    for (uint i = 0; i < num_points / 2; i++)
    {
        if (buf[i] > max) //-V2563
        {
            max = buf[i]; //-V2563
        }
    }

    for (uint i = 0; i < num_points / 2; i++)
    {
        buf[i] /= max; //-V2563
    }
}


void ResolverFFT::ApplyHamming(float *buf, uint num_points)
{
    for (uint i = 0; i < num_points; i++)
    {
        buf[(uint)i] *= 0.53836f - 0.46164f * std::cosf(2.0f * 3.1415926f * i / (num_points - 1));
    }
}


void ResolverFFT::TransformToLogarifm(float *buf, uint num_points)
{
    const float minDB = -40.0f;

    for (uint i = 0; i < num_points; i++)
    {
        buf[i] = 10 * std::log10f(buf[i]);
    }

    for (uint i = 0; i < num_points; i++)
    {
        float sub = buf[i] / minDB;

        if (sub > 1.0f)
        {
            sub = 1.0f;
        }

        buf[i] = 1.0f - sub;
    }
}
