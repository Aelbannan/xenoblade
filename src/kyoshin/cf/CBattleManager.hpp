#pragma once

#include <types.h>
#include "kyoshin/cf/CBattleManagerApi.hpp"
#include "kyoshin/cf/IFactoryEvent.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/IBattleEvent.hpp"
#include "kyoshin/cf/chain/CChain.hpp"
#include "kyoshin/cf/chain/UnkClass_800D8DBC.hpp"
// CSuddenCommu.hpp's C-linkage import signatures are aligned with the chain /
// CfObjectActor / CfGameManager headers (see CSuddenCommu.hpp), so it can be
// included directly without rename guards; its func_800F3970 copy is gone
// (single decl on CBattleManagerApi.hpp, included above).
#include "kyoshin/cf/CVision.hpp"
// CSuddenCommu.hpp's C-linkage import signatures are aligned with the chain /
// CfObjectActor / CfGameManager headers (see CSuddenCommu.hpp), so it can be
// included directly without rename guards.
#include "kyoshin/cf/CSuddenCommu.hpp"
#include "monolib/util.hpp"
#include <cstring>
#include "kyoshin/cf/CfGameManagerData.hpp"  // H3 label-owner decl (lbl_eu_80663E14; lbl_eu_80663E24)

namespace cf{
    class UnkClass_8018C5FC {
    public:
        UnkClass_8018C5FC();
        
        u8 unk0[8];
    };

    class UnkClass_80192BF4 {
    public:
        UnkClass_80192BF4();
        void __ct__80192C10();

        u8 unk0[0xC];
    };

    // 8 bytes: 4-byte key + 1-byte count + 3 padding
    struct CBattleManager_Struct1 {
        CBattleManager_Struct1() {
            // 8-byte zero fill: retail emits the rule-array member
            // construction (inside CBattleManager's ctor) as a per-element
            // memset(this, 0, 8) loop.
            std::memset(this, 0, sizeof(*this));
        }

        s32 key;    // +0x00
        u8 count;   // +0x04
        u8 pad[3];  // +0x05..0x07
    };

    struct CBattleManager_Struct2 {
        CBattleManager_Struct2(){
            clear();
        }

        void clear(){
            std::memset(unk0, 0, sizeof(unk0));
        }

        CBattleManager_Struct1 unk0[32];
    };

    //size: 0x283D8
    class CBattleManager : public IFactoryEvent {
    public:
        CBattleManager();
        virtual ~CBattleManager(); //0x8
        virtual void FactoryEvent2(); //0x10
        virtual void func_80085220(u32 r4, u32 r5); //0x1C
        virtual void func_800E2584(u32 mask); //0x20
        virtual void func_800F42A0(); //0x24
        virtual void func_800885F0(); //0x28
        virtual void func_800EA410(); //0x2C
        virtual void* func_800EA420(); //0x30
        virtual void func_800EA460(float a, float b, unsigned long c); //0x34
        virtual void func_800EA470(); //0x38
        virtual void func_800EA998(); //0x3C

        void* func_800EA444();

        static CBattleManager* getInstance();
        // TUs that cannot include this class reach the same retail symbol
        // through kyoshin/cf/CBattleManagerApi.hpp (the one shared copy).
        static void func_800D9190();
        static void func_800D91D0();
        void func_800D9218();

        //0x0: vtable
        //0x0-4: IFactoryEvent
        reslist<CfObjectActor*> mActorList1; //0x4
        reslist<CfObjectActor*> mActorList2; //0x24
        reslist<CfObjectActor*> mActorList3; //0x44
        reslist<IBattleEvent*> mBattleEventList; //0x64
        u32 unk84; //0x84
        float unk88;
        u32 unk8C;
        float unk90;
        CBattleManager_Struct2 unk94;
        UnkClass_8018C5FC unk194; //0x194
        UnkClass_80192BF4 unk19C; //0x19C
        CChain mChain; //0x1A8
        UnkClass_800D8DBC unk20C8; //0x20C8
        CSuddenCommu mSuddenCommu; //0x216C
        CVision mVision; //0x219C

        // sound-id table (10 known entries; retail .rodata symbol)
        // static u32 lbl_804F8228[]; -> moved to global lbl_eu_804FCA08

    protected:
        // static CBattleManager* spInstance; -> extern "C" lbl_eu_80663F00 in .cpp
    };
}

// --- Standalone function access structs (CBattleManager.cpp) ---

// Intrusive linked-list node: +0x00 = next, +0x04 = unk, +0x08 = data ptr
struct SimpleListNode {
    SimpleListNode* next; // +0x00
    u32 field_4;          // +0x04
    void* data;           // +0x08
};

// Return layout of func_8009EC9C (accessed at +0x1C, cast_int_arith L63/L80)
struct UnkStruct_8009EC9C_Ret {
    u8 pad_00[0x1C];
    u8 unk1C;
};

// Layout for func_800EA384 self: list sentinel ptr at +0x08
struct Func800EA384_Self {
    u8 pad_00[0x08];
    SimpleListNode* listHead;
};

// Layout for func_800F4004 this_: list sentinel ptr at +0x48
struct Func800F4004_Self {
    u8 pad_00[0x48];
    SimpleListNode* listHead;
};

// Object layout used by func_800EA9A8 / func_800DBA2C
// Has a field at +0x3f00 (flags), +0x3f10 (ptr), +0x3f28 (u16)
struct BattleObjAccessor {
    u8 pad_00[0x3f00];
    u32 field_3f00;     // flags at +0x3f00
    u8 pad_3f04[0x0C];  // 0x3f04-0x3f0F
    void* field_3f10;   // ptr at +0x3f10
    u8 pad_3f14[0x14];  // 0x3f14-0x3f27
    u16 field_3f28;     // u16 at +0x3f28
};

// Object layout used by func_800DBA2C for arg2 access at +0x78
// Has a field at +0x50 (ptr), +0x78 (flags)
struct BattleMoveObjAccessor {
    u8 pad_00[0x50];
    void* field_50;     // ptr at +0x50
    u8 pad_54[0x24];
    u32 field_78;       // flags at +0x78
};

// Move/params block for func_800DB4FC (arg4): arts-data pointer at +0x50,
// float pack at +0x54..+0x64, flags at +0x74/+0x78, gate value at +0xB4.
struct DB4FC_MoveBlock {
    u8 pad_00[0x50];
    void* artsData;             // +0x50
    f32 field_54;               // +0x54
    f32 field_58;               // +0x58
    f32 field_5C;               // +0x5C
    f32 field_60;               // +0x60
    f32 field_64;               // +0x64
    u8 pad_68[0x74 - 0x68];
    u32 field_74;               // +0x74
    u32 field_78;               // +0x78
    u8 pad_7C[0xB4 - 0x7C];
    s32 field_B4;              // +0xB4 (signed: retail emits signed cmpw)
};

// Object layout used by func_800DBA2C for r31 (*(arg2+0x50))
// Has fields at +0x3c (u16 type), +0x40 (u16)
struct BattleSubObjAccessor {
    u8 pad_00[0x3c];
    u16 type_3c;        // type at +0x3c
    u16 field_3e;       // padding?
    u16 field_40;       // value at +0x40
};

// Target/arts object fields probed by func_800E1B5C: u16 type +0x3C,
// u16 id +0x48, flags word +0x78.
struct E1B5C_TargetObj {
    u8 pad_00[0x3c];
    u16 type_3c;        // +0x3C
    u8 pad_3e[0x48 - 0x3e];
    u16 id_48;          // +0x48
    u8 pad_4a[0x78 - 0x4a];
    u32 flags_78;       // +0x78
};

// Battle-actor / action / player view used by func_800E1B5C: status flags
// (+0x3374), vtable holder (+0x3ED4), flags (+0x3F00), ptr (+0x3F10),
// u16 type (+0x3F28), gate word (+0x3F60).
struct E1B5C_ObjView {
    u8 pad_00[0x3374];
    u32 field_3374;     // +0x3374
    u8 pad_3378[0x3ED4 - 0x3378];
    void* field_3ED4;   // +0x3ED4
    u8 pad_3ED8[0x3F00 - 0x3ED8];
    u32 field_3F00;     // +0x3F00
    u8 pad_3F04[0x3F10 - 0x3F04];
    void* field_3F10;   // +0x3F10
    u8 pad_3F14[0x3F28 - 0x3F14];
    u16 field_3F28;     // +0x3F28
    u8 pad_3F2A[0x3F60 - 0x3F2A];
    u32 field_3F60;     // +0x3F60
};

// Flags-word view onto a move-data block (+0x78 only).
struct E1B5C_MoveFlags {
    u8 pad_00[0x78];
    u32 flags_78;
};

// Object layout for the removed-target access in FactoryEvent2 / func_800D9CA0
// +0x3ED4 = ptr to vtable-holder, +0x3F00 = flags
struct BattleRemoveObjAccessor {
    u8 pad_00[0x3ED4];
    void* field_3ed4;    // +0x3ED4 ptr (has vtable[0x7C] call)
    u8 pad_3ed8[0x28];   // 0x3ED8-0x3EFF
    u32 field_3f00;      // +0x3F00 flags
};

// Enemy/status object view for func_800DB4FC's arg3: status region base at
// +0x8 (func_80148778 operates on &+0x8), flags at +0x3374, embedded move
// sub-object at +0x3E9C (vtable slot 0x140, BMSubVtIf140 view).
struct DB4FC_EnemyObj {
    u8 pad_00[0x8];
    u8 statusBase;              // +0x8
    u8 pad_09[0x3374 - 0x9];
    u32 field_3374;             // +0x3374
    u8 pad_3378[0x3E9C - 0x3378];
    u8 moveSub;                 // +0x3E9C (vptr of the move sub-object)
};

// Arts/skill object (func_800DB4FC arg2->artsData): byte at +0x44,
// u16 at +0x58, flags at +0x78, sub-vtable pointer at +0x84.
struct DB4FC_ArtsObj {
    u8 pad_00[0x44];
    u8 field_44;                // +0x44
    u8 pad_45[0x58 - 0x45];
    u16 field_58;               // +0x58
    u8 pad_5A[0x78 - 0x5A];
    u32 field_78;               // +0x78
    u8 pad_7C[0x84 - 0x7C];
    void* field_84;             // +0x84 (sub-table, slot 0xC dispatch)
};

// Move sub-object interface (embedded at battle actors +0x3E9C): the status
// probe used by func_800F3E8C lives at compiled vtable slot 0x4C (declared
// index 17 under -RTTI). Never instantiated here, so no vtable is emitted.
class BMSubVtIf4C {
public:
    virtual void bmSub00();  // 0 -> +0x00
    virtual void bmSub01();
    virtual void bmSub02();
    virtual void bmSub03();
    virtual void bmSub04();
    virtual void bmSub05();
    virtual void bmSub06();
    virtual void bmSub07();
    virtual void bmSub08();
    virtual void bmSub09();
    virtual void bmSub0A();
    virtual void bmSub0B();
    virtual void bmSub0C();
    virtual void bmSub0D();
    virtual void bmSub0E();
    virtual void bmSub0F();
    virtual void bmSub10();  // 16 -> +0x48
    virtual s32 probeId();   // 17 -> +0x4C (owner actor/type id)
};

// Actor object scanned by func_800F3E8C: type id at +0x15F0 and the embedded
// move sub-object at +0x3E9C.
struct BattleScanActorView {
    u8 pad_00[0x15F0];
    u32 field_15F0;                    // +0x15F0 type id
    u8 pad_15F4[0x3E9C - 0x15F4];
    BMSubVtIf4C field_3E9C;            // embedded move sub-object
};

// Sentinel-pointer view (offset-typed alias of CBattleManager used to keep
// func_800F3E8C pass 2's sentinel load independent of pass 1's).
struct BMSentinelView {
    u8 pad00[0x48];
    _reslist_node<cf::CfObjectActor*>* sentinel;  // +0x48
};

// func_8016FE34 result view for func_800F3E8C: flags at +0x3F00 (bit 2 =
// targetable) and type id at +0x15F0.
struct BattleScanStateView {
    u8 pad_00[0x15F0];
    u32 field_15F0;                    // +0x15F0 type id
    u8 pad_15F4[0x3F00 - 0x15F4];
    u32 field_3F00;                    // +0x3F00 flags
};

// Absolute-offset view of an actor's embedded move sub-object (the C++ base
// layout in CfObjectActor.hpp is 8 bytes off retail, so raw views pin the
// offset; cf. CfActorField3F00 in CfObjectActor.hpp).
struct ActorMoveSubView3E9C {
    u8 pad_00[0x3E9C];
    u8 sub3E9C;                 // +0x3E9C (move sub-object vptr)
};

// Source/arts object view for func_800D81A8 (the +0x50 deref of its source
// param): arts type at +0x3C, art type at +0x40, flags at +0x78.
struct BattleSrcObjAccessor {
    u8 pad_00[0x3c];
    u16 type_3c;                // +0x3C
    u16 field_3e;
    u16 field_40;               // +0x40
    u8 pad_42[0x78 - 0x42];
    u32 field_78;               // +0x78
};

// func_800D81A8 source-param view: linked arts/status object pointer at +0x50.
struct D81A8_SourceView {
    u8 pad_00[0x50];
    BattleSrcObjAccessor* link;   // +0x50
};

// Battle-status list base at actor +0x08 (member-address form keeps MWCC from
// CSE-ing the three probes in func_800D81A8 into a shared temp register).
struct D81A8_StatusListView {
    u8 pad_00[0x08];
    u8 statusList;   // +0x08
};

// Status/arts-data entry returned by func_80149154 (func_800D81A8 reads the
// s32 value at +0x10).
struct BDSessionEntry {
    u8 pad_00[0x10];
    s32 field_10;               // +0x10
};

// Battle-manager +0x20AC chain-gauge view used by func_800D81A8.
struct CBattleManager_20AC {
    u8 pad_00[0x20AC];
    f32 field_20AC;             // +0x20AC
};

// 4th-arg block of func_800DB0FC: link at +0x50, flags at +0x74/+0x78,
// u16 dispatch counter-out at +0x80, and a 13-entry u32 ai table at +0x84.
struct DB0FC_MoveTable {
    u8 pad_00[0x50];
    void* field_50;             // +0x50
    u8 pad_54[0x74 - 0x54];
    u32 field_74;               // +0x74
    u32 field_78;               // +0x78
    u32 field_7C;               // +0x7C (unmodeled retail member)
    u32 pad_80;                 // +0x80 (u16 field_80 + 2 pad bytes)
    u32 table[13];              // +0x84
};
#include <stddef.h>
// Layout checks for DB0FC_MoveTable (+0x50/+0x74/+0x78/+0x84). MWCC rejects
// offsetof in constant expressions (see CfCam.cpp note), so these are
// documented rather than enforced at compile time.
#include <stddef.h>
typedef char DB0FC_ck50[(offsetof(DB0FC_MoveTable, field_50) == 0x50) ? 1 : -1];
typedef char DB0FC_ck74[(offsetof(DB0FC_MoveTable, field_74) == 0x74) ? 1 : -1];
typedef char DB0FC_ck78[(offsetof(DB0FC_MoveTable, field_78) == 0x78) ? 1 : -1];
typedef char DB0FC_ck7C[(offsetof(DB0FC_MoveTable, field_7C) == 0x7C) ? 1 : -1];
typedef char DB0FC_ck80[(offsetof(DB0FC_MoveTable, pad_80)   == 0x80) ? 1 : -1];
typedef char DB0FC_ck84[(offsetof(DB0FC_MoveTable, table)    == 0x84) ? 1 : -1];

// Object layout for func_800D9978's actor param: flags at +0x3374,
// vtable-holder at +0x3ED4 (BMSub3ED4Vt view), flags at +0x3F00.
struct BattleRegObjAccessor {
    u8 pad_00[0x3374];
    u32 field_3374;             // +0x3374
    u8 pad_3378[0x3ED4 - 0x3378];
    void* field_3ED4;           // +0x3ED4
    u8 pad_3ED8[0x3F00 - 0x3ED8];
    u32 field_3F00;             // +0x3F00
};

// Trailing battle-manager fields at the end of the 0x283D8-byte object:
// a u16 ai/move dispatch counter at +0x283D0 (func_800DB0FC increments it)
// and the accumulated flash/ticker timer at +0x283D4 drained by
// func_800D9354.
struct CBattleManagerTail283D4 {
    u8 pad_00[0x283D0];
    u16 field_283D0;            // +0x283D0
    u8 pad_02[2];
    f32 field_283D4;            // +0x283D4
};

// ---------------------------------------------------------------------------
// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
// ---------------------------------------------------------------------------

// Battle event dispatch table entry for lbl_eu_804FC828 (12 bytes each)
struct BattleTableEntry {
    u32 selector;
    s16 val;
    s8 byteVal;
    u8 pad_07;
    u32 pad_08;
};

extern "C" void __dl__FPv(void*);
extern "C" void __dla__FPv(void*);
extern "C" float lbl_eu_80666DDC;
extern "C" float lbl_eu_80666DD4;
extern "C" float lbl_eu_80666DD0;   // 0.6f (attack-sound volume)
extern "C" f64 lbl_eu_80666DE0;
extern "C" float lbl_eu_80666DEC;   // time/frame-delta scale (func_800D9354)
extern "C" float lbl_eu_8066A1FC;   // 2*pi (angle-wrap step)
extern "C" BattleTableEntry lbl_eu_804FC828[];
extern u32 lbl_eu_804FCA08[];       // attack sound-id table (retail .rodata)

// reslist<...> base-class vtables (named so the reslist deleting-dtor vtable
// store emits the retail symbol rather than an auto-mangled __vt__ name).
extern "C" u8 lbl_eu_8052BD68[];  // _reslist_base<CfObjectActor*> vtable
extern "C" u8 lbl_eu_8052BD44[];  // _reslist_base<IBattleEvent*> vtable
extern "C" u8 lbl_eu_8052BCE0[];  // CBattleManager vtable (restored by the dtor)
extern "C" u8 lbl_eu_8052BD74[];  // CChainTime vtable (restored by ~CChainTime)

// Same-TU / shared battle helpers with retail-unmangled names (declared in
// CBattleManager.hpp so call sites in this unit emit the retail reloc names
// instead of a namespace-mangled C++ form).
extern "C" void func_800F41A0(cf::CBattleManager* mgr);
extern "C" void func_80279694(cf::CChain* chain, cf::CfObjectActor* actor);
extern "C" void func_80277B34(cf::CChain* chain);
extern "C" void func_800DB4FC(void* self, void* obj, void* enemy, void* move);
extern "C" void func_800DB7F8(void* self, void* obj, void* enemy, void* move);
extern "C" void func_800DBA2C(void* self, void* obj, void* enemy, void* move);
extern "C" int func_802799F0(void* chain, void* obj);

// C++-mangled retail import (func_800B708C__Fi): actor id -> action source.
void* func_800B708C(int id);

// Object scanned by func_800D7D24: key word at +0x4, flags at +0x78 and the
// 0x20-stride entry block pointer at +0xB8.
struct D7D24_Obj {
    u8 pad_00[0x04];
    u32 field_4;            // +0x04
    u8 pad_08[0x78 - 0x08];
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0xB8 - 0x7C];
    u32* field_B8;          // +0xB8 entry block
};

// Fake single-inheritance interface for the battle manager's vtable slot
// 0x28 (func_800885F0, called with a u32 mask by func_800F3C6C). Under the
// kyoshin -RTTI layout a declared virtual #k lands at byte offset (k+2)*4,
// so the 9th declared virtual sits at 0x28. Never instantiated.
struct BMVtIf828 {
    virtual void v000() = 0;
    virtual void v001() = 0;
    virtual void v002() = 0;
    virtual void v003() = 0;
    virtual void v004() = 0;
    virtual void v005() = 0;
    virtual void v006() = 0;
    virtual void v007() = 0;
    virtual s32 v008(u32 mask) = 0;   // slot 0x28
};

// Fake single-inheritance interface for the battle-actor vtable slots
// dispatched by func_800F3970: 0x2F8 (#188), 0x2FC (#189) and 0x304 (#191)
// under the kyoshin -RTTI layout ((k+2)*4). Never instantiated.
struct BMVtIfF3970Base {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0;
    virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0;
    virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0;
    virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0;
    virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0;
    virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0;
    virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0;
    virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0;
    virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0;
    virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0;
    virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
    virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0;
    virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
    virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0;
    virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
    virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0;
    virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0;
    virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
    virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0;
    virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
    virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0;
    virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
    virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0;
    virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
    virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0;
    virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0;
    virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
    virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0;
    virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
    virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0;
    virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
    virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0;
    virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;
    virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0;
    virtual void v165() = 0; virtual void v166() = 0; virtual void v167() = 0;
    virtual void v168() = 0; virtual void v169() = 0; virtual void v170() = 0;
    virtual void v171() = 0; virtual void v172() = 0; virtual void v173() = 0;
    virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0;
    virtual void v177() = 0; virtual void v178() = 0; virtual void v179() = 0;
    virtual void v180() = 0; virtual void v181() = 0; virtual void v182() = 0;
    virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0;
    virtual void v186() = 0; virtual void v187() = 0;  // pads #0..#187
};
struct BMVtIfF3970 : BMVtIfF3970Base {
    virtual void vf2F8(s32 arg) = 0;   // #188 -> 0x2F8
    virtual void vf2FC(s32 arg) = 0;   // #189 -> 0x2FC
    virtual void v190() = 0;           // #190 (unused gap)
    virtual void vf304(s32 arg) = 0;   // #191 -> 0x304
};

// Manual-vtable interface for battle actors dispatched by func_800D81A8:
// slots 0x130 (#74), 0x290 (#158) and 0x5C0 (#366) under the kyoshin -RTTI
// layout ((k+2)*4). Real C++ dispatch makes MWCC emit the canonical
// lwz r12 / mtctr / bctrl sequence instead of pointer-cast helper calls.
// Never instantiated.
struct BMVtIfD81A8 {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0;
    virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0;
    virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0;
    virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0;
    virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0;
    virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0;
    virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0;
    virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0;  // pads #0..#73
    virtual f32 vf130() = 0;           // #74 -> 0x130
    virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0;
    virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0;
    virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
    virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0;
    virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
    virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0;
    virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
    virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0;
    virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0;
    virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
    virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0;
    virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
    virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0;
    virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
    virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0;
    virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
    virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0;
    virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0;
    virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
    virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0;
    virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
    virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0;
    virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
    virtual void v156() = 0; virtual void v157() = 0;
    virtual void v158() = 0; virtual void v159() = 0;
    virtual void v160() = 0; virtual void v161() = 0;  // pads #75..#161
    virtual void* vf290() = 0;         // -> 0x290
    virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0;
    virtual void v165() = 0; virtual void v166() = 0; virtual void v167() = 0;
    virtual void v168() = 0; virtual void v169() = 0; virtual void v170() = 0;
    virtual void v171() = 0; virtual void v172() = 0; virtual void v173() = 0;
    virtual void v174() = 0; virtual void v175() = 0; virtual void v176() = 0;
    virtual void v177() = 0; virtual void v178() = 0; virtual void v179() = 0;
    virtual void v180() = 0; virtual void v181() = 0; virtual void v182() = 0;
    virtual void v183() = 0; virtual void v184() = 0; virtual void v185() = 0;
    virtual void v186() = 0; virtual void v187() = 0; virtual void v188() = 0;
    virtual void v189() = 0; virtual void v190() = 0; virtual void v191() = 0;
    virtual void v192() = 0; virtual void v193() = 0; virtual void v194() = 0;
    virtual void v195() = 0; virtual void v196() = 0; virtual void v197() = 0;
    virtual void v198() = 0; virtual void v199() = 0; virtual void v200() = 0;
    virtual void v201() = 0; virtual void v202() = 0; virtual void v203() = 0;
    virtual void v204() = 0; virtual void v205() = 0; virtual void v206() = 0;
    virtual void v207() = 0; virtual void v208() = 0; virtual void v209() = 0;
    virtual void v210() = 0; virtual void v211() = 0; virtual void v212() = 0;
    virtual void v213() = 0; virtual void v214() = 0; virtual void v215() = 0;
    virtual void v216() = 0; virtual void v217() = 0; virtual void v218() = 0;
    virtual void v219() = 0; virtual void v220() = 0; virtual void v221() = 0;
    virtual void v222() = 0; virtual void v223() = 0; virtual void v224() = 0;
    virtual void v225() = 0; virtual void v226() = 0; virtual void v227() = 0;
    virtual void v228() = 0; virtual void v229() = 0; virtual void v230() = 0;
    virtual void v231() = 0; virtual void v232() = 0; virtual void v233() = 0;
    virtual void v234() = 0; virtual void v235() = 0; virtual void v236() = 0;
    virtual void v237() = 0; virtual void v238() = 0; virtual void v239() = 0;
    virtual void v240() = 0; virtual void v241() = 0; virtual void v242() = 0;
    virtual void v243() = 0; virtual void v244() = 0; virtual void v245() = 0;
    virtual void v246() = 0; virtual void v247() = 0; virtual void v248() = 0;
    virtual void v249() = 0; virtual void v250() = 0; virtual void v251() = 0;
    virtual void v252() = 0; virtual void v253() = 0; virtual void v254() = 0;
    virtual void v255() = 0; virtual void v256() = 0; virtual void v257() = 0;
    virtual void v258() = 0; virtual void v259() = 0; virtual void v260() = 0;
    virtual void v261() = 0; virtual void v262() = 0; virtual void v263() = 0;
    virtual void v264() = 0; virtual void v265() = 0; virtual void v266() = 0;
    virtual void v267() = 0; virtual void v268() = 0; virtual void v269() = 0;
    virtual void v270() = 0; virtual void v271() = 0; virtual void v272() = 0;
    virtual void v273() = 0; virtual void v274() = 0; virtual void v275() = 0;
    virtual void v276() = 0; virtual void v277() = 0; virtual void v278() = 0;
    virtual void v279() = 0; virtual void v280() = 0; virtual void v281() = 0;
    virtual void v282() = 0; virtual void v283() = 0; virtual void v284() = 0;
    virtual void v285() = 0; virtual void v286() = 0; virtual void v287() = 0;
    virtual void v288() = 0; virtual void v289() = 0; virtual void v290() = 0;
    virtual void v291() = 0; virtual void v292() = 0; virtual void v293() = 0;
    virtual void v294() = 0; virtual void v295() = 0; virtual void v296() = 0;
    virtual void v297() = 0; virtual void v298() = 0; virtual void v299() = 0;
    virtual void v300() = 0; virtual void v301() = 0; virtual void v302() = 0;
    virtual void v303() = 0; virtual void v304() = 0; virtual void v305() = 0;
    virtual void v306() = 0; virtual void v307() = 0; virtual void v308() = 0;
    virtual void v309() = 0; virtual void v310() = 0; virtual void v311() = 0;
    virtual void v312() = 0; virtual void v313() = 0; virtual void v314() = 0;
    virtual void v315() = 0; virtual void v316() = 0; virtual void v317() = 0;
    virtual void v318() = 0; virtual void v319() = 0; virtual void v320() = 0;
    virtual void v321() = 0; virtual void v322() = 0; virtual void v323() = 0;
    virtual void v324() = 0; virtual void v325() = 0; virtual void v326() = 0;
    virtual void v327() = 0; virtual void v328() = 0; virtual void v329() = 0;
    virtual void v330() = 0; virtual void v331() = 0; virtual void v332() = 0;
    virtual void v333() = 0; virtual void v334() = 0; virtual void v335() = 0;
    virtual void v336() = 0; virtual void v337() = 0; virtual void v338() = 0;
    virtual void v339() = 0; virtual void v340() = 0; virtual void v341() = 0;
    virtual void v342() = 0; virtual void v343() = 0; virtual void v344() = 0;
    virtual void v345() = 0; virtual void v346() = 0; virtual void v347() = 0;
    virtual void v348() = 0; virtual void v349() = 0; virtual void v350() = 0;
    virtual void v351() = 0; virtual void v352() = 0; virtual void v353() = 0;
    virtual void v354() = 0; virtual void v355() = 0; virtual void v356() = 0;
    virtual void v357() = 0; virtual void v358() = 0; virtual void v359() = 0;
    virtual void v360() = 0; virtual void v361() = 0; virtual void v362() = 0;
    virtual void v363() = 0; virtual void v364() = 0;  // pads #159..#365 (203 pads)
    virtual void* vf5C0(void* arg) = 0;   // #366 -> 0x5C0
};

// Manual-vtable interface for the embedded sub-object at actor+0x3E9C
// (func_800DB7F8): the caller casts (actor + 0x3E9C) to this type and the
// vptr is read from offset 0; slot 0x140 (#78) is called with this =
// actor + 0x3E9C. Never instantiated.
struct BMSubVtIf140 {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0;
    virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0;
    virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0;
    virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0;
    virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0;
    virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0;
    virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0;
    virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0;
    virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;  // pads #0..#77
    virtual f32 vf140() = 0;        // #78 -> 0x140
};

// Manual-vtable interface for the move sub-object's +0x84 sub-vtable
// (func_800DB7F8): the vptr is read from +0x84 and slot 0xC (#1) is called
// with this = the move sub-object. Never instantiated.
struct BMSubVtIf0C {
    u8 pad84[0x84];
    virtual void v000() = 0;
    virtual s32 vf0C() = 0;         // #1 -> 0xC
};

// Plain vtable interface for slot 0x2A8 (#166) on the actor object
// (func_800DB4FC's arts-type check calls it and compares the result +1
// against the arts byte at +0x44). Never instantiated, so no vtable emits.
struct BMVtIf2A8 {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0;
    virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0;
    virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0;
    virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0;
    virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0;
    virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0;
    virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0;
    virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0;
    virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0;
    virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0;
    virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
    virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0;
    virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
    virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0;
    virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
    virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0;
    virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0;
    virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
    virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0;
    virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
    virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0;
    virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
    virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0;
    virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
    virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0;
    virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0;
    virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
    virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0;
    virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
    virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0;
    virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
    virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0;
    virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;
    virtual void v162() = 0; virtual void v163() = 0; virtual void v164() = 0;
    virtual void v165() = 0;
    virtual void v166() = 0; virtual void v167() = 0;  // pads #0..#167
    virtual s32 vf2A8();        // #168 -> 0x2A8
};

// Vtable interface for a battle event's slot 0xC (#1), called with
// (self, actor) by func_800D9978's registration broadcast. Never
// instantiated.
struct BattleEventVtIf0C {
    virtual void v000() = 0;
    virtual void v00C(void* actor) = 0;   // #1 -> 0xC
};

// Vtable interface for the vtable-holder at actor+0x3ED4 (func_800D9978):
// slots 0x78 (#28) and 0x100 (#62) are dispatched on registration. The
// holder pointer is read from +0x3ED4; this = the holder. Never instantiated.
struct BMSub3ED4Vt {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0;  // pads #0..#27
    virtual void vf78();        // #28 -> 0x78
    virtual void v029() = 0; virtual void v030() = 0; virtual void v031() = 0;
    virtual void v032() = 0; virtual void v033() = 0; virtual void v034() = 0;
    virtual void v035() = 0; virtual void v036() = 0; virtual void v037() = 0;
    virtual void v038() = 0; virtual void v039() = 0; virtual void v040() = 0;
    virtual void v041() = 0; virtual void v042() = 0; virtual void v043() = 0;
    virtual void v044() = 0; virtual void v045() = 0; virtual void v046() = 0;
    virtual void v047() = 0; virtual void v048() = 0; virtual void v049() = 0;
    virtual void v050() = 0; virtual void v051() = 0; virtual void v052() = 0;
    virtual void v053() = 0; virtual void v054() = 0; virtual void v055() = 0;
    virtual void v056() = 0; virtual void v057() = 0; virtual void v058() = 0;
    virtual void v059() = 0; virtual void v060() = 0; virtual void v061() = 0;  // pads #29..#61
    virtual void vf100();       // #62 -> 0x100
};

// Plain vtable interface for slot 0x290 (#162) on the actor object
// (func_800DB7F8's type-2 path calls it twice). Never instantiated.
struct BMVtIf290 {
    virtual void v000() = 0; virtual void v001() = 0; virtual void v002() = 0;
    virtual void v003() = 0; virtual void v004() = 0; virtual void v005() = 0;
    virtual void v006() = 0; virtual void v007() = 0; virtual void v008() = 0;
    virtual void v009() = 0; virtual void v010() = 0; virtual void v011() = 0;
    virtual void v012() = 0; virtual void v013() = 0; virtual void v014() = 0;
    virtual void v015() = 0; virtual void v016() = 0; virtual void v017() = 0;
    virtual void v018() = 0; virtual void v019() = 0; virtual void v020() = 0;
    virtual void v021() = 0; virtual void v022() = 0; virtual void v023() = 0;
    virtual void v024() = 0; virtual void v025() = 0; virtual void v026() = 0;
    virtual void v027() = 0; virtual void v028() = 0; virtual void v029() = 0;
    virtual void v030() = 0; virtual void v031() = 0; virtual void v032() = 0;
    virtual void v033() = 0; virtual void v034() = 0; virtual void v035() = 0;
    virtual void v036() = 0; virtual void v037() = 0; virtual void v038() = 0;
    virtual void v039() = 0; virtual void v040() = 0; virtual void v041() = 0;
    virtual void v042() = 0; virtual void v043() = 0; virtual void v044() = 0;
    virtual void v045() = 0; virtual void v046() = 0; virtual void v047() = 0;
    virtual void v048() = 0; virtual void v049() = 0; virtual void v050() = 0;
    virtual void v051() = 0; virtual void v052() = 0; virtual void v053() = 0;
    virtual void v054() = 0; virtual void v055() = 0; virtual void v056() = 0;
    virtual void v057() = 0; virtual void v058() = 0; virtual void v059() = 0;
    virtual void v060() = 0; virtual void v061() = 0; virtual void v062() = 0;
    virtual void v063() = 0; virtual void v064() = 0; virtual void v065() = 0;
    virtual void v066() = 0; virtual void v067() = 0; virtual void v068() = 0;
    virtual void v069() = 0; virtual void v070() = 0; virtual void v071() = 0;
    virtual void v072() = 0; virtual void v073() = 0; virtual void v074() = 0;
    virtual void v075() = 0; virtual void v076() = 0; virtual void v077() = 0;
    virtual void v078() = 0; virtual void v079() = 0; virtual void v080() = 0;
    virtual void v081() = 0; virtual void v082() = 0; virtual void v083() = 0;
    virtual void v084() = 0; virtual void v085() = 0; virtual void v086() = 0;
    virtual void v087() = 0; virtual void v088() = 0; virtual void v089() = 0;
    virtual void v090() = 0; virtual void v091() = 0; virtual void v092() = 0;
    virtual void v093() = 0; virtual void v094() = 0; virtual void v095() = 0;
    virtual void v096() = 0; virtual void v097() = 0; virtual void v098() = 0;
    virtual void v099() = 0; virtual void v100() = 0; virtual void v101() = 0;
    virtual void v102() = 0; virtual void v103() = 0; virtual void v104() = 0;
    virtual void v105() = 0; virtual void v106() = 0; virtual void v107() = 0;
    virtual void v108() = 0; virtual void v109() = 0; virtual void v110() = 0;
    virtual void v111() = 0; virtual void v112() = 0; virtual void v113() = 0;
    virtual void v114() = 0; virtual void v115() = 0; virtual void v116() = 0;
    virtual void v117() = 0; virtual void v118() = 0; virtual void v119() = 0;
    virtual void v120() = 0; virtual void v121() = 0; virtual void v122() = 0;
    virtual void v123() = 0; virtual void v124() = 0; virtual void v125() = 0;
    virtual void v126() = 0; virtual void v127() = 0; virtual void v128() = 0;
    virtual void v129() = 0; virtual void v130() = 0; virtual void v131() = 0;
    virtual void v132() = 0; virtual void v133() = 0; virtual void v134() = 0;
    virtual void v135() = 0; virtual void v136() = 0; virtual void v137() = 0;
    virtual void v138() = 0; virtual void v139() = 0; virtual void v140() = 0;
    virtual void v141() = 0; virtual void v142() = 0; virtual void v143() = 0;
    virtual void v144() = 0; virtual void v145() = 0; virtual void v146() = 0;
    virtual void v147() = 0; virtual void v148() = 0; virtual void v149() = 0;
    virtual void v150() = 0; virtual void v151() = 0; virtual void v152() = 0;
    virtual void v153() = 0; virtual void v154() = 0; virtual void v155() = 0;
    virtual void v156() = 0; virtual void v157() = 0; virtual void v158() = 0;
    virtual void v159() = 0; virtual void v160() = 0; virtual void v161() = 0;  // pads #0..#161
    virtual void* vf290() = 0;      // #162 -> 0x290
};

// Move sub-object view for func_800DB7F8 (arg4->artsData): u16 type at
// +0x58 and the sub-vtable pointer at +0x84.
struct DB7F8_MoveSub {
    u8 pad_00[0x58];
    u16 type_58;                    // 0x58
    u8 pad_5A[0x84 - 0x5A];
    void** subVt84;                 // 0x84
};
