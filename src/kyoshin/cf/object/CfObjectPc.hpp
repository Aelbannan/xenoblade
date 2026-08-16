#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectActor.hpp"

extern UNKTYPE* func_800B708C(BOOL r3);
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
// func_800C0174 starts at +0x1C (passed to func_8009D7E4).
struct CfObjectPcArtsData {
    u8 _0[0xC];
    u16 field_0xC;    // +0xC: u16 index read by func_800C0474
    u8 _E[0x1C - 0xE];
    u8 mEntries;      // +0x1C: start of the entry table
    u8 _1D[0x17C - 0x1D];
    u8 field_0x17C;   // +0x17C: write target of func_80175A50 (func_800BFFEC)
};

// Return layout of func_8009D7E4 (indexed entry within the arts data object):
// the u16 flag at +0x1A is the "occupied" mark checked by func_800C0174.
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
extern "C" void func_80174B4C(void* obj, u32 flag);       // battle-manager flag setter
extern "C" void func_800A18A4(u8* obj, int value);      // CtrlObjectParam.cpp
extern "C" UnkStruct_8009D7E4_Ret* func_8009D7E4(u8* obj, int index);  // arts data lookup
extern "C" void func_8014B7B0(u8* obj);                  // CAIAction.cpp
extern "C" void func_8015396C(u8* obj, u32 a, u32 b);    // CAIAction.cpp
extern "C" void func_80175A50(u8* obj, u32 value);       // CActorParam.cpp
extern "C" void* getInstance__Q22cf14CBattleManagerFv(void);  // cf::CBattleManager singleton
extern "C" void CActorParam_UnkVirtualFunc176__Q22cf11CActorParamFv(
    cf::CActorParam* self, float value);   // CActorParam virtual 0x354 (Fv name, float in f1)

// sdata2 literal referenced by func_800C0080.
extern float lbl_eu_80666B14;

// Layout of the CfObjectPc region below the CfObjectActor bases (inside the
// CfObjectMove sub-object at +0x3E9C); read by func_800C11CC and the
// CActorParam_UnkVirtualFunc166/167 / func_800C02C4 family.
struct CfObjectPcSubFields {
    u8 _0[0x3ED4];
    u8* mPtr3ED4;      // 0x3ED4: sub-object pointer (CfObjectMove+0x38)
    u8 _3ED8[0x50];    // 0x3ED8-0x3F27
    u16 field_0x3F28;  // 0x3F28: data index (CfObjectModel+0x8C)
    u8 _3F2A[0x22];    // 0x3F2A-0x3F4B
    u8* mPtr3F4C;      // 0x3F4C: CfResPcImpl resource object pointer (ctor)
    u8 _3F50[0x4568 - 0x3F50];  // 0x3F50-0x4567
    u16 field_0x4568;  // 0x4568: enabled flag (set by func_800C0080)
    u8 _456A[0x56];    // 0x456A-0x45BF
    float field_0x45C0; // 0x45C0: float field (set by func_800C0080)
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
    // MWCC_REFERENCE "Retail-owned vtable data" / COccCulling).
    class __declspec(novtable) CfObjectPc : public CfObjectActor {
    public:
        virtual ~CfObjectPc();
        virtual void func_800BFF20(); //0x5D4
        virtual void func_800C0080(); //0x5D8
        virtual void func_800C0524(); //0x5DC
        virtual void func_800BFFEC(); //0x5E0
        virtual void func_800C00C0(); //0x5E4
        virtual void func_800C0174(); //0x5E8
        virtual void func_800C02C4(); //0x5EC
        virtual void func_800C02EC(); //0x5F0
        virtual void func_800C032C(); //0x5F4
        virtual void func_800C03A8(); //0x5F8
        virtual void func_800C0474(); //0x5FC
        virtual void func_800C0504(); //0x600
        virtual void func_800C0514(); //0x604
        virtual void func_800C11CC(); //0x608

        inline UNKTYPE* unkInline1(){
            BOOL thing = CObjectParam_UnkVirtualFunc5();
            UNKTYPE* idk = func_800B708C(thing);
            return func_800AD860(idk);
        }

        inline UNKTYPE* unkInline2(){
            BOOL thing2 = CObjectParam_UnkVirtualFunc5();
            UNKTYPE* idk2 = func_800B708C(thing2);
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
    void func_800C1220();
    };
}
