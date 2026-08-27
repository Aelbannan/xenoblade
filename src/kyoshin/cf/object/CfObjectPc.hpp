#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

extern UNKTYPE* findObjectById(BOOL r3);
extern UNKTYPE* func_800AD860(UNKTYPE* r3);
extern UNKTYPE* func_800C1228(UNKTYPE* r3);

// Retail data labels referenced by the CfObjectPc methods.
extern void* lbl_eu_806640DC;   // .sbss bdat file pointer (arts list)
extern void* lbl_eu_806640F4;   // .sbss bdat file pointer (arts list)
extern u8 lbl_eu_804FC5EC[];    // .rodata column-name string table
extern u8 lbl_eu_80529DA0[];    // retail CfObjectPc vtable data (.data)

// Old-style retail ctor symbol of the CfResPcImpl resource object
// (CfResPcImpl.cpp). A real C++ member ctor would mangle to
// __ct__Q22cf11CfResPcImplF..., so the C-linkage form keeps the retail name;
// declared returning the object pointer (MWCC ctor ABI keeps this in r3).
extern "C" u8* __ct__cf_CfResPcImpl(u8* obj, cf::CfObjectMove* parent);
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);


// Arts data object returned by func_8009EC9C: the per-entry table scanned by
// scanArtsEntries starts at +0x1C (passed to func_8009D7E4).
struct CfObjectPcArtsData {
    u8 _0[0xC];
    u16 field_0xC;    // +0xC: u16 index read by setupActionTable
    u8 _E[0x1C - 0xE];
    u8 mEntries;      // +0x1C: start of the entry table
    u8 _1D[0x17C - 0x1D];
    u8 field_0x17C;   // +0x17C: write target of func_80175A50 (syncArtsEntry)
};

// Return layout of func_8009D7E4 (indexed entry within the arts data object):
// the u16 flag at +0x1A is the "occupied" mark checked by scanArtsEntries.
struct UnkStruct_8009D7E4_Ret {
    u8 _0[0x1A];
    u16 field_0x1A;
};

// C-ABI imports from other kyoshin units: retail links them under their
// unmangled names, so the declarations need C linkage to emit the exact
// reloc symbols (func_8009EC9C is declared in kyoshin/CArtsInfo.hpp).
extern "C" void __ct__8009F8B8(u8* obj);   // CtrlObjectParam.cpp (arts/param ctor)
extern "C" void func_800A03F4(u8* obj);    // CtrlObjectParam.cpp
extern "C" void func_800A145C(u8* obj);    // CtrlObjectParam.cpp
extern "C" void func_800CA42C(u8* obj);    // CfObjectImplPc.cpp
extern "C" void func_800A18A4(u8* obj, int value);      // CtrlObjectParam.cpp
extern "C" UnkStruct_8009D7E4_Ret* func_8009D7E4(u8* obj, int index);  // arts data lookup
extern "C" void func_8014B7B0(u8* obj);                  // CAIAction.cpp
extern "C" void func_8015396C(u8* obj, u32 a, u32 b);    // CAIAction.cpp

extern "C" void func_8009EF9C(void* self, u32 arg2);     // CtrlObjectParam.cpp
extern "C" int CfObject_UnkVirtualFunc2__Q22cf13CfObjectModelFv(cf::CfObjectModel* self);
extern "C" void func_800BE33C(void* obj, int flag);      // CfObjectMove.cpp
extern "C" void func_800BE824(void* obj, int flag);      // CfObjectMove.cpp
extern "C" void func_804B0AD4(void* region, int param, float a, float b);
extern "C" void __dl__FPv(void* object);
extern "C" void __dt__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self, int flag);
extern "C" void __ct__8009ED08(void* destination, u32 itemId);   // CtrlObjectParam.cpp
extern "C" unsigned long func_80061FFC();   // bdat heap handle getter
// NOTE: retail keeps the virtual-call result in r3 across the second arg's
// address computation, so the value is the FIRST declared parameter.
// NOTE: the real definition is func_80175A50(CActorParam* dst, CActorParam*
// src) - the write target comes FIRST.
extern "C" void func_80175A50(u8* obj, u32 value);       // CActorParam.cpp
extern "C" void func_8014B804(u8* self, int index, int a2, int a3, int a4,
    int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
// getInstance__Q22cf14CBattleManagerFv is declared by kyoshin/cf/CBattleManagerApi.hpp
// (pulled in via CAIAction.hpp).
extern "C" void CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv(
    cf::CActorParam* self, float value);   // CActorParam virtual 0x354 (Fv name, float in f1)
extern "C" void func_800A11A4(u8* obj, int flag);         // CtrlObjectParam.cpp
extern "C" void* func_8009EC9C(u32 index);   // CtrlObjectParam.cpp (character-data lookup; owner canonical u32 form)
extern "C" void func_800A13C4(u8* obj, int flag);         // CtrlObjectParam.cpp
extern "C" void func_8018CBE8(u8* obj);                   // CfResPcImpl.cpp
extern "C" void func_8012FAA8();                          // camera/screen reset
extern "C" void func_801765A4(void* actor, f32 value, u32 arg); // CActorParam.cpp
extern "C" int func_8013EB90(int flag);                   // CfObjectImplWalker.cpp
extern float lbl_eu_80666B24;   // .sdata2 gauge constants (UnkVirtualFunc4)
extern float lbl_eu_80666B28;
extern double lbl_eu_80666B30; // 4503601774854144.0 (2^52+2^51): doubles as the int->double biased-magic constant
extern "C" void func_8010CE50(u8* obj, int a, int b, int c);
extern "C" void func_802617B8(u8* obj, u32 value, int a);
extern "C" void func_800A282C(u8* obj, int flag);
extern "C" int func_800A2AF0(u8* obj);
extern "C" void func_802761E0(u8* obj);
extern "C" void func_80276148(u8* obj, u32 value);
extern "C" void CfObject_UnkVirtualFunc4__Q22cf12CfObjectMoveFv(cf::CfObjectMove* self);
// .rodata literal copied onto the stack by UnkVirtualFunc4 (struct defined
// below); bdat file ptr.
extern void* lbl_eu_80664090;

// sdata2 literals referenced by the CfObjectPc methods.
extern float lbl_eu_80666B14;
extern float lbl_eu_80666B0C;
extern float lbl_eu_80666B10;
extern float lbl_eu_80666B18;
extern float lbl_eu_80666B1C;
extern float lbl_eu_80666B20;

// 10-byte .rodata literal copied onto the stack by UnkVirtualFunc4.
struct Lit10 {
    u8 d[10];
};
extern Lit10 lbl_eu_804FC5E0;

// Pointee of the +0x3F60 resource pointer (+0x4B0 byte / flag word at
// +0x4EC).
struct PcResFlagsObj {
    u8 _0[0x4B0];
    u32 field_0x4B0;
    u8 _4B4[0x38];
    u32 field_0x4EC;
};

// Object returned by func_80149154 (holds a word at +0x10).
struct Res10View {
    u8 _0[0x10];
    u32 field_0x10;
};

// Imports used by func_800C0DD4.
extern u16 lbl_eu_80663E42;
extern u16 lbl_eu_80663E44;
extern void* lbl_eu_806640D4;
extern float lbl_eu_80666B38;
extern double lbl_eu_80666B40;
extern "C" {
void* func_8003AA34();                         // bdat manager refresh (void* return - must match CArtsInfo.hpp:679 / the other ~40 decls; divergent extern "C" types = 10197)
u32 func_8003B41C(void* mgr);                  // first bdat row index (canonical form per ocBdat.cpp def)
u32 func_8003B1EC(void* mgr);                  // bdat row count (canonical form per ocBdat.cpp def)
unsigned long func_801BA2C8(void* obj);        // battle-manager state probe (unified with CChainTimer.hpp)
}
// cf::CfSoundMan lives on kyoshin/cf/CfSoundMan.hpp (single definition;
// include it where playActorSound is needed).

// Flags object reached through +0x3F34 (word at +0x7A4).
struct FlagsObj7A4 {
    u8 _0[0x7A4];
    u32 field_0x7A4;
};

// Object returned by primary-vtable slot 0x290 (holds a word at +0x89C).
struct Obj89cField {
    u8 _0[0x89C];
    u32 field_0x89C;
};

// Interface over the sub-object at this+0x4 (slot 0x30 used by
// func_800C0524 / CActorParam_UnkVirtualFunc88).
class PcSub4VtIf {
public:
    virtual void _q008(); virtual void _q00C(); virtual void _q010();
    virtual void _q014(); virtual void _q018(); virtual void _q01C();
    virtual void _q020(); virtual void _q024(); virtual void _q028();
    virtual void _q02C();
    virtual void* _q030();   // +0x30 returns a state pointer
};
struct PcSub4Pad { u8 _pad[0x4]; };
struct PcSub4Fake : PcSub4Pad, PcSub4VtIf {};
// Distinct type, identical layout: alternating spellings keeps MWCC from
// CSE-ing the sub-object address into one temp (retail reloads it).
struct PcSub4PadB { u8 _pad[0x4]; };
struct PcSub4FakeB : PcSub4PadB, PcSub4VtIf {};
struct PcSub4PadC { u8 _pad[0x4]; };
struct PcSub4FakeC : PcSub4PadC, PcSub4VtIf {};

// Interface over the sub-object at this+0x8 (slots 0x14/0x20 used by
// func_800C0524).
class PcSub8VtIf {
public:
    virtual void _s008(); virtual void _s00C(); virtual void _s010();
    virtual void _s014(int id);       // +0x14
    virtual void _s018(); virtual void _s01C();
    virtual void _s020(int id);       // +0x20
    virtual void _s024(); virtual void _s028(); virtual void _s02C();
    virtual void _s030();
};
struct PcSub8Pad { u8 _pad[0x8]; };
struct PcSub8Fake : PcSub8Pad, PcSub8VtIf {};
struct PcSub8PadB { u8 _pad[0x8]; };
struct PcSub8FakeB : PcSub8PadB, PcSub8VtIf {};

// Linked-list node/head view of the battle-manager list at +0x48.
struct BmListNode {
    BmListNode* mNext;
};
struct BattleMgrListView {
    u8 _0[0x48];
    BmListNode mListHead;
};

// Range/state bytes of the battle manager checked by func_800C0524 /
// CActorParam_UnkVirtualFunc88.
struct BattleMgrRangeView {
    u8 _0[0x1AA];
    u8 field_0x1AA;
    u8 _1AB[0x20C8 - 0x1AB];
    s16 field_0x20C8;
};

// Layout of the CfObjectPc region below the CfObjectActor bases (inside the
// CfObjectMove sub-object at +0x3E9C); read by handleMoveState and the
// CActorParam_UnkVirtualFunc166/167 / resetArtsState family.
struct CfObjectPcSubFields {
    u8 _0[0x15E0];
    u32 field_0x15E0;  // 0x15E0: sub-object pointer word (set by func_800C00C0)
    u8 _15E4[0x1C];    // 0x15E4-0x15FF
    u32 field_0x1600;  // 0x1600: accumulated currency (clamped 99,999,999)
    u32 field_0x1604;  // 0x1604: spent counter (negated store)
    u32 field_0x1608;  // 0x1608: second accumulated counter (word-accessed)
    u8 _160C[0x1D];    // 0x160C-0x1628
    u8 field_0x1629;   // 0x1629-0x162C: bdat string bytes (func_800BFDE0)
    u8 field_0x162A;
    u8 field_0x162B;
    u8 field_0x162C;
    u8 _162D[0x1824 - 0x162D];
    float field_0x1824; // 0x1824: gauge scale factor (UnkVirtualFunc4)
    u8 _1828[0x3ED4 - 0x1828];
    u8* mPtr3ED4;      // 0x3ED4: sub-object pointer (CfObjectMove+0x38)
    u8 _3ED8[0x28];    // 0x3ED8-0x3EFF
    u32 field_0x3F00;  // 0x3F00: gate flag (bit 0 checked by initialize)
    u8 _3F04[4];
    u32 field_0x3F08;  // 0x3F08: flag word OR'd with 1 by initialize
    u8 _3F0C[4];       // 0x3F0C-0x3F0F
    u8* field_0x3F10;  // 0x3F10: effect target pointer
    u8 _3F14[0x14];    // 0x3F14-0x3F27
    u16 field_0x3F28;  // 0x3F28: data index (CfObjectModel+0x8C)
    u8 _3F2A[0xA];     // 0x3F2A-0x3F33
    FlagsObj7A4* field_0x3F34;  // 0x3F34: flags object (word at +0x7A4)
    u8 _3F38[0x14];    // 0x3F38-0x3F4B
    u8* mPtr3F4C;      // 0x3F4C: CfResPcImpl resource object pointer (ctor)
    u8 _3F50[0x10];    // 0x3F50-0x3F5F
    PcResFlagsObj* mPtr3F60; // 0x3F60: resource flags object (+0x4EC word)
    u8 _3F64[0x4568 - 0x3F64];
    u16 field_0x4568;  // 0x4568: enabled flag (set by enablePcFlag)
    u8 _456A[0x56];    // 0x456A-0x45BF
    float field_0x45C0; // 0x45C0: float field (set by enablePcFlag)
};

// Absolute-offset vtable-pointer slots rewritten by the CfObjectPc
// dtor (stores lbl_eu_80529DA0 + 0x0/0xC/0x36C/0x37C).
namespace cf {
struct CfPcVt {
    u32 vt0;             // +0x00 primary vtable
    u8 _pad4[4];
    u32 vt8;             // +0x08 secondary vtable (+0xC)
    u8 _padC[0x3380 - 0xC];
    u32 vt3380;          // +0x3380 CAIAction sub-vtable (+0x36C)
    u8 _pad3384[0x3E9C - 0x3384];
    u32 vt3E9C;          // +0x3E9C player sub-vtable (+0x37C)
};
// +0x3380 CAIAction subobject base / block views (same shapes as the
// CfObjectEne dtor's inlined cleanup).
struct CfPcCAISub {
    u8 _pad[4];
    u32 field_4;
};
}
struct CfPcCAIBlock {
    u8 _pad[8];
    void* field_8;
    u8 _padC[0x20C - 0xC];
    u32 field_20C;
    u32 field_210;
};

// Object examined by func_800C1228: flags word at +0x64; bit 0x4000 marks
// the object as valid (returned as-is), otherwise NULL is returned.
struct FlagsObj800C1228 {
    u8 _0[0x64];
    u32 field_0x64;
};

namespace cf {
    //size: 0x45C0
    // __declspec(novtable): the retail vtable data lives at lbl_eu_80529DA0;
    // the auto-generated __vt__Q22cf10CfObjectPc symbol would fail the reloc
    // name match, so the ctor stores the retail label explicitly (see
    // MWCC_CASES "Retail-owned vtable data" / COccCulling).
    class __declspec(novtable) CfObjectPc : public CfObjectActor {
    public:
        virtual ~CfObjectPc();
        __attribute__((never_inline)) virtual int initialize(); //0x5D4
        virtual void enablePcFlag(); //0x5D8
        virtual void func_800C0524(); //0x5DC
        __attribute__((never_inline)) virtual void syncArtsEntry(); //0x5E0
        virtual void func_800C00C0(); //0x5E4
        virtual void scanArtsEntries(); //0x5E8
        virtual void resetArtsState(); //0x5EC
        virtual void applyArtsParam(); //0x5F0
        virtual void dispatchPlayerBranch(); //0x5F4
        virtual void resetActionTable(); //0x5F8
        virtual void setupActionTable(); //0x5FC
        virtual void triggerActionRefreshA(); //0x600
        virtual void triggerActionRefreshB(); //0x604
        virtual void handleMoveState(); //0x608

        inline UNKTYPE* unkInline1(){
            BOOL thing = CObjectParam_UnkVirtualFunc5();
            UNKTYPE* idk = findObjectById(thing);
            return func_800AD860(idk);
        }

        inline UNKTYPE* unkInline2(){
            BOOL thing2 = CObjectParam_UnkVirtualFunc5();
            UNKTYPE* idk2 = findObjectById(thing2);
            return func_800C1228(idk2);
        }

        //0x0: vtable
        //0x0-45BC: CfObjectActor
        //0x0: vtable
        //0x0-45BC: CfObjectActor
        u8 unk45BC[0x4];
    CfObjectPc();
    void CActorParam_UnkVirtualFunc166();
    void CActorParam_UnkVirtualFunc167();
    void CActorParam_UnkVirtualFunc4();
    void CActorParam_UnkVirtualFunc176();
    int CActorParam_UnkVirtualFunc86();
    void CActorParam_UnkVirtualFunc88();
    void CActorParam_UnkVirtualFunc178();
    void CActorParam_UnkVirtualFunc173();
    void CObjectParam_UnkVirtualFunc4();
    void CfObject_UnkVirtualFunc3(UnkClass_80082D90* data);
    void CfObject_UnkVirtualFunc2();
    void CfObjectMove_UnkVirtualFunc16();
    void CfObject_UnkVirtualFunc6();
    void CfObject_UnkVirtualFunc4();
    void finalizePcCleanup();
    };
}
