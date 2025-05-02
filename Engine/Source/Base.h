#pragma once

#define CE_VERION "0.0.3 - 02.05.2025"

#define CE_API __declspec(dllexport)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define CE_VERIFY(exp)                                                                             \
    if (!(exp))                                                                                    \
        return;
