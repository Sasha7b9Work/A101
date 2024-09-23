// (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "opt.h"

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_BUILD 299

#define DATE_BUILD "2024-09-23 15:01:06"

#define VERSION_SCPI  "1.0"

#ifndef WIN32
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#pragma clang diagnostic ignored "-Wpadded"
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"
#pragma clang diagnostic ignored "-Wimplicit-int-float-conversion"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wcast-align"
#pragma clang diagnostic ignored "-Wnon-virtual-dtor"
#pragma clang diagnostic ignored "-Wundefined-func-template"
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"
#pragma clang diagnostic ignored "-Wdouble-promotion"
#pragma clang diagnostic ignored "-Wdelete-non-abstract-non-virtual-dtor"
#pragma clang diagnostic ignored "-Wweak-vtables"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#else
#define WIN32_LEAN_AND_MEAN
#endif

typedef unsigned int       uint;
typedef unsigned short     uint16;
typedef unsigned short     uint16_t;
typedef signed   short     int16;
typedef unsigned long long uint64;
typedef signed long long   int64;
typedef unsigned char      uint8;
typedef unsigned char      uint8_t;
typedef signed char        int8;
typedef const char * const cstr;
typedef const char *       pchar;

#ifndef WIN32
#pragma clang diagnostic pop
#endif

typedef void(*pFuncVV)();
typedef void(*pFuncVI)(int);

#define _GET_BIT(value, bit)    (((value) >> (bit)) & 0x01)
#define _SET_BIT(value, bit)    ((value) |= (1 << (bit)))
#define _CLEAR_BIT(value, bit)  ((value) &= (~(1 << (bit))))

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 0x000001) % 0x08) << 0) |  \
  (((uint8)((uint)bits / 0x000008) % 0x08) << 1) |  \
  (((uint8)((uint)bits / 0x000040) % 0x08) << 2) |  \
  (((uint8)((uint)bits / 0x000200) % 0x08) << 3) |  \
  (((uint8)((uint)bits / 0x001000) % 0x08) << 4) |  \
  (((uint8)((uint)bits / 0x008000) % 0x08) << 5) |  \
  (((uint8)((uint)bits / 0x040000) % 0x08) << 6) |  \
  (((uint8)((uint)bits / 0x200000) % 0x08) << 7)))

#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U16(bits1, bits0) ((BIN_U8(bits1) << 8) | (BIN_U8(bits0)))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)


struct Item;

inline void EmptyFuncVIemB(Item *, bool)
{
}


union BitSet32
{
    BitSet32(uint *_word) : word(*_word) { }
    BitSet32(uint _word = 0) : word(_word) { }
    BitSet32(uint8 byte0, uint8 byte1, uint8 byte2, uint8 byte3)
    {
        bytes[0] = byte0;
        bytes[1] = byte1;
        bytes[2] = byte2;
        bytes[3] = byte3;
    }
    uint   word;
    uint16 half_word[2];
    uint8  bytes[4];
};


#ifdef WIN32

#define __attribute__(x)

#else

#ifndef NULL
#define NULL 0
#endif
//#define nullptr NULL   //-V1059

#endif


#define MAX_UINT ((uint)-1)

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"


#include "Utils/Log.h"
#include "Utils/Debug.h"
