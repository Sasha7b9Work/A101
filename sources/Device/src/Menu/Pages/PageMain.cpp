// 2022/10/24 12:17:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Menu/Pages/Pages.h"
#include "Ampermeter/InputRelays.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Nextion/Nextion.h"
#include "Display/Controls/WindowsMeasures.h"
#include "Ampermeter/Ampermeter.h"
#include "Display/Display.h"


namespace PageMain
{
    static WindowMeasure wndDC(TypeMeasure::DC, "titleDC", "f_sign", "valueDC", "unitsDC", "DC:");
    static WindowMeasure wndAC(TypeMeasure::AC, "titleAC", "",       "valueAC", "unitsAC", "AC:");

    static WindowMeasure wndAMPL(TypeMeasure::Ampl, "t23", "", "t10", "t27", "Iamp:");
    static WindowMeasure wndPEAK(TypeMeasure::Peak, "t20", "", "t9", "t26", "Ipp:");
    static WindowMeasure wndMIN(TypeMeasure::Min, "t22", "", "t8", "t25", "Imin:");
    static WindowMeasure wndMAX(TypeMeasure::Max, "t21", "", "t7", "t24", "Imax:");

    static void ResetAllMeasures()
    {
        wndDC.Reset();
        wndAC.Reset();
        wndAMPL.Reset();
        wndPEAK.Reset();
        wndMIN.Reset();
        wndMAX.Reset();
    }

    void PageMain::Init()
    {
        ResetAllMeasures();
    }

    void OnEventChangeRange()
    {
        ResetAllMeasures();
    }

    static void FuncOnEnter()
    {
        ResetAllMeasures();
    }

    static void FuncDraw()
    {
        Display::DrawLabelStar();

        wndDC.Draw(Ampermeter::GetDC(), Range::Current());

        wndAC.Draw(Ampermeter::GetAC(), Range::Current());

        wndAMPL.Draw(Ampermeter::GetAmpl(), Range::Current());
        wndPEAK.Draw(Ampermeter::GetPeak(), Range::Current());
        wndMIN.Draw(Ampermeter::GetMin(), Range::Current());
        wndMAX.Draw(Ampermeter::GetMax(), Range::Current());
    }


    static Button btn2mA("bt0", "01P", []()
        {
            Range::Set(0);
        });

    static Button btn20mA("bt1", "02P", []()
        {
            Range::Set(1);
        });

    static Button btn200mA("bt2", "03P", []()
        {
            Range::Set(2);
        });

    static Button btn2A("bt3", "04P", []()
        {
            Range::Set(3);
        });

    static Button btn20A("bt4", "05P", []()
        {
            Range::Set(4);
        });

    static Button btn50A("bt5", "06P", []()
        {
            Range::Set(5);
        });

    static Button btnAC_DC("b0", "0AD", []() {});

    static Button btnCalibration("bt13", "0C", []()
        {
            PageCalibration::self->SetAsCurrent();
        });

    static Button btnZeroDC_EN("bt11", "0DZ1", []()         // Включение режима "Zero DC"
        {
            Ampermeter::Set::ZeroDC::Enable();
        });

    static Button btnZeroDC_DIS("bt11", "0DZ0", []()        // Выключение режима "Zero DC"
        {
            Ampermeter::Set::ZeroDC::Disable();
        });

    static Button btnZeroAC_EN("bt10", "0AZ1", []()         // Включение режима "Zero AC"
        {
            Ampermeter::Set::ZeroAC::Enable();
        });

    static Button btnZeroAC_DIS("bt10", "0AZ0", []()        // Выключение режима "Zero AC"
        {
            Ampermeter::Set::ZeroAC::Disable();
        });

    static Button btnSignal("bt12", "0S", []()              // Signal
        {
            PageGraph::self->SetAsCurrent();
        });

    static Button btnMAX("bt15", "01I", []() {});           // Imax

    static Button btnAMP("bt19", "04I", []() {});           // Iamp

    static Button btnMIN("bt18", "02I", []() {});           // Imin

    static Button btnPEAK("bt17", "03I", []() {});          // Ipp

    static Button *buttons[] =
    {
        &btn2mA,
        &btn20mA,
        &btn200mA,
        &btn2A,
        &btn20A,
        &btn50A,
        &btnAC_DC,
        &btnCalibration,
        &btnZeroDC_EN,
        &btnZeroDC_DIS,
        &btnZeroAC_EN,
        &btnZeroAC_DIS,
        &btnSignal,
        &btnMAX,
        &btnAMP,
        &btnMIN,
        &btnPEAK,
        nullptr
    };

    static Page pageMain(buttons, FuncOnEnter, FuncDraw);

    Page *self = &pageMain;
}

