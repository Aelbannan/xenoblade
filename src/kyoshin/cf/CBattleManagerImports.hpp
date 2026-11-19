#pragma once

// TU-private imports and layout views for kyoshin/cf/CBattleManager.cpp
// (moved out of the public CBattleManager.hpp so that header stays lean).

#include "kyoshin/cf/CBattleManager.hpp"
#include "kyoshin/cf/object/CfObjectActor.hpp"
#include "kyoshin/cf/chain/CChainImports.hpp"
#include "kyoshin/cf/CVisionImports.hpp"
#include "kyoshin/cf/CSuddenCommuImports.hpp"

// --- Standalone function access structs (CBattleManager.cpp) ---

// Intrusive linked-list node: +0x00 = next, +0x04 = unk, +0x08 = data ptr
struct SimpleListNode {
    SimpleListNode* next; // +0x00
    u32 field_4;          // +0x04
    void* data;           // +0x08
};

// Return layout of CtrlObjectParam_GetCharEntry (accessed at +0x1C, cast_int_arith L63/L80)
struct UnkStruct_8009EC9C_Ret {
    u8 pad_00[0x1C];
    u8 unk1C;
};

// Layout for CBattleMan_NextListItem self: list sentinel ptr at +0x08
struct Func800EA384_Self {
    u8 pad_00[0x08];
    SimpleListNode* listHead;
};

// Layout for CBattleMan_MarkList48Flag40 this_: list sentinel ptr at +0x48
struct Func800F4004_Self {
    u8 pad_00[0x48];
    SimpleListNode* listHead;
};

// Object layout used by CBattleMan_FireActorEvent918 / CBattleMan_HandleMoveType45
// Has a field at +0x3f00 (flags), +0x3f10 (ptr), +0x3f28 (u16)
struct BattleObjAccessor {
    u8 pad_00[0x3f00];
    u32 field_3f00;     // flags at +0x3f00
    u8 pad_3f04[0x0C];  // 0x3f04-0x3f0F
    void* field_3f10;   // ptr at +0x3f10
    u8 pad_3f14[0x14];  // 0x3f14-0x3f27
    u16 field_3f28;     // u16 at +0x3f28
};

// Object layout used by CBattleMan_HandleMoveType45 for arg2 access at +0x78
// Has a field at +0x50 (ptr), +0x78 (flags)
struct BattleMoveObjAccessor {
    u8 pad_00[0x50];
    void* field_50;     // ptr at +0x50
    u8 pad_54[0x24];
    u32 field_78;       // flags at +0x78
};

// Move/params block for setupBattleMove (arg4): arts-data pointer at +0x50,
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

// Object layout used by CBattleMan_HandleMoveType45 for r31 (*(arg2+0x50))
// Has fields at +0x3c (u16 type), +0x40 (u16)
struct BattleSubObjAccessor {
    u8 pad_00[0x3c];
    u16 type_3c;        // type at +0x3c
    u16 field_3e;       // padding?
    u16 field_40;       // value at +0x40
};

// Target/arts object fields probed by evalBattleTarget: u16 type +0x3C,
// u16 id +0x48, flags word +0x78.
struct E1B5C_TargetObj {
    u8 pad_00[0x3c];
    u16 type_3c;        // +0x3C
    u8 pad_3e[0x48 - 0x3e];
    u16 id_48;          // +0x48
    u8 pad_4a[0x78 - 0x4a];
    u32 flags_78;       // +0x78
};

// Battle-actor / action / player view used by evalBattleTarget: status flags
// (+0x3374), vtable holder (+0x3ED4), flags (+0x3F00), ptr (+0x3F10),

// Flags-word view onto a move-data block (+0x78 only).
struct E1B5C_MoveFlags {
    u8 pad_00[0x78];
    u32 flags_78;
};

// Object layout for the removed-target access in FactoryEvent2 / unregisterBattleActor

// Enemy/status object view for setupBattleMove's arg3: status region base at
// +0x8 (isBattleStatusActive operates on &+0x8), flags at +0x3374, embedded move
// sub-object at +0x3E9C (vtable slot 0x140, dispatched as a real virtual
// on the owning move class).
struct DB4FC_EnemyObj {
    u8 pad_00[0x8];
    u8 statusBase;              // +0x8
    u8 pad_09[0x3374 - 0x9];
    u32 field_3374;             // +0x3374
    u8 pad_3378[0x3E9C - 0x3378];
    u8 moveSub;                 // +0x3E9C (vptr of the move sub-object)
};

// Arts/skill object (setupBattleMove arg2->artsData): byte at +0x44,
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
// probe used by CBattleMan_FindActorByKind lives at compiled vtable slot 0x4C (declared
// index 17 under -RTTI). Never instantiated here, so no vtable is emitted.
// Actor object scanned by CBattleMan_FindActorByKind: type id at +0x15F0 and the embedded

// Sentinel-pointer view (offset-typed alias of CBattleManager used to keep
// CBattleMan_FindActorByKind pass 2's sentinel load independent of pass 1's).
struct BMSentinelView {
    u8 pad00[0x48];
    _reslist_node<cf::CfObjectActor*>* sentinel;  // +0x48
};

// MapFx_ResolveOwner result view for CBattleMan_FindActorByKind: flags at +0x3F00 (bit 2 =

// Absolute-offset view of an actor's embedded move sub-object (the C++ base
// layout in CfObjectActor.hpp is 8 bytes off retail, so raw views pin the
// offset; cf. CfActorField3F00 in CfObjectActor.hpp).
struct ActorMoveSubView3E9C {
    u8 pad_00[0x3E9C];
    u8 sub3E9C;                 // +0x3E9C (move sub-object vptr)
};

// Source/arts object view for calcBattleDamage (the +0x50 deref of its source
// param): arts type at +0x3C, art type at +0x40, flags at +0x78.
struct BattleSrcObjAccessor {
    u8 pad_00[0x3c];
    u16 type_3c;                // +0x3C
    u16 field_3e;
    u16 field_40;               // +0x40
    u8 pad_42[0x78 - 0x42];
    u32 field_78;               // +0x78
};

// calcBattleDamage source-param view: linked arts/status object pointer at +0x50.
struct D81A8_SourceView {
    u8 pad_00[0x50];
    BattleSrcObjAccessor* link;   // +0x50
};

// Battle-status list base at actor +0x08 (member-address form keeps MWCC from
// CSE-ing the three probes in calcBattleDamage into a shared temp register).
struct D81A8_StatusListView {
    u8 pad_00[0x08];
    u8 statusList;   // +0x08
};

// Status/arts-data entry returned by findBattleStatusEntry (calcBattleDamage reads the
// s32 value at +0x10).
struct BDSessionEntry {
    u8 pad_00[0x10];
    s32 field_10;               // +0x10
};

// Battle-manager +0x20AC chain-gauge view used by calcBattleDamage.
struct CBattleManager_20AC {
    u8 pad_00[0x20AC];
    f32 field_20AC;             // +0x20AC
};

// 4th-arg block of dispatchBattleAI: link at +0x50, flags at +0x74/+0x78,
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

// Object layout for registerBattleActor's actor param: flags at +0x3374,
// vtable-holder at +0x3ED4 (dispatched as a real virtual on the owning


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
extern "C" const float lbl_eu_80666DDC;
extern "C" float lbl_eu_80666DD4;
extern "C" float lbl_eu_80666DD0;   // 0.6f (attack-sound volume)
extern "C" f64 lbl_eu_80666DE0;
extern "C" float lbl_eu_80666DEC;   // time/frame-delta scale (updateBattleManager)
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
extern "C" void CBattleMan_OnActorsEmpty(cf::CBattleManager* mgr);
extern "C" void CChain_NotifyStateChange(cf::CChain* chain, cf::CfObjectActor* actor);
extern "C" void CChain_ForwardVoiceClear(cf::CChain* chain);
extern "C" void setupBattleMove(void* self, void* obj, void* enemy, void* move);
extern "C" void handleBattleMoveEvent(void* self, void* obj, void* enemy, void* move);
extern "C" void CBattleMan_HandleMoveType45(void* self, void* obj, void* enemy, void* move);
extern "C" int CChain_HasMemberEntry(void* chain, void* obj);

// C++-mangled retail import (findObjectById__Fi): actor id -> action source.
void* findObjectById(int id);

// Object scanned by findFreeBattleSlot: key word at +0x4, flags at +0x78 and the
// 0x20-stride entry block pointer at +0xB8.
struct D7D24_Obj {
    u8 pad_00[0x04];
    u32 field_4;            // +0x04
    u8 pad_08[0x78 - 0x08];
    u32 field_78;           // +0x78 flags
    u8 pad_7C[0xB8 - 0x7C];
    u32* field_B8;          // +0xB8 entry block
};

// Battle-event slot 0xC (#1) dispatch shared copy lives in
// CBattleManagerApi.hpp.
// Move sub-object view for handleBattleMoveEvent (arg4->artsData): u16 type at
// +0x58 and the sub-vtable pointer at +0x84.
struct DB7F8_MoveSub {
    u8 pad_00[0x58];
    u16 type_58;                    // 0x58
    u8 pad_5A[0x84 - 0x5A];
    void** subVt84;                 // 0x84
};
