#pragma once

#include <types.h>

namespace cf {

// 0x20-byte AI action slot (slots[] / trailer / export buffer)
struct CAIActionSlot {
    u32 unk00; // 0x00
    u32 unk04; // 0x04
    u32 unk08; // 0x08
    u32 unk0C; // 0x0C
    u16 unk10; // 0x10
    s16 unk12; // 0x12
    f32 unk14; // 0x14
    u32 unk18; // 0x18
    u32 unk1C; // 0x1C
};

// symbols.txt mangles Fv; retail passes out buffers in r4/r5
struct CAIActionExport {
    u8 pad[0x200];
    CAIActionSlot* buffer; // 0x200
    u32 unk204;            // 0x204
    u32 unk208;            // 0x208
    u32 unk20C;            // 0x20C - column stride
};

// 0xC-byte AI action sub-entry (installed via aiActionStoreIndexedBytes).
struct CAIActionSubEntry {
    u8 b0;   // 0x00 -> aiActionStoreIndexedBytes a2
    u8 b1;   // 0x01 (unused)
    u8 b2;   // 0x02 -> a3
    u8 b3;   // 0x03 -> a4
    u8 b4;   // 0x04 -> a6
    u8 b5;   // 0x05 -> a5
    u8 b6;   // 0x06 -> a7
    u8 b7;   // 0x07 -> a8
    u8 b8;   // 0x08 -> a9
    u8 b9;   // 0x09 -> a10
    u8 b10;  // 0x0A -> a11
    u8 b11;  // 0x0B -> a12
};

// One 0xE-byte action record in CAIAction::entries (12 action bytes plus the
// flag halfword the store helper maintains). Declaring the array with its real
// element type removes the offset overlay otherwise used for the flag word.
struct CAIActionEntry {
    CAIActionSubEntry action; // 0x00 .. 0x0B
    u16 flags;                // 0x0C
};

// Variable-length AI config table entry (stride 0x16 + actionCount * 0xC).
struct CAIActionTableEntry {
    u8 id;                   // 0x00 - selection id
    u8 field_01;             // 0x01
    u16 artsId;              // 0x02 - stored byte-swapped; matched vs party
                             //        unkB14->unk3F28 (or 0)
    char name[0x10];         // 0x04 - art name
    u8 actionCount;          // 0x14
    u8 field_15;             // 0x15
    CAIActionSubEntry actions[1]; // 0x16 - variable
};

// AI config table header (*lbl_eu_806641B0).
struct CAIActionTable {
    u16 count;                     // 0x00
    CAIActionTableEntry entries[1]; // 0x02 - variable
};

// Party object referenced by CAIAction::unkB14 (fields used by aiInstallArts).
struct CAIPartyInfo {
    u8 pad[0x3F00];
    u32 flags;  // 0x3F00 - bits 1/2 select the lookup id (0x70 / 0x65)
    u8 pad2[0x24];
    u16 artsId; // 0x3F28 - current arts id
};

// Fixed 16-slot queue embedded in CAIAction (+0xC): 0x200 bytes of slots, a
// pointer to them, then head / count / capacity. Its destructor clears the
// count and head words; retail keeps the out-of-line weak copy in
// CfObjectEne.cpp (and CVision.cpp destroys another instance through it).
struct CAIActionSlotQueue {
    ~CAIActionSlotQueue() {
        count = 0;
        head = 0;
    }

    u8 slots[0x200]; // 0x000 (16 * 0x20)
    void* buffer;    // 0x200 -> slots
    u32 head;        // 0x204
    u32 count;       // 0x208
    u32 capacity;    // 0x20C (0x10)
};

// Ring header at CAIAction+4 that owns the slot queue (implicit destructor).
struct CAIActionRing {
    u32 unk0;                 // 0x0 
    u16 unk4;                 // 0x4 
    CAIActionSlotQueue queue; // 0x8
};

// Retail __vt__Q22cf9CAIAction (US symbols.txt: lbl_eu_8052F598, size 0x10,
// in CItem TU .data): RTTI at +0x00 (__RTTI__Q22cf9CAIAction), 0 at +0x04, then
// the two virtuals. This TU emits no __vt__ (novtable); the ctor writes
// the label explicitly, same pattern as CHelp / CToken.
struct CAIActionVtbl {
    void* rtti;  // +0x00
    u32 zero;    // +0x04
    void* slot8; // +0x08 CAIAction_UnkVirtualFunc1
    void* slotC; // +0x0C CAIAction_UnkVirtualFunc2
};

// size: 0xB1C
class __declspec(novtable) CAIAction {
public:
    CAIAction();
    ~CAIAction();

    // Overlay on the vptr at +0 so the ctor can install the retail table.
    CAIActionVtbl*& vtbl() {
        return *reinterpret_cast<CAIActionVtbl**>(this);
    }

    // 0x8: export the trailer slot to outA and the ring entries to outB.
    virtual void CAIAction_UnkVirtualFunc1(CAIActionSlot* outA, CAIActionExport* outB);
    // 0xC: inverse of 0x8 (import trailer from inA, ring entries from inB).
    virtual void CAIAction_UnkVirtualFunc2(CAIActionSlot* inA, CAIActionExport* inB);
    void CAIAction_setOwner(u32 owner);

    // 0x0: vtable
    CAIActionRing ring; // 0x4 .. 0x21C (slot queue at 0xC)
    CAIActionEntry entries[160]; // 0x21C .. 0xADC (160 * 0xE)
    u8 trailer[0x20];  // 0xADC .. 0xAFC (CAIActionSlot)
    u32 unkAFC;        // 0xAFC
    u8 unkB00;         // 0xB00
    u8 padB01[0xF];    // 0xB01
    u32 unkB10;        // 0xB10
    u32 unkB14;        // 0xB14
    u32 unkB18;        // 0xB18
};

// Inline in retail (weak copies in CfObjectEne.cpp and wherever else a
// CAIAction owner is destroyed): the body is the inlined destruction of the
// ring member and its slot queue.
inline CAIAction::~CAIAction() {}

} // namespace cf

// US symbols.txt name for __vt__Q22cf9CAIAction. Plain extern (not mangled),
// same pattern as CHelp.hpp's lbl_eu_8053B3A0.
extern cf::CAIActionVtbl lbl_eu_8052F598;

// (The former C++-linkage forward decls of aiRebootAction / aiAppendToRing /
// aiRingPrepend were removed: they overloaded the extern "C" forms on
// CVision.hpp / chain/CChain.hpp (MWCC 10197) in every TU that includes
// both headers. The definitions in CAIAction.cpp are marked extern "C"
// directly; callers use the canonical extern "C" header decls.)

extern void initAIActionTable(void*);
extern void clearAIActionTable();

// .sbss object at 0x806641B0 - pointer to the AI action config table
// (u16 entry count at +0, variable-length entries at +2). Walked by
// aiInstallArts; cleared by clearAIActionTable.
extern cf::CAIActionTable* lbl_eu_806641B0;
extern void* findObjectById(int);        // C++ linkage -> findObjectById__Fi

// Forward decl for real state sub-object (owner: CObjectState, slot 0x30).
namespace cf { class CObjectState; struct CBattleStateEntry; }

// Embedded move object at partyBase + 0x3E9C (real owner: CObjectParam/CfObjectMove, slots 0x4C/0x50).
struct CAIPartyMoveObj {
    void* vtable;        // +0x00 (slots 0x4C / 0x50 used - real CObjectParam)
    u8 pad04[0x64 - 0x4];
    u32 moveFlags;            // +0x64 (abs 0x3F00; bits 1/2 select lookup id)
};

// Party/battle object stored in CAIAction::unkB14 (used by aiRebootAction /
// aiTargetQuery). Real class is CfObjectActor/CfObjectPc/CActorParam; state sub-object at +4 is CObjectState (slot 0x30 = getStateData).
struct CAIPartyObj {
    void* vtable;        // 0x00 (slot 0x5C0 used - real CfObjectActor_sharesMoveFlags)
    cf::CObjectState* unk04;   // 0x04 state sub-object (slot 0x30 = getStateData)
    u8 pad08[0x3388 - 0x8];
    u16 unk3388;              // 0x3388 status flag bits (0x8 / 0x10 tested)
    u8 pad338A[0x3E98 - 0x338A];
    u32 unk3E98;              // 0x3E98
    CAIPartyMoveObj move;     // 0x3E9C
    u32 unk3F10;              // 0x3F10 battle handle
    u8 pad3F14[0x3F28 - 0x3F14];
    u16 unk3F28;              // 0x3F28 current arts id
};

// Iterator object handed out by the get__16CfEnumListHolderFv family; limit word at +0x620.
struct CAIEnumIter {
    u8 pad[0x620];
    u32 field620;             // 0x620 entry count / limit
};

// Element row
struct CAIElemA4View {
    u8 pad00[0xA2];
    s16 fieldA2;
};

// Character-data object (getEffOwner result): u16 id at +0x45C0.
struct CAIChDataView {
    u8 pad00[0x45C0];
    u16 field45C0;
};

// Imports used by aiJudgeAction
extern "C" long testActorStatus(void* p, void* out, u32 arg);
extern "C" int canAIUseArts(void* party, u32 arg);
extern "C" int lookupArtsParamById(void* artsSet, int idx);
extern "C" int isPcEffectSlotActive(int idx);
extern "C" u16 getReloadParam0(void);   // canonical u16 form (chapter/episode getter)
extern "C" u32 CtrlObjectParam_GetCurrentRowKey(void* obj);
#include "kyoshin/cf/CtrlRemoteApi.hpp"
extern "C" int CItem_getNameIdFromFam(u32 v);
extern "C" u32 getBdatStringColumnValue(void* table, const char* col, s32 index = 0); // canonical ocBdat.hpp form; default keeps 2-arg call sites compiling
extern "C" void* CPartsChange_FindActorById(void* table, u32 id);
void* getEffOwner(void* obj);   // C++ linkage -> getEffOwner____FPv
extern "C" void* CPartsChange_GetActorTable(void);
extern "C" int IdTable_GetSum(void* self, int id, void* out);
extern "C" int isBattleEventKind3(int id);   // canonical int param (CBattleState.cpp definition)
// Canonical declarations for the two CBattleState.cpp helpers (CBattleManager and
// CActorParam pass the CBattleState sub-object as an opaque pointer).
extern "C" cf::CBattleStateEntry* matchBattleStatusEntry(void* self, u32 id, u32 a, u32 b, u32 c);
extern "C" bool isBattleStatusKind1(int value);
extern "C" int ArtsGateClear(void* param, void* actor, u32 flags);
// (getArtsSlotRC / getArtsParamRC2 moved to their single owner decl on
// kyoshin/cf/chain/CChainActorList.hpp - int/const void* forms matching the
// CActorParam.cpp definitions; this header's u16/void* copies are gone.)
extern "C" void* getArtsParamByIdx(void* artsSet, int idx);
extern "C" u16 getArtsSlotByFlatIdx(const void* artsSet, int idx);
extern "C" u16 getArtsSlotAtCnt(void* artsSet, u32 idx);
extern "C" void* getAtkParam(void* artsSet, int idx);
// BDAT select table + global bdat root (data imports).
extern u8 lbl_eu_80501968[];

// Sub-object referenced by CAIActionQuery::unk18.
struct CAIQueryTarget {
    u8 pad00[0x3C];
    u16 unk3C;                // 0x3C action-class tag (3/4 special-cased)
    u8 pad3E[0x5C - 0x3E];
    u16 unk5C;                // 0x5C dispatch selector (1/2/3)
};

// 0x20-byte query struct passed to aiQueryHandle / aiTargetQuery.
struct CAIActionQuery {
    u32 unk00;                // 0x00
    u8 unk04;                 // 0x04
    u8 unk05;                 // 0x05 (2nd dispatch key 0..178)
    u8 unk06;                 // 0x06 (1st dispatch key; 0x25 short-circuit)
    u8 unk07;                 // 0x07
    u32 unk08;                // 0x08
    u8 unk0C;                 // 0x0C
    u8 unk0D;                 // 0x0D (mapped via (b+0xCB)&0xFF / ==0x3B)
    u8 unk0E;                 // 0x0E
    u8 unk0F;                 // 0x0F
    u16 unk10;                // 0x10 flag bits (0x400 set pre-dispatch)
    s16 unk12;                // 0x12
    f32 unk14;                // 0x14
    CAIQueryTarget* unk18;    // 0x18 dispatch target
    u32 unk1C;                // 0x1C set to 6 when result is null
};

// Canonical (void*, int*, int) form - MUST match code_802B8A3C.hpp and the
// other ~30 TU-local declarations (divergent extern "C" type lists for one
// symbol break every TU whose include chain pulls two of them together with
// "illegal function overloading" 10197).
// Canonical family form (void*, int*, int) - MUST match CfObjectPc.hpp:77 /
// code_802B8A3C.hpp / the other ~30 TU-local declarations (divergent extern
// "C" type lists for one symbol break every TU whose include chain pulls two
// of them together with "illegal function overloading" 10197).

extern "C" void* MapFx_ResolveOwner(void*);          // -> MapFx_ResolveOwner
// NOTE (agent pi-019fef06, 2026-08-11): the two declarations below were
// changed from CAIActionEnumHolder* to void* to match the identical extern
// "C" declarations already in CVision.hpp (included alongside this header
// by CBattleManager.hpp). With both typed pointers visible in one TU, MWCC
// reports (10197) illegal function overloading, breaking every TU that
// includes both headers. ABI is unchanged (opaque pointer).
extern "C" void* aiQueryHandle(cf::CAIAction*, CAIActionQuery*);
extern "C" void* aiTargetQuery(cf::CAIAction*, CAIActionQuery*);
extern "C" int aiFetchAction(void* a, void* b);
extern "C" void* aiDispatchAct(cf::CAIAction*, const void*);
extern "C" int aiJudgeAction(cf::CAIAction*, const u8*, cf::CAIActionSlot*);
void aiRingConsume(cf::CAIAction*, u32);

// CfObjEnumList helper family (retail names unmangled -> extern "C").
extern "C" void startEnumObjects(void*, u32, u32);
extern "C" void appendObjectById(void*, u32);
extern "C" void* findFirstCleanObjectId(void*);
extern "C" void* getObjectAt(void*, u32);
extern "C" void* getEntryAt(void*, u32);
extern "C" void sortEntriesByDistance(void*, void*);
extern "C" void resortByActorValue(void*);
extern "C" void __ct__800FC040(void*, u32, u32, u32);
extern "C" void __ct__800FC19C(void*, u32, u32);
extern "C" void __ct__800FC648(void*, u32, f32);
extern "C" void __ct__800FC790(void*, u32, f32);
extern "C" void __ct__800FC8DC(void*, u32, f32);
extern "C" void __ct__800FCA24(void*, void*, u32);
extern "C" void __ct__800FCB80(void*, void*, u32);
extern "C" void __ct__800FCD10(void*, void*, u32);
extern "C" void __ct__800FD250(void*);
extern "C" void* firstReslistB28(void*);
extern "C" void* nextReslistB28(void*, void*);
extern "C" void* findObjB28ById(s32);

extern "C" void* findBattleStatusEntry(void*, u32);

#include "kyoshin/cf/CBattleManager.hpp"
extern "C" int rand(void);
extern "C" f32 CREvtLight_DistToTarget(void*, void*);
extern "C" int CREvtLight_AngleState(void*, void*);

// C++-linkage helpers owned by kyoshin/cf/object/CAIAction.
void* aiActionClearBlockADC(void* p);
void aiActionUpdateEntriesDelta(void* self, float delta);
void aiActionClearBits0006(void* self);
