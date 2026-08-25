#pragma once

#include <types.h>
#include "kyoshin/cf/chain/CChainActor.hpp"
#include "monolib/util.hpp"

namespace cf {
    // Standalone mirror of the chain-actor layout (0x80 bytes, manual vtable at
    // +0x70, non-standard ABI). Deliberately NOT derived from CChainActor:
    // retail Pc/Ene dtors are plain empty+delete with no base/member dtor
    // calls, which requires these classes to have no non-trivial base or
    // dtor-ful members.
    class CChainActorPc {
    public:
        u32 unk0;          //0x0
        u8 field_4[0x60];  //0x4 CChainTemp-ish buffer
        u8 field_64;       //0x64
        u8 _pad65[3];
        u32 field_68;      //0x68 sub-object vtable
        u16 unk6C;         //0x6C
        u8 _pad6E[2];
        u32 mVTable;       //0x70 manual vtable
        u8 field_74[0xC];  //0x74 CChainEffect region (no dtor)

        CChainActorPc();
        ~CChainActorPc();
    };

    class CChainActorEne {
    public:
        u32 unk0;          //0x0
        u8 field_4[0x60];  //0x4 CChainTemp-ish buffer
        u8 field_64;       //0x64
        u8 _pad65[3];
        u32 field_68;      //0x68 sub-object vtable
        u16 unk6C;         //0x6C
        u8 _pad6E[2];
        u32 mVTable;       //0x70 manual vtable
        u8 field_74[0xC];  //0x74 CChainEffect region (no dtor)

        CChainActorEne();
        ~CChainActorEne();
    };

    //size: 0x1d80
    class UnkClass_8027AD70 {
    public:
        CChainActorPc mPlayerActors[3]; //0x0
        CChainActorEne mEnemyActors[56]; //0x180

        UnkClass_8027AD70(){}
        ~UnkClass_8027AD70(){}
    };

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

    // Mirror of CChainChance with signed 16-bit fields: func_8027C0B0 reads the
    // 0x0/0x8/0xA fields sign-extended (lha), while CChainChance.hpp declares
    // them u16. Layout-twin (same offsets/sizes) so the free function's param
    // can force signed loads without touching CChainChance.hpp.
    class CChainChanceS {
    public:
        s16 mChainCount; //0x0
        u8 unk2[2];      //0x2
        u32 unk4;        //0x4
        s16 mField08;    //0x8
        s16 mField0A;    //0xA
        u8 mField0C;     //0xC
        u8 _pad0D[3];    //0xD
        //0x10: vtable
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
extern "C" float lbl_eu_8050EDE0[4];
extern "C" int func_800B8920(void* addr);
extern "C" int func_80148778(void*, int);
extern "C" void func_80082568__Q22cf13CfGameManagerFv(int, int, int);
extern "C" int func_8017FD44(void*);
extern "C" int func_8017FD4C(int);
extern "C" int func_80086F9C__Q22cf13CfGameManagerFv(int arg);
extern "C" int getArtsSlotRC(const void* arts, short index, short subindex);
extern "C" void* getArtsParamRC2(const void* arts, int index, int subindex);
extern "C" void func_8027EEF4(int);
extern "C" u32 func_8027EE88(int, int);
extern "C" __declspec(noinline) void func_802811FC(cf::CChainActorList* self);
extern "C" cf::CChainActor* func_8028120C(cf::CChainActorList* self);
extern "C" void func_8027B8C8(cf::CChainActorList* self, cf::CChainActor* actor);
extern "C" u32 func_8004C5EC(void* battleObj);

// Minimal mirror of the object referenced by a chain actor's unk0 (probed by
// func_8027BE84 / func_8027CAE0): vtable at +0x0, move sub-object pointer at
// +0x4, probed address at +0x8.
class CChainTargetObj {
public:
    u32 field_0;  //0x0 vtable
    u32 field_4;  //0x4 move sub-object pointer
    u32 field_8;  //0x8
};

// Tail view of the battle object at a chain actor's unk0: the u16 arts/battle
// id at +0x3F28 (func_8027C924 accumulates the chain arts pair from these)
// and the battle-object pointer at +0x3F60 queried by func_8004C5EC.
class CChainActorObjId {
public:
    u8 field_0[0x3F28];  //0x0
    u16 field_0x3F28;    //0x3F28
    u8 field_0x3F2A[0x3F60 - 0x3F2A];
    u32 field_0x3F60;    //0x3F60
};

// Tail view used by func_8027BC14: an embedded sub-object with a manual
// vtable sits at +0x3E9C (its vtable slot 19 returns the move sub-object
// pointer), and +0x3F60 holds a battle-object pointer queried by
// func_8004C5EC.
class CChainBattleObjTail {
public:
    u8 field_0[0x3E9C];  //0x0
    u32 field_0x3E9C;    //0x3E9C embedded sub-object (vtable at its +0)
    u8 field_0x3EA0[0x3F60 - 0x3EA0];
    u32 field_0x3F60;    //0x3F60
};

// Interface for the move sub-object's vtable: declared virtual #10 lands at
// vtable offset +0x30 under -RTTI (retail func_8027BE84 dispatches there and
// dereferences the returned pointer). Casting the sub-object and calling this
// virtual makes MWCC emit the r12 dispatch, like CChainActorVtIf. Slot 19
// (+0x4c) is the move-sub-object getter used by func_8027BC14.
class CChainSubVtIf {
public:
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual void v008() = 0;
    virtual void v009() = 0;
    virtual int* v010() = 0;  // slot 12 / +0x30
    virtual void v011() = 0;
    virtual void v012() = 0;
    virtual void v013() = 0;
    virtual void v014() = 0;
    virtual void v015() = 0;
    virtual void v016() = 0;
    virtual int  v017() = 0;  // slot 19 / +0x4c
};

// Manual vtable objects stored by the Pc/Ene constructors (retail .data).
// Declared at their full retail sizes (0x78 / 0x10) so MWCC emits the
// lis/addi HA-LO address form; a small (<8B) extern type would otherwise
// use sda21 addressing and break the reloc match (MWCC_CASES §833).
struct CChainVtblActor {
    u8 bytes[0x78];
};
struct CChainVtblSub {
    u8 bytes[0x10];
};
extern CChainVtblActor lbl_eu_80538290;
extern CChainVtblActor lbl_eu_805384E0;
extern CChainVtblActor lbl_eu_80538458;
extern CChainVtblSub lbl_eu_80538338;

// Explicit call target for the CChainEffect ctor: retail calls it at the END
// of the CChainActorPc/Ene ctor bodies, and MWCC rejects explicit ctor-call
// syntax (10409), so the call is made through this free-function alias. The
// extern "C" keeps the reloc name unmangled (hexdiff reloc-drift fix).
extern "C" void __ct__Q22cf12CChainEffectFv(cf::CChainEffect* p);
