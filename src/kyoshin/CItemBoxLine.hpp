#pragma once

#include <types.h>

class CIBLTabCur {
public:
    CIBLTabCur();

    // TODO: add fields
};

class CItemBoxLine {
public:
    CItemBoxLine();
    virtual ~CItemBoxLine();
    void OnFileEvent();

    void func_801EDA4C(unsigned char val);
    void func_801EECC8();

    // Fields — populated from standalone accessor functions in CItemBoxLine.cpp
    u8 pad_00[0x59];
    u8 unk59;                       // +0x59: read by func_801ED800
    u8 pad_5A[0x332];               // 0x5A..0x38B
    s16 unk38C;                     // +0x38C: read by func_801EECC8
    s16 unk38E;                     // +0x38E: read by func_801EECC8
    u8 pad_390[0x0E];               // 0x390..0x39D
    u8 unk39E;                      // +0x39E: read by func_801EECC0
    u8 pad_39F[0x05];               // 0x39F..0x3A3
    u32 unk3A4;                     // +0x3A4: target of func_801EC3B0 call
};

