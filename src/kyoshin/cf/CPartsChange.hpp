#pragma once

#include <types.h>

// Resource getter (retail unmangled symbol; defined in kyoshin/CMiniMap.cpp).
extern "C" u32 func_8009CF8C(u32 resourceId);

// BDAT table column accessor (retail unmangled C-ABI import; func_80195384).
extern "C" u32 getBdatStringColumnValue(void* bdat, const char* column, int index);

// BDAT table pointer + column-name blob read by func_80195384.
extern void* lbl_eu_806640CC;
extern char lbl_eu_80503C48[];

// BDAT table pointer + column index read by func_801931D0.
extern void* lbl_eu_806640A8;
extern u32 lbl_eu_80664184;

// Party-change helpers used by func_801952CC (retail unmangled C-ABI
// imports from other TUs; C linkage so the call relocs name the retail
// symbols).
extern "C" void func_8009D018(u32 owner, u32 flag);
extern "C" void func_802918AC(int val);
extern "C" int func_80291C60(int v);
extern "C" void* func_8009EC9C(u16 index);
extern "C" void func_800A282C(void* obj, int flag);
extern "C" void func_802761C0(void* self);
// CfGameManager statics (retail mangled names used literally - MWCC does not
// re-mangle identifiers that already carry a type suffix).
u32 func_8008235C__Q22cf13CfGameManagerFv(u32 id);
void* getPlayer__Q22cf13CfGameManagerFi(int index);
// Global event/presentation flag word (.sbss); bit 0x200000 gates re-arming.
extern u32 lbl_eu_80663E24;

// Retail CPartsChange vtable (.data:0x80532AA8, split1). The retail ctor
// symbol is the splitter-renamed `__ct__cf_CPartsChange` (not MWCC's
// __ct__Q22cf12CPartsChangeFv), so the ctor is written as a C-style function
// that stores this label explicitly - the reloc must name lbl_eu_80532AA8 to
// match the retail ctor's reloc (compiler __vt__ symbol would drift).
extern u8 lbl_eu_80532AA8[];

// Retail _reslist_base<cf::CfPartyInfo> vtable (.data) stored by the dtor
// (same explicit-label treatment as the ctor above).
extern u8 lbl_eu_80532AE4[];

namespace cf {

class CPartsChange {
public:
    virtual ~CPartsChange();

    /* 0x04 */ u8 mField04;
    /* 0x08 */ u32 mField08;
    /* 0x0C */ u8 mData[0x20];
    /* 0x2C */ u8 mField2C;
};

// Struct filled by func_80198710 (likely CfPartyInfo)
struct CfPartyInfo {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
    /* 0x0C */ s32 field_0C;
    /* 0x10 */ u32 field_10;
    /* 0x14 */ s32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ f32 field_1C;
    /* 0x20 */ f32 field_20;
    /* 0x24 */ u32 field_24;
    /* 0x28 */ f32 field_28;
    /* 0x2C */ u8 field_2C;
    /* 0x2D */ u8 field_2D;
    /* 0x2E */ u8 field_2E;

    void func_80195AFC(u8 val);
    void func_80198710(void* r4, float f1, int r5, int r6, float f2, float f3);
};

// Comparator key struct for func_8019641C
struct CfPartyInfoSortKey {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ f32 sortKey;
};

// Wrapper for CActorParam fields accessed by CPartsChange accessors
struct CfActorAccessors {
    u8 pad_00[0x1E];
    /* 0x1E */ u16 mFlags1E;
    u8 pad_20[0x10];
    /* 0x30 */ void* mField30;
    u8 pad_34[0x58];
    /* 0x8C */ f32 mField8C;
    u8 pad_90[4];
    /* 0x94 */ u32 mField94;
    u8 pad_98[6];
    /* 0x9E */ u16 mField9E;

    void func_80193A88(int enable);
    u32 func_80193B04();
    u16 func_80193CC8();
    u32 func_80195284();
    u32 func_80195AC0();
    void func_80195ACC(float val);
    void* func_80195AEC();
};

// Wrapper for CfObjectPc fields at 0x45C4+
struct CfObjectPcExt {
    u8 pad[0x45C4];
    /* 0x45C4 */ u16 mField45C4;
    /* 0x45C6 */ u16 mField45C6;
    /* 0x45C8 */ u16 mField45C8;

    void func_80195AD4(u16 val);
    void func_80195ADC(u16 val);
    u16 func_80195AE4();
    void* func_80195AF4();
};

} // namespace cf

// 12-byte id/state triple shared by the CPartsChange helpers: copied by
// func_80198AC4; the first word is resolved as an actor id by func_80198310.
// (Global scope - the retail helper symbols are unmangled free functions
// defined at global scope in CPartsChange.cpp.)
struct CfPartsTri {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
};

// 8-byte-stride list entries (func_801984E4 / func_801984F0); the first word
// is an actor id fed to func_800B708C.
struct CfPartsListEntry {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
};

// View over a single actor-id word at +0x00 (func_80198310).
struct CfActorIdRef {
    /* 0x00 */ u32 field_00;
};

// View over the +0x1D44 resource slot queried by func_80195290: the slot
// address itself is passed to func_8009CF8C (no load of its contents).
struct CfResSlot {
    u8 pad_00[0x1D44];
    /* 0x1D44 */ u8 field_1D44;
};

// 0x4C-byte element of the array scanned by func_8019747C (u16 id at +0x1C).
// Also carries the speed/flags fields touched by func_80193678 / func_801931D0
// and the 9-byte sub-blocks at +0x30 and +0x42.
struct CfPartsElem4C {
    u8 pad_00[0x10];
    /* 0x10 */ f32 field_10;
    /* 0x14 */ f32 field_14;
    u8 pad_18[4];
    /* 0x1C */ u16 field_1C;
    /* 0x1E */ u16 field_1E;
    u8 pad_20[2];
    /* 0x22 */ u16 field_22;
    u8 pad_24[0x30 - 0x24];
    /* 0x30 */ u8 field_30[9];
    u8 pad_39[0x42 - 0x39];
    /* 0x42 */ u8 field_42[9];
    u8 pad_4B[0x4C - 0x4B];
};

// Array view for func_8019747C: 0x4C-stride elements from +0x00, count at
// +0x9800 (0x200 * 0x4C = 0x9800, so mElems ends exactly at the count).
struct CfPartsElemArray {
    /* 0x00 */ CfPartsElem4C mElems[0x200];
    /* 0x9800 */ u32 mCount;
};

// 0xA4-byte element of the 16-entry table scanned by func_80193AB0; u32
// search key at +0x94, actor id at +0x00 (func_80193CD0 list elements).
// The +0x9E u16 is the flag read through func_80193CC8 by func_80197AA0.
struct CfElemA4 {
    /* 0x00 */ u32 field_00;
    u8 pad_04[0x90];
    /* 0x94 */ u32 field_94;
    u8 pad_98[6];
    /* 0x9E */ u16 field_9E;
    u8 pad_A0[0xA4 - 0xA0];
};

// Flags/state words read by func_801974CC from its second argument
// (bit 2 of +0x3F00 gates the scan; +0x456C supplies the shifted id).
struct CfPartsFlagView {
    u8 pad_00[0x3F00];
    /* 0x3F00 */ u32 field_3F00;
    u8 pad_3F04[0x456C - 0x3F04];
    /* 0x456C */ u16 field_456C;
};

// u16 state word at +0x45C0 read by func_80193CD0 (zero means "no lookup").
struct CfPartsIdView {
    u8 pad_00[0x45C0];
    /* 0x45C0 */ u16 field_45C0;
};

// Node of the singly-linked list walked by func_80193CD0: next link at +0x00,
// element pointer at +0x08; the terminal node links back to the head node.
struct CfPartsNode {
    /* 0x00 */ CfPartsNode* next;
    u8 pad_04[4];
    /* 0x08 */ CfElemA4* data;
};

// Container whose list head node lives at +0xA80C (func_80193CD0).
struct CfPartsListA80C {
    u8 pad_00[0xA80C];
    /* 0xA80C */ CfPartsNode* head;
};

// Table view for func_80193AB0: 16 elements of stride 0xA4 at +0xA828.
struct CfTableA4 {
    u8 pad_00[0xA828];
    /* 0xA828 */ CfElemA4 mElems[16];
};

// Init block written by func_80193C74 (memset 0x80, then explicit fields in
// retail store order).
struct CfPartState90 {
    u8 pad_00[0x90];
    /* 0x90 */ f32 field_90;
    /* 0x94 */ u32 field_94;
    /* 0x98 */ u16 field_98;
    /* 0x9A */ u16 field_9A;
    u8 pad_9C[4];
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ u16 field_A2;
};

// Float constant written by func_80193C74 to +0x90 (.sdata2, r2-sda21).
// `const` so MWCC treats the load as a constant and hoists it (cf.
// MWCC_REFERENCE.md `extern const float` hoist note).
extern const f32 lbl_eu_80667AD4;

// Rotation scale read by func_80198D44 (.sdata2, FIdx degrees conversion).
extern const f32 lbl_eu_80667B50;
// s32->f32 conversion magic double (0x4330000080000000) for func_80198D44's
// `(f32)(s32)` cast. Defined in CPartsChange.cpp so MWCC's constant pool
// reuses this named .sdata2 symbol instead of a TU-local @N label
// (CfResReloadImpl.cpp / CMenuArtsSelect.cpp idiom).
extern const f64 lbl_eu_80667B58;

// Speed-scale constants read by func_801931D0 (.sdata2).
extern const f32 lbl_eu_80667AB8;
extern const f32 lbl_eu_80667ABC;
extern const f32 lbl_eu_80667AC0;
// 2^52 (0x4330000000000000) u32->double conversion magic (.sdata2, lfd).
// Defined in CPartsChange.cpp so the builtin (f32) cast's constant pool
// reuses this entry (retail reloc name) instead of a TU-local @N label.
extern const double lbl_eu_80667AC8;

// Polymorphic receiver for the party-info state pmf table (lbl_eu_80532AF0).
// Abstract so MWCC emits no vtable; the virtuals force 12-byte
// pointer-to-member records (retail `mulli r0,r0,0xc`). The conceptual vptr
// occupies +0x00, so field_0C lands at byte +0x0C (CfPartyInfo::field_0C).
class CfPartyInfoState {
public:
    virtual void _vf00() = 0;
    virtual void _vf04() = 0;
    virtual void _vf08() = 0;
    virtual void _vf0C() = 0;
    virtual void _vf10() = 0;
    virtual void _vf14() = 0;
    virtual void _vf18() = 0;
    virtual void _vf1C() = 0;
    u8 pad_04[0x8];          // 0x04..0x0B (conceptual vptr + pad)
    /* 0x0C */ u32 field_0C; // state index (CfPartyInfo::field_0C)
    virtual void _vf20() = 0;
    virtual void _vf24() = 0;
};

// Party-info state-dispatch table (.data): 12-byte pmf records indexed by the
// +0x0C state word. MWCC lowers (self->*table[idx])(arg) to lis/addi + mulli
// + `bl __ptmf_scall`.
typedef int (CfPartyInfoState::*CfPartyInfoPMF)(CfPartsTri*);
extern CfPartyInfoPMF lbl_eu_80532AF0[];

// Party-info processor tail-called by func_80198AC4 (retail `b`) and called
// by func_80198D44 (retail `bl`). The retail symbol is unmangled, so C
// linkage is required for the call relocs to name func_801987A4.
extern "C" void func_801987A4(CfPartsTri* src, CfPartsTri* dst);

// 0xC-byte reslist node (4-byte item) of the retail
// _reslist_base<cf::CfPartyInfo> instantiation.
struct CfPartyListNode {
    /* 0x00 */ CfPartyListNode* mNext;
    /* 0x04 */ CfPartyListNode* mPrev;
    /* 0x08 */ u32 field_08;
};

// Mirror of _reslist_base<cf::CfPartyInfo> (retail instantiation): the
// sentinel node at +0x08 is 0xC bytes, so mList lands at +0x14 and the
// owns-flag at +0x1C - the reslist.hpp template would embed a full 0x38-byte
// node and push these to +0x40/+0x48.
struct CfPartyListBase {
    /* 0x00 */ void* vtable;
    /* 0x04 */ CfPartyListNode* mStartNodePtr;
    /* 0x08 */ CfPartyListNode mStartNode;
    /* 0x14 */ CfPartyListNode* mList;
    /* 0x18 */ int mCapacity;
    /* 0x1C */ u8 mOwnsList;
};

// Big manager object (func_80197AA0 / func_80193678): 0x4C-element array
// with count at +0x9800, the party-info reslist at +0xA808, and the
// 16-entry 0xA4 table at +0xA828.
struct CfPartsManager {
    /* 0x0000 */ CfPartsElemArray mElems;
    u8 pad_9804[0xA808 - 0x9804];
    /* 0xA808 */ CfPartyListBase mPartyList;
    /* 0xA828 */ CfElemA4 mTable[16];
};

// View of func_80197BA4's self object (a CfObjectPc): flags word at +0x3F08
// (bit 0x08000000 gates the re-arm) and the u16 party id at +0x45C8.
struct CfPartsChangeObj {
    u8 pad_00[0x3F08];
    /* 0x3F08 */ u32 field_3F08;
    u8 pad_3F0C[0x45C8 - 0x3F0C];
    /* 0x45C8 */ u16 field_45C8;
};

// Function-pointer view of the CfPartsChangeObj primary vtable (offset +0x00),
// slot +0x2BC: returns a nonzero busy flag. Same pattern as CfActorVt5C4Table
// (CfObjectActor.hpp) - avoids a 176-slot virtual proxy.
typedef int (*CfPartsVt2BCFn)(void* self);
struct CfPartsVtPtr {
    u32* vt;  // 0x00
};
struct CfPartsVt2BCTable {
    u32 _pad[0x2BC / 4];
    CfPartsVt2BCFn fn2BC;  // 0x2BC
};

// Player object returned by getPlayer__Q22cf13CfGameManagerFi: +0x74 holds
// the CMenuLvUp-ish object passed to func_802761C0 (func_801952CC).
struct CfGamePlayerView {
    u8 pad_00[0x74];
    /* 0x74 */ void* field_74;
};

// Layout for func_80198D44's source triple: 12 copied bytes, then the s32
// turn index at +0x14 and the f32 speed scale at +0x18/+0x1C.
struct CfPartsMoveSrc {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u32 field_04;
    /* 0x08 */ u32 field_08;
    u8 pad_0C[0x14 - 0x0C];
    /* 0x14 */ s32 field_14;
    /* 0x18 */ f32 field_18;
    /* 0x1C */ f32 field_1C;
};

// 16 8-byte slots (+0x00) plus the +0x94/+0xA0/+0xA2 state tail and the
// +0x3F10 actor id scanned by func_80198284. Both function params use this
// type so MWCC cannot prove no-aliasing and re-loads field_3F10 inside the
// loop (retail lwz-per-iteration).
struct CfPartsSlots {
    /* 0x00 */ CfPartsListEntry mEntries[16];
    u8 pad_80[0x14];
    /* 0x94 */ u32 field_94;
    /* 0x98 */ u16 field_98;
    /* 0x9A */ u16 field_9A;
    u8 pad_9C[4];
    /* 0xA0 */ u16 field_A0;
    /* 0xA2 */ s16 field_A2;
    u8 pad_A4[0x3F10 - 0xA4];
    /* 0x3F10 */ u32 field_3F10;
};
