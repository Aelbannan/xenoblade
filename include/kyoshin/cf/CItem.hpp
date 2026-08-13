#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 func_8009CF8C(u32 resourceId);

// BDAT column lookup (retail C-linkage name; declared here like
// CfObjectPoint.hpp - ocBdat.hpp intentionally does not promote it).
extern "C" u32 getBdatStringColumnValue(void*, const char*, int);

// BDAT row base/count helpers (used by func_8015B25C's item-box scan).
extern "C" u32 func_8003B41C(void* bdat);
extern "C" u32 func_8003B1EC(void* bdat);

// Item-box post-processing helper (used by func_8015B25C).
extern "C" void func_8013E424(void* self, int a);

// Variadic format helper (MWCC emits the crclr cr1eq varargs marker before
// the call; C linkage keeps the call-site reloc on the plain name).
extern "C" int sprintf(char*, const char*, ...);

// Global BDAT handles used by func_801558B4's lookups.
extern void* lbl_eu_806640D8;
extern void* lbl_eu_806640EC;

// BDAT handle used by func_8015B130's item-box creation lookups.
extern void* lbl_eu_806640CC;

// BDAT handle used by func_8015B25C's item-box creation lookups.
extern void* lbl_eu_806640E8;

// BDAT handle used by func_8015A930's item-roll loop.
extern void* lbl_eu_8066414C;

// BDAT handles selected by func_80155A00's category dispatch (cat==2 picks
// lbl_eu_806640F4, cat in [4,8] picks lbl_eu_806640F8).
extern void* lbl_eu_806640F4;
extern void* lbl_eu_806640F8;

// Item-name string pool (also the +0x42 column-name argument in func_801558B4).
extern char lbl_eu_80501C58[];

// Global item block base pointer (allocated once; freed by __dt__80157150).
extern char* lbl_eu_806641B8;

// Per-category name tables indexed by (itemWord >> 7) & 7.
extern const char* lbl_eu_8052F628[8];
extern const char* lbl_eu_8052F644[8];
extern const char* lbl_eu_8052F660[8];

// 4-byte .sdata column-name pointer globals (retail reads them with a plain
// lwz sym@sda21(r0) - d-form r0 is the literal-zero base, not an index).
extern const char* lbl_eu_80662288;
extern const char* lbl_eu_8066228C;

// Column-name string pointers mutated by func_8015A7FC: the last character
// of each pointed-to string is replaced with '1'..'8' per loop iteration.
extern char* lbl_eu_806622D8;
extern char* lbl_eu_806622DC;

// Column-name string pointers mutated by func_8015A3CC / func_8015A6AC: the
// last character of each pointed-to string is replaced with '1'..'8' per
// loop iteration (same pattern as the func_8015A7FC pair).
extern char* lbl_eu_806622BC;
extern char* lbl_eu_806622C0;
extern char* lbl_eu_806622D0;
extern char* lbl_eu_806622D4;

// Column-name string pointers mutated by func_8015A930: the last character
// of each pointed-to string is replaced with '1'..'4' per loop iteration.
extern char* lbl_eu_806622E0;
extern char* lbl_eu_806622E4;

// Lazy strlen-1 cache + init flag for the func_8015A3CC / func_8015A6AC
// column-string mutation (computed once from the BC/D0 strings).
extern s32 lbl_eu_80664210;
extern s8 lbl_eu_80664214;
extern s32 lbl_eu_80664220;
extern s8 lbl_eu_80664224;

// ---------------------------------------------------------------------------
// Item data types (unit kyoshin/cf/CItem)
// ---------------------------------------------------------------------------

// 8-byte item record written by func_801558B4 (u32 packed word + u16 flags).
struct CItemRec {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u16 field_04;
};

// Bitfield view over CItemData::field_00: bits 2-4 (LSB) are the item
// category. A category write re-reads the word (retail reloads before the
// rlwimi in func_80156ED4).
struct ItemWordCat {
    u32 pad0 : 2;
    /* bits 2-4 */ u32 mCat : 3;
    u32 pad1 : 27;
};

// ---------------------------------------------------------------------------
// Item data types (unit kyoshin/cf/CItem)
// ---------------------------------------------------------------------------

// Item record: 32-bit packed word at 0x00, record buffer at 0x08.
// func_80156F0C / func_80156F30 pack bitfields into field_00; func_801558B4
// fills the 8-byte record at field_08.
struct CItemData {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u8 field_04;
    /* 0x05 */ u8 field_05;
    /* 0x06 */ u8 field_06;
    /* 0x07 */ u8 field_07;
    /* 0x08 */ u8 field_08[0x10];
    /* 0x18 */ u16 field_18;
    /* 0x1A */ u16 field_1A;
};

// Item extension block (52 bytes): packed item word at 0x00, 8-byte sub
// records at 0x08, s16 entry array at 0x28 with u8 count at 0x30.
// func_8015B86C returns mEntries[idx] when idx < mCount else 0;
// func_8015B6B4 clears the 0x08 sub-records and the entries in place.
struct CItemExt {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u8 field_04;
    /* 0x05 */ u8 field_05;
    /* 0x06 */ u8 field_06;
    /* 0x07 */ u8 field_07;
    /* 0x08 */ u8 field_08[0x20];
    /* 0x28 */ s16 mEntries[4];
    /* 0x30 */ u8 mCount;
    /* 0x31 */ u8 field_31;
    /* 0x32 */ u8 field_32;
    /* 0x33 */ u8 field_33;
};

// Four 52-byte item records followed by a u32 count (stride 0x34; count at
// 0xD0). func_8015AE9C refreshes each record in place.
struct CItemFour {
    /* 0x00 */ CItemExt mItems[4];
    /* 0xD0 */ u32 mCount;
};

// Shared four-record item block used by func_8015AFA4 / func_8015B25C.
extern CItemFour lbl_eu_80573E18;

// Scratch item record built by func_8015A3CC / func_8015A6AC (52 bytes):
// packed word at 0x00 and u16 at 0x04 - the retail initializes both to zero
// (stw + sth) before the bulk 0x34 memset.
struct CItemScratch {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u16 field_04;
    /* 0x06 */ u8 field_06[46];
};

// Indirect u16-id holder used by the func_801589A0 comparator (returns 1 when
// a->mpFamily->mId < b->mpFamily->mId).
struct CItemFamily {
    u8 field_00[4];
    /* 0x04 */ u16 mId;
};

struct CItemFamilyRef {
    /* 0x00 */ CItemFamily* mpFamily;
};

// Per-kind family record sorted by __dt__801589BC: family descriptor pointer
// at 0x00, u16 rank written by the sorter at 0x04.
struct CItemFamilyRec {
    /* 0x00 */ CItemFamily* mpFamily;
    /* 0x04 */ u16 mRank;
};

// Scratch buffer allocated by __dt__801589BC (0x644 bytes): up to 0x190
// collected record pointers, then the count word at 0x640.
struct CItemFamilyBuf {
    /* 0x000 */ CItemFamilyRec* mRecs[0x190];
    /* 0x640 */ s32 mCount;
};

// Sort helper for __dt__801589BC: sorts the pointer range [base, end) with
// the func_801589A0 comparator (retail symbol unmangled - C linkage).
extern "C" void func_80158AF4(CItemFamilyRec** base, CItemFamilyRec** end,
                              u32 (*cmp)(CItemFamilyRec*, CItemFamilyRec*));

// Comparator used by __dt__801589BC's sort: 1 when a's family id < b's.
extern "C" u32 func_801589A0(CItemFamilyRec* a, CItemFamilyRec* b);

// Window over the global item block (lbl_eu_806641B8). 0x10000 is the base of
// the per-slot region (func_8015783C / func_80157948 index it), 0x12038 holds a
// u16 slot/sub-slot table, 0x120E8 the item-count word (func_80157184),
// 0x120EC a u16 flag table (func_8015780C / func_80157824), and 0x12108 the
// 32-bit counters.
struct CItemBlockCounters {
    u8 field_00000[0x10000];
    u8 field_10000[0x2038];
    /* 0x12038 */ u16 mSlots12038[1];  // grows: func_80157948 indexes (b + (a-1)*8) beyond [0]
    u8 field_1203A[0xAE];
    /* 0x120E8 */ u32 mCountE8;
    /* 0x120EC */ u16 mFlags120EC[0x0E];
    /* 0x12108 */ u32 mCount08;
    /* 0x1210C */ u32 mCount0C;
    /* 0x12110 */ u32 mCount10;
};

// Item parameter block: u16 category/id word at 0x0C, u16 flags at 0x10,
// u16 halfword at 0x18.
struct CItemParam {
    u8 field_00[0xC];
    /* 0x0C */ u16 field_0C;
    /* 0x0E */ u16 field_0E;
    /* 0x10 */ u16 field_10;
    u8 field_12[6];
    /* 0x18 */ u16 field_18;
};

// Object exposing an item size via its 4th virtual (vtable slot 0x14;
// MWCC leaves 2 reserved slots at 0x00/0x04 before user virtuals here).
// func_8015B46C dispatches to slot 0x14 to size a memset of an output buffer.
struct CItemVtblSize {
    virtual void vf00() = 0;
    virtual void vf04() = 0;
    virtual void vf08() = 0;
    virtual u32 vf14() = 0;
};

// u16 flag table compared against the item kind by func_8015720C.
extern u16 lbl_eu_8052F6B8[];

// Lazy-init guard flag and shared sort scratch buffer used by
// func_80155F34 / func_80159D74 (u8 at 0x00, u32 at 0x40).
extern s8 lbl_eu_806641FC;
struct CItemSortBuf {
    /* 0x00 */ u8 field_00;
    /* 0x01 */ u8 field_01[0x3F];
    /* 0x40 */ u32 field_40;
};
extern CItemSortBuf lbl_eu_80573FC0;

// Second lazy-init guard/buffer pair used by func_80156060 / func_80156164
// (same layout as CItemSortBuf).
extern s8 lbl_eu_806641FD;
extern s8 lbl_eu_806641FE;
extern CItemSortBuf lbl_eu_80574004;
extern CItemSortBuf lbl_eu_80574048;

// Item-implementation vtable interface returned by
// CItem_initItemImplInstances. Only the vtable offsets exercised by the
// retail call sites (0x08, 0x1C, 0x4C, 0x50, 0x68, 0x78, 0x80) are named;
// the remaining virtuals are placeholders that keep those offsets exact.
// (MWCC auto-inserts RTTI + null at vtable slots 0-1, so the first
// declared virtual lands at slot 2 = offset 0x08.)
class CItemImpl {
public:
    virtual u32 vf08(CItemData* p);              // 0x08
    virtual void vf0C(CItemData* p, u32 x);      // 0x0C
    virtual void vf10(CItemData* p);             // 0x10
    virtual void vf14();                         // 0x14
    virtual void vf18();                         // 0x18
    virtual u32 vf1C(CItemData* p);              // 0x1C
    virtual void vf20();                         // 0x20
    virtual void vf24();                         // 0x24
    virtual void vf28();                         // 0x28
    virtual CItemRec* vf2C(CItemData* p, u32 x); // 0x2C (returns the 8-byte sub-record)
    virtual u32 vf30(CItemData* p);              // 0x30
    virtual void vf34();                         // 0x34
    virtual void vf38();                         // 0x38
    virtual void vf3C();                         // 0x3C
    virtual void vf40();                         // 0x40
    virtual void vf44();                         // 0x44
    virtual void vf48();                         // 0x48
    virtual u32 vf4C(CItemData* p, u32 x);       // 0x4C
    virtual u32 vf50(CItemData* p, u32 x, u16 y); // 0x50
    virtual void vf54();                         // 0x54
    virtual void vf58();                         // 0x58
    virtual void vf5C();                         // 0x5C
    virtual void vf60();                         // 0x60
    virtual void vf64();                         // 0x64
    virtual u32 vf68(CItemData* p, u32 x, u16 y); // 0x68
    virtual void vf6C();                         // 0x6C
    virtual void vf70();                         // 0x70
    virtual void vf74();                         // 0x74
    virtual u32 vf78(CItemData* p);              // 0x78
    virtual void vf7C();                         // 0x7C
    virtual u32 vf80(CItemData* p);              // 0x80
    virtual u32 vf84(CItemData* p, u32 x);       // 0x84
};

// Lazy item-impl accessor: returns the per-category impl instance for self.
extern "C" CItemImpl* CItem_initItemImplInstances(CItemData* self);

// Sort scratch filler shared with func_80155F34 (writes the u8 at 0x00 and
// the u32 at 0x40 of buf).
extern "C" void func_80159D74(void* buf, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);

// Count the zero records in the func_801579C4 list for arg, minus the
// randomizer result when the category is in [2,8].
extern "C" s32 func_80157CD0(u32 arg);

// Item-slot randomizer (size 0x144): counts non-empty slots for kind 3
// (per-slot u16 high-byte table) or the func_801575B0 family flags.
// C linkage: retail symbol is unmangled, so the call-site relocs must
// reference the plain name.
extern "C" s32 func_801576C8(u32 arg);

// Item-family BDAT resolver (unit-local): returns the BDAT file handle for
// family id v and writes the kind/row sub-ids into outA/outB. The retail
// return value is recovered from the func_80155DBC/func_80155E30 call sites,
// which pass it straight to getBdatStringColumnValue. C linkage so the
// call-site reloc uses the plain retail name (PLAN.md §17.6).
extern "C" void* func_80157F04(u16 v, void* outA, void* outB);

// Item-box engine helpers (defined in kyoshin/cf/CtrlObjectParam.cpp).
// func_8009EC9C returns the character-data block for a u16 character id;
// func_800A33C8 looks a character id up by comparing the given inventory
// slot against the party's equipment; func_800A30E4 syncs equipment state.
extern "C" void* func_8009EC9C(u16 index);
extern "C" s32 func_800A33C8(CItemExt* obj);
extern "C" void func_800A30E4(void* data);
