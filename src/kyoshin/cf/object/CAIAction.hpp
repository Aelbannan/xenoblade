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

// 0xC-byte AI action sub-entry (installed via func_8014B804).
struct CAIActionSubEntry {
    u8 b0;   // 0x00 -> func_8014B804 a2
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

// Party object referenced by CAIAction::unkB14 (fields used by func_8015396C).
struct CAIPartyInfo {
    u8 pad[0x3F00];
    u32 flags;  // 0x3F00 - bits 1/2 select the lookup id (0x70 / 0x65)
    u8 pad2[0x24];
    u16 artsId; // 0x3F28 - current arts id
};

// size: 0xB1C
class CAIAction {
public:
    CAIAction();
    ~CAIAction();

    // Declared Fv for vtable; body is extern "C" with outA/outB args
    virtual void CAIAction_UnkVirtualFunc1(); // 0x8
    virtual void CAIAction_UnkVirtualFunc2(); // 0xC

    // 0x0: vtable
    u32 unk4;          // 0x4
    u16 unk8;          // 0x8
    u8 padA[2];        // 0xA
    u8 slots[0x200];   // 0xC .. 0x20C (16 * 0x20)
    void* unk20C;      // 0x20C - pointer to slots
    u32 unk210;        // 0x210
    u32 unk214;        // 0x214
    u32 unk218;        // 0x218 - capacity (0x10)
    u8 entries[0x8C0]; // 0x21C .. 0xADC (160 * 0xE)
    u8 trailer[0x20];  // 0xADC .. 0xAFC (CAIActionSlot)
    u32 unkAFC;        // 0xAFC
    u8 unkB00;         // 0xB00
    u8 padB01[0xF];    // 0xB01
    u32 unkB10;        // 0xB10
    u32 unkB14;        // 0xB14
    u32 unkB18;        // 0xB18
};

} // namespace cf

extern void func_801537F0(cf::CAIAction* self);
extern int func_8014B120(cf::CAIAction* self, const cf::CAIActionSlot* in);
extern u32 func_8014AC38(cf::CAIAction* self, const cf::CAIActionSlot* in);

// Fv mangling, but callers leave outA in r4 and outB in r5
extern "C" void CAIAction_UnkVirtualFunc1__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* outA,
                                                              cf::CAIActionExport* outB);

// Inverse of UnkVirtualFunc1: imports trailer from inA, then imports ring
// entries from inB into this->unk20C. Same Fv mangling / r4-r5 ABI.
extern "C" void CAIAction_UnkVirtualFunc2__Q22cf9CAIActionFv(cf::CAIAction* self,
                                                              cf::CAIActionSlot* inA,
                                                              cf::CAIActionExport* inB);

extern void func_8014A86C(void*);
extern void func_8014A8F8();

// .sbss object at 0x806641B0 - pointer to the AI action config table
// (u16 entry count at +0, variable-length entries at +2). Walked by
// func_8015396C; cleared by func_8014A8F8.
extern cf::CAIActionTable* lbl_eu_806641B0;
extern void* func_800B708C(int);        // C++ linkage -> func_800B708C__Fi

// Raw vtable view: retail code invokes fixed slots through object vtables.
struct CAIVtable {
    void* slot[0x170]; // up to offset 0x5C0
};

// Embedded move object at partyBase + 0x3E9C.
struct CAIPartyMoveObj {
    CAIVtable* vtable;        // +0x00 (slots 0x4C / 0x50 used)
    u8 pad04[0x64 - 0x4];
    u32 moveFlags;            // +0x64 (abs 0x3F00; bits 1/2 select lookup id)
};

// Party/battle object stored in CAIAction::unkB14 (used by func_801537F0 /
// func_80150618).
// State sub-object at party+4: real object whose vptr sits at its own +0.
// Slot +0x30 returns a u32* whose first word is the probed tag.
class CAIPartyStateVt {
public:
    virtual void _d008(); virtual void _d00C(); virtual void _d010();
    virtual void _d014(); virtual void _d018(); virtual void _d01C();
    virtual void _d020(); virtual void _d024(); virtual void _d028();
    virtual void _d02C();
    virtual u32* vf30(); // vtable +0x30
};

struct CAIPartyObj {
    CAIVtable* vtable;        // 0x00 (slot 0x5C0 used)
    CAIPartyStateVt* unk04;   // 0x04 state sub-object (slot 0x30 used)
    u8 pad08[0x3388 - 0x8];
    u16 unk3388;              // 0x3388 status flag bits (0x8 / 0x10 tested)
    u8 pad338A[0x3E98 - 0x338A];
    u32 unk3E98;              // 0x3E98
    CAIPartyMoveObj move;     // 0x3E9C
    u32 unk3F10;              // 0x3F10 battle handle
    u8 pad3F14[0x3F28 - 0x3F14];
    u16 unk3F28;              // 0x3F28 current arts id
};

// Iterator object handed out by the func_80043F18 family; limit word at +0x620.
struct CAIEnumIter {
    u8 pad[0x620];
    u32 field620;             // 0x620 entry count / limit
};

// Arts-parameter record probed by the action gates.
struct CAIArtsParamView {
    u8 pad00[0x34];
    s16 field34;              // 0x34 (cost/level field, sign-extended)
    u8 pad36[0x48 - 0x36];
    u16 field48;              // 0x48 arts id
    u8 pad4A[0x77 - 0x4A];
    u8 field77;               // 0x77 category byte
    u32 field78;              // 0x78 flag word (bit 0x8000)
    u8 pad7C[0x80 - 0x7C];
    f32 field80;              // 0x80 power value
};

// Element row (func_80193AB0 result): s16 gate value at +0xA2.
struct CAIElemA4View {
    u8 pad00[0xA2];
    s16 fieldA2;
};

// Character-data object (func_800AD860 result): u16 id at +0x45C0.
struct CAIChDataView {
    u8 pad00[0x45C0];
    u16 field45C0;
};

// Battle manager singleton fields probed by func_8014CE78.
struct CBattleMgrAIView {
    u8 pad00[0x194];
    s32 field194;             // 0x194 turn/frame counter
    u8 pad198[0x824 - 0x198];
    u32 field824;             // 0x824 flag bits (bit 0x20000)
};

// Imports used by func_8014CE78 (unmangled retail symbols).
extern "C" long func_80174C98(void* p, void* out, u32 arg);
extern "C" int func_801541B0(void* party, u32 arg);
extern "C" int func_80153DCC(void* artsSet, int idx);
extern "C" int func_801B1FFC(int idx);
extern "C" u16 func_8016DF2C(void);   // canonical u16 form (chapter/episode getter)
extern "C" void* func_8009EC9C(u16 idx);
extern "C" u32 func_800A32BC(void* obj);
extern "C" u32 func_8009CF8C(u32 id);
extern "C" int func_80158018(u32 v);
extern "C" u32 getBdatStringColumnValue(void* table, const char* col, s32 index = 0); // canonical ocBdat.hpp form; default keeps 2-arg call sites compiling
extern "C" void* func_80193AB0(void* table, u32 id);
void* func_800AD860(void* obj);   // C++ linkage -> func_800AD860__FPv
extern "C" void* func_80193670(void);
extern "C" int func_80260264(void* self, int id, void* out);
extern "C" int func_80145C00(int id);   // canonical int param (CBattleState.cpp definition)
extern "C" int func_801554DC(void* param, void* actor, u32 flags);
extern "C" void* getArtsParamByIdx(void* artsSet, int idx);
extern "C" u16 func_80153CAC(const void* artsSet, int idx);
extern "C" u16 getArtsSlotAtCnt(void* artsSet, u32 idx);
extern "C" u16 getArtsSlotRC(void* artsSet, int a, int b);
extern "C" void* getArtsParamRC2(void* artsSet, int a, int b);
extern "C" void* getAtkParam(void* artsSet, int idx);
// BDAT select table + global bdat root (data imports).
extern u32 lbl_eu_806640F8;
extern u8 lbl_eu_80501968[];

// Sub-object referenced by CAIActionQuery::unk18.
struct CAIQueryTarget {
    u8 pad00[0x3C];
    u16 unk3C;                // 0x3C action-class tag (3/4 special-cased)
    u8 pad3E[0x5C - 0x3E];
    u16 unk5C;                // 0x5C dispatch selector (1/2/3)
};

// 0x20-byte query struct passed to func_80150828 / func_80150618.
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

// Stack-allocated 8-byte holder (list + handle).
struct CAIActionEnumHolder {
    void* list;               // 0x0
    u32 handle;               // 0x4
};

// Canonical (void*, int*, int) form - MUST match code_802B8A3C.hpp and the
// other ~30 TU-local declarations (divergent extern "C" type lists for one
// symbol break every TU whose include chain pulls two of them together with
// "illegal function overloading" 10197).
// Canonical family form (void*, int*, int) - MUST match CfObjectPc.hpp:77 /
// code_802B8A3C.hpp / the other ~30 TU-local declarations (divergent extern
// "C" type lists for one symbol break every TU whose include chain pulls two
// of them together with "illegal function overloading" 10197).

extern "C" void* func_8016FE34(void*);          // -> func_8016FE34
// NOTE (agent pi-019fef06, 2026-08-11): the two declarations below were
// changed from CAIActionEnumHolder* to void* to match the identical extern
// "C" declarations already in CVision.hpp (included alongside this header
// by CBattleManager.hpp). With both typed pointers visible in one TU, MWCC
// reports (10197) illegal function overloading, breaking every TU that
// includes both headers. ABI is unchanged (opaque pointer).
extern "C" void func_80043D90(void*);
extern "C" void __dt__80043E88(void*, int);
extern "C" void* func_80150828(cf::CAIAction*, CAIActionQuery*);
extern "C" void* func_80150618(cf::CAIAction*, CAIActionQuery*);
extern "C" int func_8014B8BC(void* a, void* b);
extern "C" void* func_801522C4(cf::CAIAction*, const void*);
extern "C" int func_8014CE78(cf::CAIAction*, const u8*, cf::CAIActionSlot*);
void func_8014B344(cf::CAIAction*, u32);

// CfObjEnumList helper family (retail names unmangled -> extern "C").
extern "C" void* func_80043F18(void*);
extern "C" void func_800F4A98(void*, u32, u32);
extern "C" void func_800F6D50(void*, u32);
extern "C" void* func_800F6E08(void*);
extern "C" void* func_800F6EAC(void*, u32);
extern "C" void* func_800F6EC0(void*, u32);
extern "C" void func_800F6ED0(void*, void*);
extern "C" void func_800F9AEC(void*);
extern "C" void __ct__800FBF08(void*, u32, u32);
extern "C" void __ct__800FC040(void*, u32, u32, u32);
extern "C" void __ct__800FC19C(void*, u32, u32);
extern "C" void __ct__800FC4FC(void*, u32, f32);
extern "C" void __ct__800FC648(void*, u32, f32);
extern "C" void __ct__800FC790(void*, u32, f32);
extern "C" void __ct__800FC8DC(void*, u32, f32);
extern "C" void __ct__800FCA24(void*, void*, u32);
extern "C" void __ct__800FCB80(void*, void*, u32);
extern "C" void __ct__800FCD10(void*, void*, u32);
extern "C" void __ct__800FCF1C(void*, u32, u32);
extern "C" void __ct__800FD250(void*);
extern "C" void* func_800B76A4(void*);
extern "C" void* func_800B77E4(void*, void*);
extern "C" void* func_800B8B94(s32);
extern void* func_800B07E8();   // C++ linkage -> func_800B07E8__Fv
extern "C" int func_80148778(void*, int);
extern "C" void* func_80149154(void*, u32);
extern "C" void* func_800EA444(void*);
extern "C" void* getPlayer__Q22cf13CfGameManagerFi(int);
#include "kyoshin/cf/CBattleManagerApi.hpp"
extern "C" int rand(void);
extern "C" f32 func_801C37CC(void*, void*);
extern "C" int func_801C3850(void*, void*);
