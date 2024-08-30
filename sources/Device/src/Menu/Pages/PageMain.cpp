// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"
#include "Ampermeter/Ampermeter.h"
#include "Nextion/Display.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#include <cstdio>


namespace PageMain
{
    Label labelZeroDC{ "ноль", "zero", 23, 170, 90, 50, Font::_1 };
    Label labelZeroAC{ "ноль", "zero", 23, 260, 90, 50, Font::_1 };

    _WindowMeasure_ wndDC(TypeMeasure::DC, "titleDC", "f_sign", "valueDC", Label{ "", "", 552, 143, 100, 85, Font::_4 }, "DC:");
    _WindowMeasure_ wndAC(TypeMeasure::AC, "titleAC", "",       "valueAC", Label{ "", "", 552, 233, 100, 85, Font::_4 }, "AC:");

    static _WindowMeasure_ wndAMPL(TypeMeasure::Ampl, "tIamp", "",          "tIampValue", Label{ "", "", 606, 27, 51, 40, Font::_0 }, "");
    static _WindowMeasure_ wndPEAK(TypeMeasure::Peak, "tIpp",  "",          "tIppValue",  Label{ "", "", 606, 70, 51, 40, Font::_0 }, "");
    static _WindowMeasure_ wndMIN(TypeMeasure::Min,   "tImin", "tIminSign", "tIminValue", Label{ "", "", 360, 70, 51, 40, Font::_0 }, "");
    static _WindowMeasure_ wndMAX(TypeMeasure::Max,   "tImax", "tImaxSign", "tImaxValue", Label{ "", "", 360, 27, 51, 40, Font::_0 }, "");

    static void ResetAllMeasures()
    {
        wndDC.Reset();
        wndAC.Reset();
        wndAMPL.Reset();
        wndPEAK.Reset();
        wndMIN.Reset();
        wndMAX.Reset();
    }

    static ButtonPress btnZero("Уст. 0", "Set 0", Font::_1, 669, 321, 127, 74, []()
    {
        Ampermeter::ZeroDC::FloatingZero::Process();
    });

    void OnEventChangeRange(int new_range)
    {
        ResetAllMeasures();

        btnZero.SetShown(new_range >= 4);
    }

    static void FuncOnEnter()
    {
        ResetAllMeasures();

        int range = Range::Current();

        if (range > 0)
        {
            Range::Set(range - 1);
            Range::Set(range);
        }
        else
        {
            Range::Set(range + 1);
            Range::Set(range);
        }

        for (int i = 0; i < 6; i++)
        {
            char name[32];
            std::sprintf(name, "bt%d", i);
            Nextion::SetValue(name, Range::Current() == i ? 1 : 0);
        }
    }

    static void FuncDraw()
    {
        Display::LabelStar::Update();

        wndDC.Draw(Ampermeter::GetDC(), Range::Current());

        wndAC.Draw(Ampermeter::GetAC(), Range::Current());

        Measure ampl = Ampermeter::GetAmpl();
        wndAMPL.Draw(ampl, Range::Current());

        wndPEAK.Draw(Ampermeter::GetPeak(), Range::Current());
        wndMIN.Draw(Ampermeter::GetMin(), Range::Current());
        wndMAX.Draw(Ampermeter::GetMax(), Range::Current());
    }

    // Вызывается при нажатии кнопки
    static void FuncOnRange(int range)
    {
        if (Range::Current() != range)
        {
            for (int i = 0; i < 6; i++)
            {
                if (i != range)
                {
                    PageMain::self->GetItem(i)->ToButtonRange()->SetValue(0);
                }
            }

            Ampermeter::AVP::Disable();
            Range::Set(range);
        }
    }

    static ButtonRange btn2mA("2 мА", "2 mA", 4, 402, []()
    {
        FuncOnRange(0);
    });


    static ButtonRange btn20mA("20 мА", "20 mA", 137, 402, []()
    {
        FuncOnRange(1);
    });

    static ButtonRange btn200mA("200 мА", "200 mA", 270, 402, []()
    {
        FuncOnRange(2);
    });

    static ButtonRange btn2A("2 А", "2 A", 403, 402, []()
    {
        FuncOnRange(3);
    });

    static ButtonRange btn20A("20 А", "20 A", 536, 402, []()
    {
        FuncOnRange(4);
    });

    static ButtonRange btn50A("50 А", "50 A", 669, 402, []()
    {
        FuncOnRange(5);
    });

    static ButtonOld btnAC_DC("btnACDC", "0AD", []()            // Переход в AC+DC
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC_DC);
    });

    static ButtonOld btnAC("btnACDC", "0AC", []()              // Переход в AC
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::AC);
    });

    static ButtonOld btnDC("btnACDC", "0DC", []()              // Переход в DC
    {
        MeasuresOnDisplay::Set(MeasuresOnDisplay::DC);
    });

    static ButtonOld btnCalibration("btnCalibr", "0C", []()
    {
        PageCalibration::self->SetAsCurrent();
    });

    static ButtonOld btnSettings("btnSettings", "0T", []()
    {
        PageSettings::self->SetAsCurrent();
    });

    static ButtonOld btnZeroDC_EN("btnZeroDC", "0DZ1", []()         // Включение режима "Zero DC"
    {
        Ampermeter::ZeroDC::Enable();
    });

    static ButtonOld btnZeroDC_DIS("btnZeroDC", "0DZ0", []()        // Выключение режима "Zero DC"
    {
        Ampermeter::ZeroDC::Disable();
    });

    static ButtonOld btnZeroAC_EN("btnZeroAC", "0AZ1", []()         // Включение режима "Zero AC"
    {
        Ampermeter::ZeroAC::Enable();
    });

    static ButtonOld btnZeroAC_DIS("btnZeroAC", "0AZ0", []()        // Выключение режима "Zero AC"
    {
        Ampermeter::ZeroAC::Disable();
    });

    static ButtonOld btnGraphics("btnGraphics", "0S", []()            // Доступ к графикам
    {
        PageGraph::self->SetAsCurrent();
    });

    static ButtonOld btnMAX("btnImax", "01I", []() {});           // Imax

    static ButtonOld btnAMP("btnIamp", "04I", []() {});           // Iamp

    static ButtonOld btnMIN("btnImin", "02I", []() {});           // Imin

    static ButtonOld btnPEAK("btnIpp", "03I", []() {});          // Ipp

    static Item *items[] =
    {
        &btn2mA,
        &btn20mA,
        &btn200mA,
        &btn2A,
        &btn20A,
        &btn50A,
        &btnAC_DC,
        &btnAC,
        &btnDC,
        &btnCalibration,
        &btnZeroDC_EN,
        &btnZeroDC_DIS,
        &btnZeroAC_EN,
        &btnZeroAC_DIS,
        &btnGraphics,
        &btnMAX,
        &btnAMP,
        &btnMIN,
        &btnPEAK,
        &btnSettings,
        &btnZero,
        nullptr
    };

    static Page pageMain(items, FuncOnEnter, FuncDraw);

    Page *self = &pageMain;

    void SetRange(MeasuresOnDisplay::E meas, int range)
    {
        MeasuresOnDisplay::Set(meas);

        if (MeasuresOnDisplay::IsAC_DC())
        {
            btnAC_DC.SetText("AC+DC");
        }
        else if (MeasuresOnDisplay::IsAC())
        {
            btnAC_DC.SetText("AC");
        }
        else if (MeasuresOnDisplay::IsDC())
        {
            btnAC_DC.SetText("DC");
        }

        wndAC.SetVisible(MeasuresOnDisplay::IsAC_DC() || MeasuresOnDisplay::IsAC());
        wndDC.SetVisible(MeasuresOnDisplay::IsAC_DC() || MeasuresOnDisplay::IsDC());

        for (int i = 0; i < 6; i++)
        {
            PageMain::self->GetItem(i)->ToButtonRange()->SetValue(i == range ? 1 : 0);
        }

        PageMain::self->GetItem(range)->ToButtonRange()->Press();
    }


    void HightLightCurrentRange()
    {
        PageMain::self->GetItem(Range::Current())->ToButtonRange()->SetValue(1);
    }
}


void PageMain::EnableZero(MeasuresOnDisplay::E meas, bool enable)
{
    if (meas == MeasuresOnDisplay::AC)
    {
        btnZeroAC_EN.SetValue(enable ? 1 : 0);

        enable ? btnZeroAC_EN.Press() : btnZeroAC_DIS.Press();

        PageMain::labelZeroAC.SetShown(enable);
    }
    else if (meas == MeasuresOnDisplay::DC)
    {
        btnZeroDC_EN.SetValue(enable ? 1 : 0);

        enable ? btnZeroDC_EN.Press() : btnZeroDC_DIS.Press();

        PageMain::labelZeroDC.SetShown(enable);
    }
}


void PageMain::Init()
{
    btn2A.SetValue(1);
}
