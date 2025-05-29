#pragma once

#define CE_VERION "0.1.1 - 29.05.2025"

#define CE_API __declspec(dllexport)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

#define CE_VERIFY(exp)                                                                             \
    if (!(exp))                                                                                    \
    return

#ifdef CE_WITH_EDITOR
#define CE_ED_API __declspec(dllexport)
#else
#define CE_ED_API
#endif
