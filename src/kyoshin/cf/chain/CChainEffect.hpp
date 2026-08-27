#pragma once

#include <types.h>
#include "kyoshin/cf/object/IObjectInfo.hpp"

namespace cf {
    //size: 0xC
    class CChainEffect : public IObjectInfo {
    public:
        CChainEffect();
        virtual ~CChainEffect();
        virtual void IObjectInfo_UnkVirtualFunc1();

        //0x0: vtable
        //0x0-4: IObjectInfo
        u32 unk4;
        u32 unk8;
    };
}

extern "C" void func_802A0950(cf::CChainEffect*, int a, int b, int c, int d, int e);

// ---------------------------------------------------------------------------
// Effect linkage / registry shapes (global scope members of this TU).
// ---------------------------------------------------------------------------

// Effect linkage object returned by func_800451D8 (owns the caller effect).
struct CChainObj {
    u8 pad_00[0x98];
    u32 field_98;                 //0x98
    u8 pad_9C[0x14];              //0x9C
    cf::CChainEffect* field_b0;   //0xB0
};

// Singleton chain manager returned by func_800B6C34.
struct CChainManager {
    u32 field_00;                        //0x0
    struct CChainNode* field_04;         //0x4: head of circular node list
};

// Node in the manager's circular list.
struct CChainNode {
    struct CChainNode* field_00;         //0x0: next
    u32 field_04;                        //0x4
    void* field_08;                      //0x8: object pointer
};

// Object produced by func_800AC610 (examined in func_802A0818).
struct CChainItem {
    u8 pad_00[0x8C];
    u16 field_8C;                 //0x8C
    u8 pad_8E[0xE];               //0x8E
    u32 field_9C;                 //0x9C
};

// Cross-TU C-ABI imports. These retail symbols are emitted unmangled, so they
// are declared with C linkage (matches the C-linkage import blocks used across
// the codebase). getInstance is a plain global C++ function whose retail
// symbol already carries the MWCC `__Fv` mangle, so it is left as-is.
extern "C" {
void* func_800451D8(u32 cls, int param);
void setTargetObj_(void* obj, void* target);
void setChildB59__(void* obj, s8 val);
void func_800B3A88(void* self, void* target);
struct CChainManager* func_800B6C34(void);
void* func_800AC610(void* param);
extern "C" u16 playActorSound__Q22cf10CfSoundManFUlUlUlUlf(u32 a, u32 b, u32 c, u32 d, f32 e);
extern f32 lbl_eu_80668C50;
}
// Plain global C++ callee (retail symbol getInstance__Fv).
void* getInstance(void);