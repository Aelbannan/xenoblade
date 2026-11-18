#pragma once

#include <types.h>
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/plugin/ocBdat.hpp"
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

extern UNKTYPE* findObjectById(BOOL r3);
extern UNKTYPE* getEffOwner(UNKTYPE* r3);
extern UNKTYPE* getValidObject(UNKTYPE* r3);

// Retail data labels referenced by the CfObjectPc methods.
extern void* lbl_eu_806640DC;   // .sbss bdat file pointer (arts list)
extern void* lbl_eu_806640F4;   // .sbss bdat file pointer (arts list)
extern const char lbl_eu_804FC5EC[];    // .rodata column-name string table

// Old-style retail ctor symbol of the CfResPcImpl resource object
// (CfResPcImpl.cpp). A real C++ member ctor would mangle to
// __ct__Q22cf11CfResPcImplF..., so the C-linkage form keeps the retail name;
// declared returning the object pointer (MWCC ctor ABI keeps this in r3).
extern "C" u8* __ct__cf_CfResPcImpl(u8* obj, cf::CfObjectMove* parent);
extern "C" void* allocate__Q23mtl10MemManagerFUlUl(u32 size, u32 heap);


// Arts data object returned by CtrlObjectParam_GetCharEntry: the per-entry table scanned by
// scanArtsEntries starts at +0x1C (passed to CtrlObjectParam_GetArtsStatsRow).
struct CfObjectPcArtsData {
    u8 _0[0xC];
    u16 field_0xC;    // +0xC: u16 index read by setupActionTable
    u8 _E[0x1C - 0xE];
    u8 mEntries;      // +0x1C: start of the entry table
    u8 _1D[0x17C - 0x1D];
    u8 field_0x17C;   // +0x17C: write target of CActorParam_copyAll (syncArtsEntry)
};

// Return layout of CtrlObjectParam_GetArtsStatsRow (indexed entry within the arts data object):
// the u16 flag at +0x1A is the "occupied" mark checked by scanArtsEntries.
struct UnkStruct_8009D7E4_Ret {
    u8 _0[0x1A];
    u16 field_0x1A;
};

// C-ABI imports from other kyoshin units: retail links them under their
// unmangled names, so the declarations need C linkage to emit the exact
// reloc symbols (CtrlObjectParam_GetCharEntry is declared in kyoshin/CArtsInfo.hpp).
extern "C" void __ct__8009F8B8(u8* obj);   // CtrlObjectParam.cpp (arts/param ctor)
extern "C" void CtrlObjectParam_InitArtsSlotSet(u8* obj);    // CtrlObjectParam.cpp
extern "C" int CtrlObjectParam_LearnArtsSlots(u8* obj);    // CtrlObjectParam.cpp
extern "C" void ObjPc_ForwardVf0C_A42C(u8* obj);    // CfObjectImplPc.cpp
extern "C" void CtrlObjectParam_ClaimArtsSlotRow(u8* obj, int value);      // CtrlObjectParam.cpp
extern "C" UnkStruct_8009D7E4_Ret* CtrlObjectParam_GetArtsStatsRow(u8* obj, int index);  // arts data lookup
extern "C" void aiResetTables(u8* obj);                  // CAIAction.cpp
extern "C" void aiInstallArts(u8* obj, u32 a, u32 b);    // CAIAction.cpp

extern "C" void CtrlObjectParam_RefreshCharEntry(void* self, u32 arg2);     // CtrlObjectParam.cpp
extern "C" void CfObjectMove_setModelDisplayFlag(void* obj, int flag);      // CfObjectMove.cpp
extern "C" void CfObjectMove_setRegionAttached(void* obj, int flag);      // CfObjectMove.cpp
extern "C" void ColiSetMoveVec2(void* region, int param, float a, float b);
extern "C" void __ct__8009ED08(void* destination, u32 itemId);   // CtrlObjectParam.cpp
extern "C" unsigned long CfRes_getAllocHandle();   // bdat heap handle getter
// NOTE: retail keeps the virtual-call result in r3 across the second arg's
// address computation, so the value is the FIRST declared parameter.
// NOTE: the real definition is CActorParam_copyAll(CActorParam* dst, CActorParam*
// src) - the write target comes FIRST.
extern "C" void CActorParam_copyAll(cf::CActorParam* dst, cf::CActorParam* src);       // CActorParam.cpp
extern "C" void aiActionStoreIndexedBytes(u8* self, int index, int a2, int a3, int a4,
    int a5, int a6, int a7, int a8, int a9, int a10, int a11, int a12, int a13);
// getInstance__Q22cf14CBattleManagerFv is declared by kyoshin/cf/CBattleManagerApi.hpp
// (pulled in via CAIAction.hpp).
extern "C" void CActorParam_decayArtsMatrix__Q22cf11CActorParamFf(
    cf::CActorParam* self, float value);   // CActorParam virtual 0x354 (Fv name, float in f1)
extern "C" void CtrlObjectParam_RaiseArtsLevelBy(u8* obj, int flag);         // CtrlObjectParam.cpp
extern "C" void* CtrlObjectParam_GetCharEntry(u32 index);   // CtrlObjectParam.cpp (character-data lookup; owner canonical u32 form)
extern "C" void CtrlObjectParam_SyncParamFromActorEx(u8* obj, int flag);         // CtrlObjectParam.cpp
extern "C" void CfResPcImpl_loadPcState(u8* obj);                   // CfResPcImpl.cpp
extern "C" void UIBattleReleaseAll();                          // camera/screen reset
extern "C" void CActorParam_updateTick(void* actor, f32 value, u32 arg); // CActorParam.cpp
extern "C" int UIWin_HasPendingUpdate(int flag);                   // CfObjectImplWalker.cpp
extern const float lbl_eu_80666B24;   // .sdata2 gauge constants (UnkVirtualFunc4)
extern const float lbl_eu_80666B28;
extern const double lbl_eu_80666B30; // 4503601774854144.0 (2^52+2^51): doubles as the int->double biased-magic constant
extern "C" void BpsStateQueueDamage(u8* obj, int a, int b, int c);
extern "C" void Counter_AddClamped(u8* obj, u32 value, int a);
extern "C" void CtrlObjectParam_BumpArtsCounter(u8* obj, int flag);
extern "C" int CtrlObjectParam_ReapplyArtsSet(u8* obj);
extern "C" void fireLvUpMode4(u8* obj);
extern "C" void fireLvUpCombo(u8* obj, int learned);
// .rodata literal copied onto the stack by UnkVirtualFunc4 (struct defined
// below); bdat file ptr.
extern void* lbl_eu_80664090;

// sdata2 literals referenced by the CfObjectPc methods.
extern const float lbl_eu_80666B08;
extern const float lbl_eu_80666B14;
extern const float lbl_eu_80666B0C;
extern const float lbl_eu_80666B10;
extern const float lbl_eu_80666B18;
extern const float lbl_eu_80666B1C;
extern const float lbl_eu_80666B20;

// Pointee of the +0x3F60 resource pointer (+0x4B0 byte / flag word at
// +0x4EC).
struct PcResFlagsObj {
    u8 _0[0x4B0];
    u32 field_0x4B0;
    u8 _4B4[0x38];
    u32 field_0x4EC;
};

// Object returned by findBattleStatusEntry (holds a word at +0x10).
struct Res10View {
    u8 _0[0x10];
    u32 field_0x10;
};

// Imports used by CfObjectPc_refreshGauge.
extern u16 lbl_eu_80663E42;
extern u16 lbl_eu_80663E44;
extern void* lbl_eu_806640D4;
extern const float lbl_eu_80666B38;
extern const double lbl_eu_80666B40;
extern "C" {
void* Bdat_GetTable_AA34();                         // bdat manager refresh (void* return - must match CArtsInfo.hpp:679 / the other ~40 decls; divergent extern "C" types = 10197)
u32 Bdat_GetRowBase_B41C(void* mgr);                  // first bdat row index (canonical form per ocBdat.cpp def)
u32 Bdat_GetMaxRow_B1EC(void* mgr);                  // bdat row count (canonical form per ocBdat.cpp def)
unsigned long SuddenCommuIsStateActive(void* obj);        // battle-manager state probe (unified with CChainTimer.hpp)
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

// Foreign sub-object at +0x4 is a CObjectState pointer (owner: CObjectState).
// Slot 0x30 is getStateData (void*). Deleted fake iface;
// call via *(CObjectState**)((u8*)this+4)->getStateData().

// Foreign sub-object at +0x8 is the embedded CBattleState (owner: CBattleState).
// Slots 0x14/0x20 are CBattleState_UnkVirtualFunc4 / 7 (void(u32)). Deleted
// fake pad; call via reinterpret_cast<CBattleState*>((u8*)this+8)->...

// Linked-list node/head view of the battle-manager list at +0x48.
struct BmListNode {
    BmListNode* mNext;
};
struct BattleMgrListView {
    u8 _0[0x48];
    BmListNode mListHead;
};

// Range/state bytes of the battle manager checked by updatePc /
// CActorParam_UnkVirtualFunc88.
struct BattleMgrRangeView {
    u8 _0[0x1AA];
    u8 field_0x1AA;
    u8 _1AB[0x20C8 - 0x1AB];
    s16 field_0x20C8;

    bool isInRange() const {
        u8 phase = field_0x1AA;
        bool result = false;
        if (phase < 1) {
        } else if (phase > 0x18) {
        } else {
            result = true;
        }
        return result;
    }
};

// Layout of the CfObjectPc region below the CfObjectActor bases (inside the
// CfObjectMove sub-object at +0x3E9C); read by handleMoveState and the
// CActorParam_UnkVirtualFunc166/167 / resetArtsState family.
struct CfObjectPcSubFields {
    u8 _0[0x15E0];
    u32 field_0x15E0;  // 0x15E0: sub-object pointer word (set by refreshArtsData)
    u8 _15E4[0x1C];    // 0x15E4-0x15FF
    u32 field_0x1600;  // 0x1600: accumulated currency (clamped 99,999,999)
    u32 field_0x1604;  // 0x1604: spent counter (negated store)
    u32 field_0x1608;  // 0x1608: second accumulated counter (word-accessed)
    u8 _160C[0x1D];    // 0x160C-0x1628
    u8 field_0x1629;   // 0x1629-0x162C: bdat string bytes (CfObjectPc_setupFrame)
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

// Dtor vtable restores use direct *(void**) stores (same layout as former CfPcVt).
// Deleted CfPcVt pad; the shared struct is gone - both ctor and dtor write
// the four retail pointers directly.
namespace cf {
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

// Object examined by getValidObject: flags word at +0x64; bit 0x4000 marks
// the object as valid (returned as-is), otherwise NULL is returned.
struct FlagsObj800C1228 {
    u8 _0[0x64];
    u32 field_0x64;
};

namespace cf {
    //size: 0x45C0
    // Tail slots (0x5D4..0x608) are appended to the CfObjectMove vtable
    // group in declaration order: overrides of CfObjectMove-chain slots get
    // a direct tail slot plus an @16028@ thunk; the eight slots from
    // scanArtsEntries on are CfObjectPc's own virtuals.
    class CfObjectPc : public CfObjectActor {
    public:
        CfObjectPc();
        virtual ~CfObjectPc();                        //0x5A0
        virtual int CfObject_UnkVirtualFunc2();       //0x5D4 (CfObject slot 0x58: initialise)
        virtual void CfObject_UnkVirtualFunc3();      //0x5D8 (CfObject slot 0x5C: enable hook)
        virtual void CfObject_UnkVirtualFunc4();      //0x5DC (CfObject slot 0x60: per-frame update)
        virtual void CfObject_releaseMoveTargets();   //0x5E0 (CfObject slot 0x68)
        virtual void CObjectParam_UnkVirtualFunc4();  //0x5E4 (CObjectParam slot 0x48: refresh arts data)
        virtual void scanArtsEntries();               //0x5E8
        virtual int resetArtsState();                 //0x5EC
        virtual void applyArtsParam(int param);       //0x5F0
        virtual void dispatchPlayerBranch();          //0x5F4
        virtual void resetActionTable();              //0x5F8
        virtual void setupActionTable();              //0x5FC
        virtual void triggerActionRefreshA();         //0x600
        virtual void triggerActionRefreshB();         //0x604
        virtual void CfObjectMove_attachEffectSlot(u32 a, u32 b, u32 c, u32 d, u32 e); //0x608 (CfObjectMove slot 0x204)

        // Overrides that reuse an existing offset-0 slot (CActorParam tail).
        virtual void CActorParam_resetArtsStatus(void* arts);            //0xA4
        virtual int CActorParam_getCurrencyBalance();                    //0x1EC
        virtual void CActorParam_UnkVirtualFunc88(u32 a, u32 b, u32 c);  //0x1F4
        virtual void CActorParam_UnkVirtualFunc166();                    //0x32C
        virtual void CActorParam_UnkVirtualFunc167();                    //0x330
        virtual void CActorParam_UnkVirtualFunc173();                    //0x348
        virtual void CActorParam_decayArtsMatrix(float f1);              //0x354
        virtual int CActorParam_UnkVirtualFunc178();                     //0x35C

        inline UNKTYPE* unkInline1(){
            BOOL thing = CObjectParam_getSelfObjectId();
            UNKTYPE* idk = findObjectById(thing);
            return getEffOwner(idk);
        }

        inline UNKTYPE* unkInline2(){
            BOOL thing2 = CObjectParam_getSelfObjectId();
            UNKTYPE* idk2 = findObjectById(thing2);
            return getValidObject(idk2);
        }


        //0x0-45C0: CfObjectActor
    };
}
