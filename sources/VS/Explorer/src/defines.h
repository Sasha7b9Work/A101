// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#endif

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint;
typedef const char *pchar;


#define BUTTON_SIZE wxSize(70, 30)

#pragma warning(push, 0)
#include <wx/spinctrl.h>
#pragma warning(pop)
