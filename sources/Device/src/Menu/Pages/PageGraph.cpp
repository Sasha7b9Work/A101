// 2022/10/24 12:16:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Nextion/DiagramInput.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/Calibrator/Calibrator.h"
#include "Nextion/Display.h"


namespace PageGraph
{
    extern ButtonMenuPress btnTypeGraph;
    extern ButtonMenuPress btnTypeSignal;

    static void SetTitleButtonTypeGraph()
    {
        static const pchar titles[2][Lang::Count] =
        {
            {"Сигнал", "Signal"},
            {"FFT",    "FFT"}
        };

        btnTypeGraph.SetText(titles[set.type_graph.GetValue()][Lang::RU], titles[set.type_graph.GetValue()][Lang::EN]);

        btnTypeGraph.Refresh();

        static const pchar signals[2][Lang::Count] =
        {
            {"DC", "DC"},
            {"AC", "AC"}
        };

        btnTypeSignal.SetText(signals[set.type_signal.GetValue()][Lang::RU], signals[set.type_signal.GetValue()][Lang::EN]);

        btnTypeSignal.Refresh();
    }

    static void FuncOnEnter()
    {
        DiagramInput::Reset(true);

        SetTitleButtonTypeGraph();

        btnTypeSignal.SetShown(set.type_graph.IsSignal());

        btnTypeSignal.Refresh();
    }

    static void FuncDraw()
    {
        DiagramInput::Draw();
    }

    ButtonMenuPress btnTypeGraph("Сигнал", "Signal", 0, 0, [](Item *, bool press)
    {
        if (!press)
        {
            set.type_graph.Increase();

            FuncOnEnter();
        }
    });

    ButtonMenuPress btnTypeSignal("DC", "DC", 1, 0, [](Item *, bool press)
    {
        if (!press)
        {
            set.type_signal.Increase();

            FuncOnEnter();
        }
    });

    static ButtonMenuPress btnBack("Назад", "Back", 2, 0, [](Item *, bool press)
    {
        if (!press)
        {
            PageMain::self->SetAsCurrent();
        }
    });

    static Item *items[] =
    {
        &btnTypeGraph,          // FFT или f(t) выводим
        &btnTypeSignal,         // AC или DC выводим
        &btnBack,
        nullptr
    };

    static Page pageTwo(items, FuncOnEnter, FuncDraw);

    Page *self = &pageTwo;
}
