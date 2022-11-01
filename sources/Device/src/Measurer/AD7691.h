// 2022/10/18 16:57:46 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


// АЦП
namespace AD7691
{
    void Init();

    int ReadValue();

    // При вкл/откл генератора нужно вызывать эту функцию
    void GeneratorChangedEvent();
}
