// 2022/11/01 15:04:23 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Ampermeter/Calculator/FFT.h"
#include "Hardware/Timer.h"
#include "Hardware/Timer.h"
#include <cmath>
#include <cstdlib>


FFT::FFT(const BufferADC &_data)
{
    double *in = (double *)std::malloc(sizeof(double) * BufferADC::SIZE);

    double out[BufferADC::SIZE];

    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        in[i] = _data[i].Real();
    }

    CalculateFFT(in, out);

    for (int i = 0; i < SIZE; i++)
    {
        data[i] = (uint8)(255.0 * out[i]);
    }

    std::free(in);
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


void FFT::CalculateFFT(double dataR[BufferADC::SIZE], double result[BufferADC::SIZE])
{
    for (int i = 0; i < BufferADC::SIZE; i++)
    {
        result[i] = 0.0;
    }

    int logN = GetLogN();

    static const double Rcoef[14] =
    {
        -1.0000000000000000, 0.0000000000000000, 0.7071067811865475,
         0.9238795325112867, 0.9807852804032304, 0.9951847266721969,
         0.9987954562051724, 0.9996988186962042, 0.9999247018391445,
         0.9999811752826011, 0.9999952938095761, 0.9999988234517018,
         0.9999997058628822, 0.9999999264657178
    };

    static const double Icoef[14] =
    {
         0.0000000000000000, -1.0000000000000000, -0.7071067811865474,
        -0.3826834323650897, -0.1950903220161282, -0.0980171403295606,
        -0.0490676743274180, -0.0245412285229122, -0.0122715382857199,
        -0.0061358846491544, -0.0030679567629659, -0.0015339801862847,
        -0.0007669903187427, -0.0003834951875714
    };

    int nn = BufferADC::SIZE >> 1;
    int ie = BufferADC::SIZE;

    for (int n = 1; n <= logN; n++)
    {
        double rw = Rcoef[logN - n];
        double iw = Icoef[logN - n];
        int in = ie >> 1;
        double ru = 1.0;
        double iu = 0.0;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < BufferADC::SIZE; i += ie)
            {
                int io = i + in;
                double dRi = dataR[i]; //-V2563
                double dRio = dataR[io]; //-V2563
                double ri = result[i]; //-V2563
                double rio = result[io]; //-V2563
                dataR[i] = dRi + dRio; //-V2563
                result[i] = ri + rio; //-V2563
                double rtq = dRi - dRio;
                double itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu; //-V2563
                result[io] = itq * ru + rtq * iu; //-V2563
            }
            double sr = ru;
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
            double rtp = dataR[in]; //-V2563
            double itp = result[in]; //-V2563
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
        result[i] = std::sqrt(dataR[i] * dataR[i] + result[i] * result[i]); //-V2563
    }

    result[0] = 0.0;       // \todo нулева€ составл€юща€ мешает посто€нно. надо еЄ убрать //-V2563

    Normalize(result, num_points);
}


void FFT::Normalize(double *in, int num_points)
{
    double max = 0.0;

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
