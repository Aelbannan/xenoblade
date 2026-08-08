#pragma once

#include <types.h>
#include "kyoshin/cf/chain/UnkClass_8027AD70.hpp"
#include "monolib/util.hpp"

namespace cf {
    //size: 0x1DB0
    class CChainActorList {
    public:
        UnkClass_8027AD70 unk0; //0x0
        u8 unk1D80[8];
        reslist<CChainActor*> mChainActorList; //0x1D88
        u8 unk1DA8[4];
        //0x1DAC: vtable

        CChainActorList();
        virtual ~CChainActorList();
    };

    // size 0xE8: fixed array of CChainActor* with an element count
    // (field at 0xE0) and a byte flag (field at 0xE4).
    class CChainList {
    public:
        CChainActor* mActors[0x38]; //0x0
        u32 mCount;                 //0xE0
        u8 mFlag;                   //0xE4
    };

    // Minimal object manipulated by func_8027C154 (u16 field at 0x0).
    class CChainMsg {
    public:
        u16 field_0x0; //0x0
    };

    // Minimal object manipulated by func_8027CBE8 (s32 field at 0x0).
    class CChainCounter {
    public:
        s32 field_0x0; //0x0
    };

    // Large object manipulated by func_8027C040 (u32 flag at 0x3F00).
    class CChainFlag {
    public:
        u8 field_0[0x3F00]; //0x0
        u32 field_0x3F00;   //0x3F00
    };

    // 3x8 float table mirroring arts-param tuning values plus an enable flag
    // (manipulated by func_8027CC3C / func_8027CD08).
    class CChainMusic {
    public:
        float mSlots[3][8]; //0x0
        u8 mEnabled;        //0x60
    };

    // State object manipulated by func_8027C33C (s16 at 0x0, u8 at 0xC).
    class CChainAction {
    public:
        s16 field_0;  //0x0
        u8 unk2[10];  //0x2
        u8 field_0xc; //0xc
    };
}

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" void func_802A07F4(int, void*);
extern "C" u32 func_8013C54C();
extern "C" void func_8013E800(int);
extern "C" int lbl_eu_80662A80;
extern "C" int func_800B8920(void* addr);
extern "C" int func_80148778(void*, int);
extern "C" int func_80174C98(void*, int*, int);
extern "C" void func_80082568__Q22cf13CfGameManagerFv(int, int, int);
extern "C" int func_8017FD44(void*);
extern "C" int func_8017FD4C(int);
extern "C" int getArtsSlotRC(const void* arts, short index, short subindex);
extern "C" void* getArtsParamRC2(const void* arts, int index, int subindex);
extern "C" void func_8027EEF4(int);
extern "C" u32 func_8027EE88(int, int);
