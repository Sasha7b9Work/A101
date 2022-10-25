// 2022/10/19 09:03:08 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/DiagramInput.h"
#include "Display/Painter.h"
#include <limits>


using namespace Primitives;


namespace DiagramInput
{
    static BufferADC data;

    static const int width = 390;               // Столько точек графика выводится
//    static const int height = 256;              // Таков размах по вре
//    static const int y0 = height / 2;

    static void CalculateMinMax(float *, float *);
}


void DiagramInput::SetData(const BufferADC &_data)
{
    data = _data;
}


void DiagramInput::Draw()
{
    float min = 0.0f;
    float max = 0.0f;

    CalculateMinMax(&min, &max);

//    float scale = height / (max - min);

    for (int i = 1; i < width; i++)
    {
//        int y1 = (int)((data.At(i - 1) - min) * scale + y0);
//        int y2 = (int)((data.At(i) - min) * scale + y0);


    }
}


void DiagramInput::CalculateMinMax(float *out_min, float *out_max)
{
    float min = std::numeric_limits<float>::max();
    float max = std::numeric_limits<float>::min();

    for (int i = 0; i < data.Size(); i++)
    {
        if (data.At(i) < min)
        {
            min = data.At(i);
        }

        if (data.At(i) > max)
        {
            max = data.At(i);
        }
    }

    *out_min = min;
    *out_max = max;
}
