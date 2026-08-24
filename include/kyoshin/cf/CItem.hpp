#pragma once

#include <types.h>

// C-linkage imports (retail symbol names - keep linkage/signatures verbatim)
extern "C" u32 func_8009CF8C(u32 resourceId);

// BDAT column lookup (retail C-linkage name; declared here like
// CfObjectPoint.hpp - ocBdat.hpp intentionally does not promote it).
extern "C" u32 getBdatStringColumnValue(void*, const char*, s32);

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

// BDAT handle used by func_8015ACAC's random-roll lookups.
extern void* lbl_eu_80664154;

// BDAT handles used by func_80159D74's per-family item-name lookups (a==2
// path picks one of the three by the c argument).
extern void* lbl_eu_80664170;
extern void* lbl_eu_80664094;
extern void* lbl_eu_8066416C;

// BDAT handle used by func_8015A930's item-roll loop.
extern void* lbl_eu_8066414C;

// BDAT handles selected by func_80155A00's category dispatch (cat==2 picks
// lbl_eu_806640F4, cat in [4,8] picks lbl_eu_806640F8).
extern void* lbl_eu_806640F4;
extern void* lbl_eu_806640F8;
// BDAT handles selected by func_80157F04's category dispatch (cases 2..7).
extern void* lbl_eu_806640FC;
extern void* lbl_eu_80664104;
extern void* lbl_eu_80664108;
extern void* lbl_eu_8066410C;
extern void* lbl_eu_80664110;

// BDAT handle used by func_80156934's slot refresh lookups.
extern void* lbl_eu_80664100;

// BDAT handle used by func_8015AAB4's weighted random-roll lookups.
extern void* lbl_eu_80664150;

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

// u8 min/max pair table indexed by func_80156934's slot-size roll. Fixed
// size keeps it sdata-eligible (an incomplete array type is not, so MWCC
// would emit lis/addi instead of the retail li sym@sda21).
extern u8 lbl_eu_80662290[8];

// Introsort depth counters (func_80158AF4 / func_80158E74 pivot-sample
// jitter - each recursive helper keeps its own counter).
extern s32 lbl_eu_8066229C;
extern s32 lbl_eu_806622A0;

// Item-name prefix string used by func_80159D74's per-language name
// concatenation (copied to the prefix buffer, then appended in language-
// specific order). Fixed size keeps it sdata-eligible (an incomplete array
// type is not, so MWCC would emit lis/addi instead of the retail li
// sym@sda21).
extern char lbl_eu_806622A4[8];

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

// Column-name string pointers mutated by func_8015A238: the last character
// of each pointed-to string is replaced with '1'..'4' per loop iteration
// (three columns per iteration).
extern char* lbl_eu_806622B0;
extern char* lbl_eu_806622B4;
extern char* lbl_eu_806622B8;

// Column-name string pointers mutated by func_8015A51C: the last character
// of each pointed-to string is replaced with '1'..'4' per loop iteration
// (three columns per iteration).
extern char* lbl_eu_806622C4;
extern char* lbl_eu_806622C8;
extern char* lbl_eu_806622CC;

// Column-name string pointer mutated by func_80156350: the last character
// of the pointed-to string is replaced with '1'.. per loop iteration.
extern char* lbl_eu_80662298;

// Column-name string pointers mutated by func_8015A054: the last character
// of each pointed-to string is replaced with '1'/'2' per loop iteration.
extern char* lbl_eu_806622A8;
extern char* lbl_eu_806622AC;

// Lazy strlen-1 cache + init flag for the func_8015A3CC / func_8015A6AC
// column-string mutation (computed once from the BC/D0 strings).
extern s32 lbl_eu_80664210;
extern s8 lbl_eu_80664214;
extern s32 lbl_eu_80664220;
extern s8 lbl_eu_80664224;

// Lazy strlen-1 cache + init flag for the func_8015A238 / func_8015A51C
// column-string mutation (computed once from the B0/C4 strings).
extern s32 lbl_eu_80664208;
extern s8 lbl_eu_8066420C;
extern s32 lbl_eu_80664218;
extern s8 lbl_eu_8066421C;

// Lazy strlen-1 cache + init flag for the func_8015A054 column-string
// mutation (computed once from the A8/AC strings).
extern s32 lbl_eu_80664200;
extern s8 lbl_eu_80664204;

// ---------------------------------------------------------------------------
// Item data types (unit kyoshin/cf/CItem)
// ---------------------------------------------------------------------------

// 8-byte item record written by func_801558B4 (u32 packed word + u16 flags).
struct CItemRec {
    /* 0x00 */ u32 field_00;
    /* 0x04 */ u16 field_04;
};

// Bitfield view over CItemData::field_00: bits 2-4 (LSB, i.e. MSB 27-29)
// are the item category. A category write re-reads the word (retail reloads
// before the rlwimi in func_80156ED4). PPC bitfields allocate from the MSB.
struct ItemWordCat {
    u32 pad0 : 27;
    /* MSB bits 27-29 */ u32 mCat : 3;
    u32 pad1 : 2;
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
// The user-provided constructor (zeroes each record's packed word + flag
// halfword and the count) makes the globals non-trivial, which is what
// generates retail's sinit_8015B9D8 static initializer.
struct CItemFour {
    /* 0x00 */ CItemExt mItems[4];
    /* 0xD0 */ u32 mCount;
    CItemFour();
};

// Shared four-record item block used by func_8015AFA4 / func_8015B25C.
extern CItemFour lbl_eu_80573E18;
extern CItemFour lbl_eu_80573EEC;

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
// Sort the pointer range [base, end) using the func_801589A0 comparator
// (retail symbol unmangled - C linkage).
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
    /* 0x12114 */ u16 mFlag14;
    /* 0x12116 */ u16 mFlag16;
    /* 0x12118 */ u16 mFlag18;
    /* 0x1211A */ u16 mFlag1A;
    /* 0x1211C */ u16 mFlag1C;
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

// ---------------------------------------------------------------------------
// Per-category item-impl instances (lazy-initialized by
// CItem_initItemImplInstances)
// ---------------------------------------------------------------------------

// 4-byte impl instance shell: the only member is the vtable pointer that the
// lazy-init guard blocks write (retail stw of a .data vtable address).
struct CItemImplInst {
    void* vtable;
};
extern CItemImplInst lbl_eu_806641C0;
extern CItemImplInst lbl_eu_806641C8;
extern CItemImplInst lbl_eu_806641D0;
extern CItemImplInst lbl_eu_806641D8;
extern CItemImplInst lbl_eu_806641E0;
extern CItemImplInst lbl_eu_806641E8;
extern CItemImplInst lbl_eu_806641F0;
extern CItemImplInst lbl_eu_806641F8;

// Lazy-init guard flags (one per impl instance above).
extern s8 lbl_eu_806641BC;
extern s8 lbl_eu_806641C4;
extern s8 lbl_eu_806641CC;
extern s8 lbl_eu_806641D4;
extern s8 lbl_eu_806641DC;
extern s8 lbl_eu_806641E4;
extern s8 lbl_eu_806641EC;
extern s8 lbl_eu_806641F4;

// Vtable objects written into the impl instances' vtable slots.
extern u8 lbl_eu_8052FC58[];
extern u8 lbl_eu_8052FBA8[];
extern u8 lbl_eu_8052FAF8[];
extern u8 lbl_eu_8052FA48[];
extern u8 lbl_eu_8052F990[];
extern u8 lbl_eu_8052F8D8[];
extern u8 lbl_eu_8052F828[];
extern u8 lbl_eu_8052F770[];

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
    virtual void vf34(CItemData* p, u32 x);      // 0x34
    virtual void vf38();                         // 0x38
    virtual void vf3C(CItemData* p, u32 x);      // 0x3C
    virtual u32 vf40(CItemData* p, u32 x);       // 0x40
    virtual void vf44(CItemData* p, u32 x, u32 y); // 0x44
    virtual void vf48(CItemData* p);             // 0x48
    virtual u32 vf4C(CItemData* p, u32 x);       // 0x4C
    virtual u32 vf50(CItemData* p, u32 x, u16 y); // 0x50
    virtual void vf54();                         // 0x54
    virtual void vf58(CItemData* p, u32 x);      // 0x58
    virtual void vf5C();                         // 0x5C
    virtual void vf60();                         // 0x60
    virtual void vf64();                         // 0x64
    virtual u32 vf68(CItemData* p, u32 x, u16 y); // 0x68
    virtual void vf6C();                         // 0x6C
    virtual void vf70();                         // 0x70
    virtual void vf74();                         // 0x74
    virtual u32 vf78(CItemData* p);              // 0x78
    virtual void vf7C(CItemData* p, u16 x);      // 0x7C
    virtual u32 vf80(CItemData* p);              // 0x80
    virtual u32 vf84(CItemData* p, u32 x);       // 0x84
    virtual void vf88();                         // 0x88
    virtual void vf8C(CItemData* p, u32 x);      // 0x8C
    virtual void vf90();                         // 0x90
    virtual u32 vf94(CItemData* p, u32 x);       // 0x94
};

// Lazy item-impl accessor: returns the per-category impl instance for self.
extern "C" CItemImpl* CItem_initItemImplInstances(CItemData* self);

// Per-kind impl table (indexed by kind & 0xF) used by the default dispatch
// of CItem_initItemImplInstances.
extern CItemImpl* lbl_eu_8052F67C[16];

// Sort scratch filler shared with func_80155F34 (writes the u8 at 0x00 and
// the u32 at 0x40 of buf).
extern "C" void func_80159D74(CItemSortBuf* buf, u32 a, u32 b, u32 c, u32 d,
                              u32 e, u32 f);

// 68-byte string slot used by func_80159D74's name building: a 0x40-byte
// string buffer plus a running length (the retail zeroes mStr[0]/mLen at the
// top of the function, then fills each slot via strlen/strcpy/strcat).
struct CItemNameBuf {
    /* 0x00 */ char mStr[0x40];
    /* 0x40 */ s32 mLen;
};

// CfGameManager data-id refresh (retail C-linkage name; the unity TU
// provides the definition).
extern "C" void func_8007F11C__Q22cf13CfGameManagerFv();

// Console language id (0..6).
extern "C" int getLanguage__9CDeviceSCFv();

// ---------------------------------------------------------------------------
// Party item-slot bookkeeping table (func_80159348)
// ---------------------------------------------------------------------------

// Per-party inventory/equip-slot tables refreshed by func_80159348: the 13
// characters (ids 1..13) each own six equip slots (u16 at +0x0C, row stride
// 0xC) and up to eight parallel (item id, slot, sub-index) tuples stored in
// the three u16 arrays at +0xAC / +0x17C / +0x24C (row stride 0x10). The
// entries are signed so the retail's `li r31, -1` clear value materialises
// as a signed -1 (u16 fields would force a lis/subi 0xFFFF pair).
struct CItemPartySlots {
    u8 pad_00[0xC];
    /* 0x0C */ s16 mSlots[13][6];
    u8 pad_A8[4];
    /* 0xAC */ s16 mArr1[13][8];
    /* 0x17C */ s16 mArr2[13][8];
    /* 0x24C */ s16 mArr3[12][8];
    /* 0x30C */ u8 pad_30C[8];
};

// Count the zero records in the func_801579C4 list for arg, minus the
// randomizer result when the category is in [2,8].
extern "C" s32 func_80157CD0(u32 arg);

// Party item-slot table refresh (retail symbol unmangled - C linkage).
extern "C" void func_80159348(CItemPartySlots* self);

// Compact one kind's record list into the out table (retail symbol
// unmangled - C linkage; the capacity argument is ignored by the callee).
extern "C" int func_80158894(u16 arg, u16* out, s32 capacity);

// Item-slot randomizer (size 0x144): counts non-empty slots for kind 3
// (per-slot u16 high-byte table) or the func_801575B0 family flags.
// C linkage: retail symbol is unmangled, so the call-site relocs must
// reference the plain name.
extern "C" s32 func_801576C8(u32 arg);

// Item-roll helpers called by func_8015AAB4's weighted dispatch (retail
// symbols unmangled - C linkage so call-site relocs use the plain names).
extern "C" s32 func_8015A51C(CItemFour* self, u32 a, u32 unused, void* c, u32 d);
extern "C" s32 func_8015A054(CItemFour* self, u32 a, u32 unused, void* c, u32 d, u32 e);

// Find a free record in the item-block list for arg (writes the record
// index to *pOut; retail symbol unmangled - C linkage).
extern "C" CItemExt* func_80157D6C(u32 arg, s16* pOut, u32 family);

// Item record lookup by kind + entry id (defined in CItem.cpp; noinline in
// retail - call sites bl the out-of-line symbol).
extern "C" __declspec(noinline) CItemExt* func_80157C4C(u32 kind, s16 idx);

// Three-element sort helper used by func_80158AF4's median-of-3 pivot
// selection (retail symbol unmangled - C linkage).
extern "C" void func_801591F4(u32* a, u32* b, u32* c, int (**pCmp)(u32*, u32*));

// Recursive half-sort used by func_80158AF4's introsort loop (retail
// symbol unmangled - C linkage).
extern "C" void func_80158E74(CItemFamilyRec** base, CItemFamilyRec** end,
                              u32 (**cmp)(CItemFamilyRec*, CItemFamilyRec*));

// Item-family BDAT resolver (unit-local): returns the BDAT file handle for
// family id v and writes the kind/row sub-ids into outA/outB. The retail
// return value is recovered from the func_80155DBC/func_80155E30 call sites,
// which pass it straight to getBdatStringColumnValue. C linkage so the
// call-site reloc uses the plain retail name (PLAN.md §17.6). The first
// param is u32 so call sites pass the id unmasked (retail mr, not clrlwi).
extern "C" void* func_80157F04(u32 v, void* outA, void* outB);

// Character-data / equip-table helpers (defined in CtrlObjectParam.cpp):
// resolve the item instance for an equip slot, set an equip-slot entry, and
// the u16 row-table readers/writers used by the party item-slot refresh.
extern "C" u8* func_8009D790(s16* arr, u32 idx);
extern "C" void func_8009DBF4(void* a, unsigned long b, void* c);
extern "C" u16 func_8009E0B4(void* data, u32 index, void* ptr);
extern "C" void func_8009E0C4(void* table, u16 index, u16 value);

// CfGameManager party-inventory queries (defined in the CfGameManager unity
// TU, src/kyoshin/cf/CfGameManagerUnityHelpers.hpp): count/sum the items of
// a kind across the party members. The retail mangled names are kept as the
// plain C-linkage identifiers.
extern "C" u32 func_80082E50__Q22cf13CfGameManagerFv(s32 playerIndex, u32 value);
extern "C" u32 func_80082EC4__Q22cf13CfGameManagerFv(s32 playerIndex, u32 value);

// Item-box engine helpers (defined in kyoshin/cf/CtrlObjectParam.cpp).
// func_8009EC9C returns the character-data block for a u16 character id;
// func_800A33C8 looks a character id up by comparing the given inventory
// slot against the party's equipment; func_800A30E4 syncs equipment state.
extern "C" void* func_8009EC9C(u16 index);
extern "C" s32 func_800A33C8(CItemExt* obj);
extern "C" void func_800A30E4(void* data);
