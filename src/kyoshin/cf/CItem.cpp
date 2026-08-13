// Auto-scaffolded catalog TU for kyoshin/cf/CItem
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"
#include "kyoshin/cf/CItem.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/MemManager.hpp"

// Forward declarations for item-related classes
struct CItemInfo;
struct CItemData;
struct CItemParam;
struct CItemExt;

void func_80155660(){}

// extern "C": retail symbol is unmangled, so call-site relocs (func_8015B4F0)
// reference the plain name.
extern "C" int func_80155854(unsigned int param_1) {
    int result;
    if (param_1 < 0xC9) {
        if (param_1 < 0x65) {
            if (param_1 < 1) {
                result = 0;
            } else {
                result = 1;
            }
        } else {
            result = 2;
        }
    } else {
        if (param_1 < 0x191) {
            if (param_1 < 0x12D) {
                result = 3;
            } else {
                result = 4;
            }
        } else {
            result = 0;
        }
    }
    return result;
}

// Retail symbol func_801558B4 is unmangled (C linkage), so the call-site
// relocs must reference the plain name - definition must be extern "C".
extern "C" void func_801558B4(CItemRec* rec, u32 a, u32 b, u32 c, u32 d) {
    if (b == 0) {
        memset(rec, 0, 8);
        return;
    }
    // Pack category/flag fields into the 8-byte record.
    u32 w = rec->field_00;
    w = __rlwimi(w, a, 21, 0, 10);
    u32 h = rec->field_04;
    h = __rlwimi(h, b, 4, 16, 27);
    rec->field_00 = w;
    rec->field_04 = (u16)h;

    int v = func_80155854(b);

    h = rec->field_04;
    h = __rlwimi(h, v, 1, 28, 30);
    w = rec->field_00;
    w = __rlwimi(w, d, 7, 22, 24);
    h = __rlwimi(h, c, 0, 31, 31);
    rec->field_00 = w;
    rec->field_04 = (u16)h;

    // Default item name from the fixed column; empty -> randomize via the
    // per-category tables. The unions keep the call results on the stack
    // (retail stores each result and re-reads its high byte). The D8 handle
    // is loaded into a saved register before the first call (retail hoists
    // the load into r29, reusing the dead d parameter slot).
    void* hD8 = lbl_eu_806640D8;
    union {
        u32 v;
        u8 b[4];
    } t0;
    t0.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0x42, a);
    u32 val = t0.b[0];
    if (val == 0) {
        union {
            u32 v;
            u8 b[4];
        } t1;
        union {
            u32 v;
            u8 b[4];
        } t2;
        t1.v = getBdatStringColumnValue(hD8, lbl_eu_8052F628[(rec->field_00 >> 7) & 7], b);
        t2.v = getBdatStringColumnValue(hD8, lbl_eu_8052F644[(rec->field_00 >> 7) & 7], b);
        val = (u32)ml::math::mtRand(t1.b[0], t2.b[0] + 1);
    }
    w = rec->field_00;
    w = __rlwimi(w, val, 10, 11, 21);
    rec->field_00 = w;

    union {
        u32 v;
        u8 b[4];
    } t3;
    t3.v = getBdatStringColumnValue(hD8, lbl_eu_8052F660[(rec->field_00 >> 7) & 7], b);
    w = rec->field_00;
    w = __rlwimi(w, t3.b[0], 0, 25, 31);
    rec->field_00 = w;
}

// Category-driven item-name refresh on an inventory slot (52-byte CItemExt):
// clear the count, then re-derive it from the BDAT name column when the
// packed word's family id and category select a valid handle.
void func_80155A00(CItemExt* self) {
    u32 word = self->field_00;
    self->mCount = 0;
    if ((word >> 20) != 0) {
        u32 cat = (word >> 16) & 0xF;
        void* h = 0;
        if (cat == 2) {
            h = lbl_eu_806640F4;
        } else {
            // Dispatch on category: 4..8 selects the second handle via a
            // materialized boolean (retail: two cmplwi + li/cmpwi test).
            u32 b = cat >= 4 && cat <= 8;
            if (b) {
                h = lbl_eu_806640F8;
            }
        }
        if (h != 0) {
            u32 w2 = self->field_00;
            union {
                u32 v;
                u8 b[4];
            } tmp;
            tmp.v = getBdatStringColumnValue(h, lbl_eu_80501C58 + 0x5a,
                                             (w2 >> 5) & 0x7FF);
            self->mCount = tmp.b[0];
        }
    }
}

void func_80155AA0(){}

extern "C" __declspec(noinline) CItemImpl* CItem_initItemImplInstances(CItemData* self) { return 0; }

// retail: lwz r0,0x0(r4); extrwi r3,r0,3,27 = (x>>2)&7
extern "C" u32 func_80155CB4(void* self, const void* p) { return (*(const u32*)p >> 2) & 7; }

void func_80155CC0(CItemData*, unsigned long* ptr, unsigned long val) {
    unsigned long v = *ptr;
    *ptr = __rlwimi(v, val, 2, 27, 29);
}

void func_80155CD0(){}

char* func_80155D28() {
    extern char lbl_eu_80501C58[];
    return lbl_eu_80501C58 + 0x63;
}

extern char lbl_eu_80501C58[];

// Recovered CItemData bitfield area (offsets 0x7/0x8/0x18)
struct ItemByte7 { u8 pad0 : 6; u8 b01 : 2; };
struct ItemWord8 { u32 pad0 : 11; u32 f11 : 11; u32 pad1 : 3; u32 f7 : 7; };
struct ItemHalf18 { u16 bit15 : 1; u16 low15 : 15; };

struct If20 { virtual void _v008(); virtual void _v00C(); virtual void _v010(); virtual void _v014();
    virtual void _v018(); virtual void _v01C(); virtual void vf20(); };

extern "C" const char* func_80155D38(void) { return (const char*)lbl_eu_80501C58 + 0x63; }

void func_80155D48(){}

// Resolve the family BDAT column for a packed item record: the column name
// comes from a byte-indexed .sdata table (retail lwz tbl@sda21(rX) is
// unscaled), the BDAT file handle from func_80157F04(family), the row from
// bits 16-26 of the record word (extrwi 11,16 = (x>>5)&0x7FF). Empty lookups
// fall back to the default item-name string. The record word is re-read
// after the call (volatile).
const char* func_80155DBC(u32 unused, CItemRec* rec) {
    u32 v = rec->field_00;
    // Plain scalar read of the .sdata column-name pointer (retail:
    // lwz r31, lbl_eu_8066228C@sda21(r0) - d-form r0 is literal zero).
    const char* col = lbl_eu_8066228C;
    u16 a, b;
    void* h = func_80157F04(v >> 20, &b, &a);
    u32 v2 = *(volatile u32*)&rec->field_00;
    const char* s = (const char*)getBdatStringColumnValue(h, col, (v2 >> 5) & 0x7FF);
    // Eager default-string address + conditional select (retail: lis/cmpi/
    // addi/addi/beq/mr - not a branchy ternary).
    const char* result = lbl_eu_80501C58 + 0x63;
    if (s) result = s;
    return result;
}

// Same as func_80155DBC but using the lbl_eu_80662288 column pointer.
const char* func_80155E30(u32 unused, CItemRec* rec) {
    u32 v = rec->field_00;
    const char* col = lbl_eu_80662288;
    u16 a, b;
    void* h = func_80157F04(v >> 20, &b, &a);
    u32 v2 = *(volatile u32*)&rec->field_00;
    const char* s = (const char*)getBdatStringColumnValue(h, col, (v2 >> 5) & 0x7FF);
    const char* result = lbl_eu_80501C58 + 0x63;
    if (s) result = s;
    return result;
}

void func_80155EA4(){}

// Build the sort scratch buffer for self: lazily zero the shared buffer,
// then collect the per-category impl counts (vtable slots 0x4C/0x80/0x78/
// 0x08) plus the packed field_07 bits and hand them to func_80159D74.
CItemSortBuf* func_80155F34(u32 unused, CItemData* self) {
    if (!lbl_eu_806641FC) {
        lbl_eu_80573FC0.field_00 = 0;
        lbl_eu_80573FC0.field_40 = 0;
        lbl_eu_806641FC = 1;
    }
    u32 v1 = CItem_initItemImplInstances(self)->vf4C(self, 0);
    u32 v2 = CItem_initItemImplInstances(self)->vf80(self);
    u32 v27 = (self->field_07 >> 24) & 0x3F;
    u32 v3 = CItem_initItemImplInstances(self)->vf78(self);
    u32 v4 = CItem_initItemImplInstances(self)->vf08(self);
    func_80159D74(&lbl_eu_80573FC0, self->field_07 & 3, (u16)v4, (u16)v3, v27, (u16)v2, (u16)v1);
    return &lbl_eu_80573FC0;
}

int CItemInfo_getSize0(CItemInfo*) { return 0; }

int CItemInfo_isEnabled0(CItemInfo*) { return 0; }

int CItemInfo_getFlag0(CItemInfo*) { return 0; }

extern "C" void func_80156050(If20* self) { self->vf20(); }

// Lazy-init the shared sort scratch buffer, then collect the per-category
// impl counts (vtable slots 0x4C/0x80/0x78/0x08) plus the packed field_07
// bits and hand them to func_80159D74. Same shape as func_80155F34 but with
// its own guard/buffer pair.
CItemSortBuf* func_80156060(u32 unused, CItemData* self) {
    if (!lbl_eu_806641FD) {
        lbl_eu_80574004.field_00 = 0;
        lbl_eu_80574004.field_40 = 0;
        lbl_eu_806641FD = 1;
    }
    u32 v1 = CItem_initItemImplInstances(self)->vf4C(self, 0);
    u32 v2 = CItem_initItemImplInstances(self)->vf80(self);
    u32 v27 = (self->field_07 >> 2) & 0x3F;
    u32 v3 = CItem_initItemImplInstances(self)->vf78(self);
    func_80159D74(&lbl_eu_80574004, self->field_07 & 3,
                  (u16)CItem_initItemImplInstances(self)->vf08(self),
                  (u16)v3, v27, (u16)v2, (u16)v1);
    return &lbl_eu_80574004;
}

// Same lazy-init sort-scratch helper as func_80156060 with its own guard
// (lbl_eu_806641FE) and buffer (lbl_eu_80574048).
CItemSortBuf* func_80156164(u32 unused, CItemData* self) {
    if (!lbl_eu_806641FE) {
        lbl_eu_80574048.field_00 = 0;
        lbl_eu_80574048.field_40 = 0;
        lbl_eu_806641FE = 1;
    }
    u32 v1 = CItem_initItemImplInstances(self)->vf4C(self, 0);
    u32 v2 = CItem_initItemImplInstances(self)->vf80(self);
    u32 v27 = (self->field_07 >> 2) & 0x3F;
    u32 v3 = CItem_initItemImplInstances(self)->vf78(self);
    // Inlining the last vf call in the argument list fuses its (u16)
    // truncation into the arg move (rlwinm r5,r3 instead of or+rlwinm).
    func_80159D74(&lbl_eu_80574048, self->field_07 & 3,
                  (u16)CItem_initItemImplInstances(self)->vf08(self),
                  (u16)v3, v27, (u16)v2, (u16)v1);
    return &lbl_eu_80574048;
}

void func_80156268(){}

void func_801562DC(){}

void func_80156350(){}

void func_8015650C(){}

void func_80156924() {}

void func_80156928() {}

u8 CItemInfo_getByte22(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

void func_80156934(){}

void func_80156BD8() {}

u8 CItemInfo_getByte22_dup1(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

// Prepare an item slot for use: force a non-zero family/row id, clear the
// slot flag word, refresh the per-category impl state (vf0C/vf84), re-derive
// the item-name byte from BDAT (column +0x6D), pack the id into field_18,
// then re-rank the four u16 slot flags (each low nibble, plus the
// func_80155854 rank).
void func_80156BE4(u32 unused, CItemData* self) {
    u32 word = self->field_00;
    void* handle = lbl_eu_806640D8;
    u32 v = (word >> 5) & 0x7FF;
    union {
        u32 value;
        u8 b[4];
    } tmp;
    u32 f7;
    u32 f18;
    if (v == 0) v = 1;
    *(u16*)&self->field_08[8] = 0;
    CItem_initItemImplInstances(self)->vf0C(self, 1);
    CItem_initItemImplInstances(self)->vf84(self, 1);
    self->field_08[0xE] = 1;
    tmp.value = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x6d, (u16)v);
    f18 = self->field_18;
    f7 = self->field_07;
    f18 = __rlwimi(f18, v, 0, 17, 31);
    f7 = __rlwimi(f7, tmp.b[0], 2, 24, 29);
    self->field_07 = (u8)f7;
    self->field_18 = (u16)f18;
    u16* slots = (u16*)self->field_08;
    for (int i = 0; i < 4; i++) {
        // Mask to the low nibble: __rlwinm forces the exact rlwinm r0,r0,0,28,15
        // (retail mask 0xF000000F; a plain & 0xF would emit 28,31).
        u32 sv = slots[i];
        sv = __rlwinm(sv, 0, 28, 15);
        slots[i] = (u16)sv;
        u32 rank = (u32)func_80155854(0);
        // Volatile re-read after the call: retail reloads the halfword here;
        // a plain load would fold to the stored register value (callee is pure).
        u32 h = *(volatile u16*)&slots[i];
        h = __rlwimi(h, rank, 1, 28, 30);
        slots[i] = (u16)h;
    }
}

u8 CItemInfo_getByte22_dup2(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

// Pack the family/row ids into the slot word and refresh the per-index u16
// record flags for a CItemData slot. When the row is 0, also refresh the
// item name from BDAT (column +0x6D of the default-name handle).
void func_80156CF8(u32 unused, CItemData* obj, int index, int value) {
    if (value != 0 && index == 0) {
        u32 w = obj->field_00;
        w = __rlwimi(w, value, 20, 0, 11);
        w = __rlwimi(w, value, 5, 16, 26);
        obj->field_00 = w;

        // Read the MSB of the BDAT result; the union keeps the call result
        // on the stack (big-endian b[0] is the high byte).
        union { u32 value; u8 b[4]; } tmp;
        tmp.value = getBdatStringColumnValue(lbl_eu_806640D8, lbl_eu_80501C58 + 0x6d, value);
        u32 f7 = obj->field_07;
        f7 = __rlwimi(f7, tmp.b[0], 2, 24, 29);
        obj->field_07 = (u8)f7;
    }

    // Volatile slot view: retail reloads the halfword after the func_80155854
    // call; MWCC folds a non-volatile reload into the stored register value.
    u16* slot = (u16*)&obj->field_08[index * 2];
    u32 h = slot[0];
    h = __rlwimi(h, value, 4, 16, 27);
    slot[0] = (u16)h;
    int rank = func_80155854(value);
    // Volatile re-read after the call: retail reloads the halfword here;
    // a plain load would fold to the stored register value (callee is pure).
    h = *(volatile u16*)slot;
    h = __rlwimi(h, rank, 1, 28, 30);
    slot[0] = (u16)h;
}

// Refresh the item-slot word of self: re-derive the item-name id from BDAT
// (column +0x4A) when the family id is non-zero, pack it into bits 5-15,
// refresh the per-category impl state via vf0C, then rebuild the 8-byte
// record at field_08 (memset when family is 0, else via func_801558B4 with
// the +0x4A/+0x51 column values).
void func_80156DAC(u32 unused, CItemData* self) {
    u32 word = self->field_00;
    u16 val;
    if ((word >> 20) != 0) {
        union { u32 v; u16 h[2]; } tmp;
        tmp.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0x4a, word >> 20);
        val = tmp.h[0];
    } else {
        val = 0;
    }
    word = self->field_00;
    word = __rlwimi(word, val, 5, 16, 26);
    self->field_00 = word;
    union { u32 v; u8 b[4]; } t2;
    t2.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0x51, word >> 20);
    CItem_initItemImplInstances(self)->vf0C(self, t2.b[0]);
    u32 family = self->field_00 >> 20;
    if (family == 0) {
        memset(&self->field_08, 0, 8);
    } else {
        void* handle = lbl_eu_806640EC;
        u16 v2 = 0;
        if (family != 0) {
            union { u32 v; u16 h[2]; } t3;
            t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x4a, family);
            v2 = t3.h[0];
        }
        union { u32 v; u8 b[4]; } t4;
        t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x51, family);
        func_801558B4((CItemRec*)self->field_08, family, v2, 0, t4.b[0]);
    }
}

void func_80156ED4(u32 unused, CItemData* obj) {
    // Set the category bits (field_00 bits 2-4) to 1 when they are still 0.
    // Volatile re-read reproduces the retail's reload before the rlwimi.
    u32 word = obj->field_00;
    if (((word >> 2) & 7) == 0) {
        u32 w = *(volatile u32*)&obj->field_00;
        obj->field_00 = __rlwimi(w, 1, 2, 27, 29);
    }
    word = obj->field_00;
    func_801558B4((CItemRec*)obj->field_08, word >> 20, (word >> 5) & 0x7FF, 0, (word >> 2) & 7);
}

void func_80156F0C(u32 unused, CItemData* obj, u32 val) {
    u32 word = obj->field_00;
    word = __rlwimi(word, val, 2, 27, 29);
    obj->field_00 = word;
    return func_801558B4((CItemRec*)obj->field_08, word >> 20, (word >> 5) & 0x7FF, 0, (word >> 2) & 7);
}

void func_80156F30(u32 unused, CItemData* obj, u32 val) {
    u32 word = obj->field_00;
    word = __rlwimi(word, val, 5, 16, 26);
    obj->field_00 = word;
    return func_801558B4((CItemRec*)obj->field_08, word >> 20, (word >> 5) & 0x7FF, 0, (word >> 2) & 7);
}

// Map an item category (0-15) to the record stride used by func_801579C4's
// kind table: 2/4-8 -> 52 (0x34), 9 -> 28 (0x1c), 3 -> 16 (0x10), else 8.
extern "C" int func_80156F54(u16 v) {
    int rank;
    if (v < 4) goto sw;
    if (v > 8) goto sw;
    rank = 2;
    goto done;
sw:
    switch (v) {
    case 0: rank = 0; break;
    case 2: rank = 2; break;
    case 9: rank = 3; break;
    case 3: rank = 4; break;
    default: rank = 1; break;
    }
done:
    switch (rank) {
    case 1: return 8;
    case 2: return 0x34;
    case 3: return 0x1c;
    case 4: return 0x10;
    default: return 8;
    }
}

void func_80156FF8(){}

void func_8015704C(){}

void func_801570A0(){}

void __dt__801570F0(){}

// Destructor of the global item block: frees lbl_eu_806641B8 and nulls it.
void __dt__80157150() {
    if (lbl_eu_806641B8 != 0) {
        operator delete(lbl_eu_806641B8);
        lbl_eu_806641B8 = 0;
    }
}

// Clamp the value to 999999999 and publish it as the item-count word.
void func_80157184(u32 value) {
    u32 idx = 999999999u;
    if (value <= 999999999u) {
        idx = value;
    }
    CItemBlockCounters* blk = (CItemBlockCounters*)lbl_eu_806641B8;
    blk->mCountE8 = idx;
}

void func_801571A8(){}

extern "C" void* func_801571FC(void) { return *(void**)((u8*)lbl_eu_806641B8 + 0x10000 + 0x20e8); }

// Enable/disable the family flag for kind v: set (or clear) bit (1<<v) in
// the word at block+0x12108 when v matches one of the six u16 entries of
// the lbl_eu_8052F6B8 table (t[0] via the lhzu-updated pointer, then
// t[2..6] -- the first entry's post-increment skews the offsets by one).
void func_8015720C(u32 v, u32 set) {
    if (v < 1) return;
    if (v > 0xb) return;
    u32 bit = 1u << v;
    if (set != 0) {
        const u16* p = lbl_eu_8052F6B8;
        if (v == *p++) ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 |= bit;
        if (v == p[1]) ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 |= bit;
        if (v == p[2]) ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 |= bit;
        if (v == p[3]) ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 |= bit;
        if (v == p[4]) ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 |= bit;
        if (v == p[5]) ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 |= bit;
    } else {
        ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 &= ~bit;
    }
}

void func_8015730C(){}

extern "C" u32 func_801575B0(u32 v, u32 kind) {
    if (v < 1) return 0;
    if (v > 0xb) return 0;
    u32 bit = 1u << v;
    if (kind == 2) return (*(u32*)((u8*)lbl_eu_806641B8 + 0x1210c) & bit) != 0;
    if (kind == 4) return (*(u16*)((u8*)lbl_eu_806641B8 + 0x12114) & bit) != 0;
    if (kind == 5) return (*(u16*)((u8*)lbl_eu_806641B8 + 0x12116) & bit) != 0;
    if (kind == 6) return (*(u16*)((u8*)lbl_eu_806641B8 + 0x12118) & bit) != 0;
    if (kind == 7) return (*(u16*)((u8*)lbl_eu_806641B8 + 0x1211a) & bit) != 0;
    if (kind == 8) return (*(u16*)((u8*)lbl_eu_806641B8 + 0x1211c) & bit) != 0;
    return 0;
}

// Count the non-empty sub-slots of the item block for the given kind: kind 3
// reads the signed high bytes of the per-slot u16 table at block+0x12038
// (8 entries per slot); other kinds use func_801575B0's family-flag lookup
// for each of the 11 slots.
extern "C" s32 func_801576C8(u32 kind) {
    s32 count = 0;
    u32 i = 1;
    do {
        if (kind == 3) {
            u32 n8 = (i - 1) * 8;
            const u16* tbl = (const u16*)((const char*)lbl_eu_806641B8 + 0x12038);
            if (*(const s8*)&tbl[n8 + 0] > 0) count++;
            if (*(const s8*)&tbl[n8 + 1] > 0) count++;
            if (*(const s8*)&tbl[n8 + 2] > 0) count++;
            if (*(const s8*)&tbl[n8 + 3] > 0) count++;
            if (*(const s8*)&tbl[n8 + 4] > 0) count++;
            if (*(const s8*)&tbl[n8 + 5] > 0) count++;
            if (*(const s8*)&tbl[n8 + 6] > 0) count++;
            if (*(const s8*)&tbl[n8 + 7] > 0) count++;
        } else if (func_801575B0(i, kind) != 0) {
            count++;
        }
        i++;
    } while (i <= 11);
    return count;
}

unsigned short func_8015780C(int index) {
    return ((unsigned short*)(lbl_eu_806641B8 + 0x120EC))[index];
}

void func_80157824(int index, short value) {
    ((short*)((char*)lbl_eu_806641B8 + 0x120EC))[index] = value;
}

extern "C" void* func_8015783C(s32 kind, u32 idx, u32 sub) {
    extern char* lbl_eu_806641B8;
    if (idx - 1 > 0xa) return 0;
    switch (kind) {
    case 2: return (u8*)lbl_eu_806641B8 + 0x10000 + (idx + 0x96) * 0x34 - 0x5a74;
    case 4: return (u8*)lbl_eu_806641B8 + (idx + 0x96) * 0x34;
    case 5: return (u8*)lbl_eu_806641B8 + (idx + 0x96) * 0x34 + 0x211c;
    case 6: return (u8*)lbl_eu_806641B8 + (idx + 0x96) * 0x34 + 0x4238;
    case 7: return (u8*)lbl_eu_806641B8 + (idx + 0x96) * 0x34 + 0x6354;
    case 8: return (u8*)lbl_eu_806641B8 + 0x10000 + (idx + 0x96) * 0x34 - 0x7b90;
    case 3: return (u8*)lbl_eu_806641B8 + 0x10000 + ((sub + (idx - 1) * 8) << 4) + 0x1ab8;
    }
    return 0;
}

// Return a pointer into the per-slot u16 table at block+0x12038, indexed by
// sub-slot b and slot a (byte offset (b + (a-1)*8)*2 from 0x10000+0x2038).
char* func_80157948(u32 a, u32 b) {
    char* base = (char*)lbl_eu_806641B8;
    return base + 0x10000 + (b + (a - 1) * 8) * 2 + 0x2038;
}

// Increment the shared slot counter (block+0x12110) by x, then clamp it to 30.
void func_8015796C(u32 x) {
    ((CItemBlockCounters*)lbl_eu_806641B8)->mCount10 += x;
    u32 v = ((CItemBlockCounters*)lbl_eu_806641B8)->mCount10;
    ((CItemBlockCounters*)lbl_eu_806641B8)->mCount10 = v > 30 ? 30 : v;
}

void func_801579A4() {
    // Clear the two per-slot counters in the global item block
    // (lbl_eu_806641B8 + 0x12110 / 0x12108).
    ((CItemBlockCounters*)lbl_eu_806641B8)->mCount10 = 0;
    ((CItemBlockCounters*)lbl_eu_806641B8)->mCount08 = 0;
}

extern "C" void* func_801579C4(u16 arg, s32* out1, s32* out2) {
    void* result = 0;
    u32* ptr;
    u32 v;

    func_8009CF8C(0x80c);

    *out1 = 0;

    for (u32 i = 1; i <= 11; i++) {
        if (!!(((u32*)lbl_eu_806641B8)[0x4842] & (1 << i))) {
            *out1 += 10;
        }
    }

    ptr = (u32*)lbl_eu_806641B8;
    v = ptr[0x4844];
    if (v < 30) {
        v = 30;
    }
    ptr[0x4844] = v;

    if (arg > 13) {
        *out1 = 0;
        *out2 = 0;
        return 0;
    }

    switch (arg) {
        case 0:
        case 1:
            *out2 = 0;
            *out1 = 0;
            return 0;
        case 2:
            *out2 = 52;
            result = (char*)ptr + 0xA58C;
            *out1 += v + 60;
            break;
        case 3:
            *out2 = 16;
            *out1 = 300;
            result = (char*)ptr + 0xE778;
            break;
        case 4:
            *out2 = 52;
            result = ptr;
            *out1 += v + 60;
            break;
        case 5:
            *out2 = 52;
            result = (char*)ptr + 0x211C;
            *out1 += v + 60;
            break;
        case 6:
            *out2 = 52;
            result = (char*)ptr + 0x4238;
            *out1 += v + 60;
            break;
        case 7:
            *out2 = 52;
            result = (char*)ptr + 0x6354;
            *out1 += v + 60;
            break;
        case 8:
            *out2 = 52;
            result = (char*)ptr + 0x8470;
            *out1 += v + 60;
            break;
        case 9:
            *out2 = 28;
            *out1 = 300;
            result = (char*)ptr + 0xE778;
            break;
        case 10:
            *out2 = 8;
            *out1 = 300;
            result = (char*)ptr + 0x101B8;
            break;
        case 11:
            *out2 = 8;
            result = (char*)ptr + 0x10B18;
            *out1 += v + 60;
            break;
        case 12:
            *out2 = 8;
            *out1 = 200;
            result = (char*)ptr + 0x11478;
            break;
        case 13:
            *out2 = 8;
            *out1 = 240;
            result = (char*)ptr + 0xFA38;
            break;
    }

    return result;
}

extern "C" u32 func_80157C20(u16 arg) {
    s32 a, b;
    func_801579C4(arg, &a, &b);
    return (u32)a;
}

// Returns a pointer to the item record for the given family/kind (body not
// yet recovered; noinline keeps the retail `bl` at call sites, C linkage
// keeps call-site relocs on the plain retail name).
extern "C" __declspec(noinline) CItemExt* func_80157C4C(u32 a, s16 b) { return 0; }

// Count the zero records in the item-block list for arg (stride-stepped),
// then subtract the randomizer result when the category is in [2,8].
s32 func_80157CD0(u32 arg) {
    s32 count, stride;
    void* list = func_801579C4((u16)arg, &count, &stride);
    s32 result = 0;
    if (list != 0) {
        u8* p = (u8*)list;
        for (s32 i = 0; i < count; i++) {
            if (*(u32*)p == 0) result++;
            p += stride;
        }
        if (arg - 2 <= 6) {
            result -= func_801576C8(arg);
            if (result < 0) result = 0;
        }
    }
    return result;
}

void func_80157D6C(){}

// Retail signature recovered from the func_80155DBC/func_80155E30 call
// sites: family/row id in r3 (u16), two out pointers, BDAT handle in r3.
// Body not yet recovered (not a target); noinline keeps the retail `bl` at
// the two call sites above (MWCC would fold the trivial body). C linkage to
// match the header declaration (plain-name reloc).
extern "C" __declspec(noinline) void* func_80157F04(u16 v, void* outA, void* outB) { return 0; }

// Read a u16 column value (string column +0xBD) for the given index.
// The union keeps the call result on the stack (MWCC allocates unions in
// memory); the returned halfword is the upper 16 bits (big-endian h[0]).
u16 func_80157FDC(int param) {
    union {
        u32 value;
        u16 half[2];
    } tmp;
    tmp.value = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, param);
    return tmp.half[0];
}

void func_80158018(){}

// Sum the +6 slot byte over every family-matching record in the item block
// list returned by func_801579C4 (list of 52-byte CItemExt records).
u32 func_80158068(u32 family) {
    u32 result = 0;
    if (family != 0) {
        s32 count, stride;
        union {
            u32 v;
            u16 h[2];
        } tmp;
        tmp.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, family);
        CItemExt* list = (CItemExt*)func_801579C4(tmp.h[0], &count, &stride);
        if (list != 0) {
            u8* base = (u8*)list;
            for (s32 i = 0; i < count; i++) {
                CItemExt* rec = (CItemExt*)(base + i * stride);
                if (family == (rec->field_00 >> 20)) {
                    result += rec->field_06;
                }
            }
        }
    }
    return result;
}

// stub body not yet recovered; noinline keeps the retail `bl` at call sites.
// extern "C": retail symbol is unmangled, so the call-site reloc must reference
// the plain name (same rationale as func_801558B4's documented comment).
extern "C" void __declspec(noinline) func_80158118(CItemData* self, u32 a, u32 b){}

void func_801582FC() {}

void func_80158300(){}

void func_801583DC() {}

// Enable the family flag: set low 2 bits of byte 7 (ItemByte7::b01).
void func_801583E0(CItemData* self, u32 a) {
    func_80158118(self, a, 1);
    ((ItemByte7*)&self->field_07)->b01 = 1;
}

// not a target; noinline keeps the retail `bl` in callers (MWCC would fold
// the trivial body). extern "C" so call-site relocs use the plain retail
// name (func_801586D4's reloc was drifting to the mangled form).
extern "C" __declspec(noinline) CItemExt* func_80158420(u32 a, s16* pOut1, u32 b, u32* pOut2) { return 0; }

void func_801586CC() {}

void func_801586D0() {}

// Thunk: forwards to func_80158420 with two stack out-params.
void func_801586D4(u32 a, u32 b) {
    s16 local1;
    u32 local2;
    func_80158420(a, &local1, b, &local2);
}

// Find the first free (zero first-word) record in the item-block list for
// self's category. func_801579C4 writes the record count to a local and the
// stride into *pStride; on success *pOut receives the record index and the
// record address is returned. The stride is re-read every iteration.
extern "C" CItemExt* func_80158700(CItemData* self, s32* pStride, s16* pOut) {
    *pOut = -1;
    if (self == 0) return 0;
    s32 count;
    void* list = func_801579C4((self->field_00 >> 16) & 0xF, &count, pStride);
    if (list != 0) {
        u32 cat = (self->field_00 >> 16) & 0xF;
        if ((cat - 2) <= 6 && func_80157CD0(cat) == 0) return 0;
        for (s32 i = 0; i < count; i++) {
            CItemExt* rec = (CItemExt*)((u8*)list + (*pStride) * i);
            if (rec->field_00 == 0) {
                *pOut = (s16)i;
                return rec;
            }
        }
    }
    return 0;
}

// Look up the BDAT item-name id for the family, resolve the item-block list
// via func_801579C4, and return the first 52-byte record whose packed word
// matches the family id (or 0 when nothing matches).
CItemExt* func_801587E8(u32 family) {
    s32 count, stride;
    union {
        u32 v;
        u16 h[2];
    } tmp;
    tmp.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, family);
    if (tmp.h[0] == 0) return 0;
    CItemExt* list = (CItemExt*)func_801579C4(tmp.h[0], &count, &stride);
    if (list != 0) {
        u8* base = (u8*)list;
        for (s32 i = 0; i < count; i++) {
            CItemExt* rec = (CItemExt*)(base + i * stride);
            if (family == (rec->field_00 >> 20)) return rec;
        }
    }
    return 0;
}

// Compact the item-block list: allocate a stride*count scratch buffer, copy
// every non-empty record forward, record the destination index of each
// copied slot in out (all other out entries stay -1), then copy the
// compacted list back over the original.
int func_80158894(u16 arg, u16* out) {
    s32 count;
    s32 stride;
    u8* list = (u8*)func_801579C4(arg, &count, &stride);
    if (list == 0) return 0;
    u32 total = stride * count;
    void* buf = mtl::MemManager::allocate_head(mtl::MemManager::getHandleMEM2(), total, 4);
    memset(buf, 0, total);
    for (s32 i = 0; i < count; i++) out[i] = 0xFFFF;
    s32 dst = 0;
    for (s32 src = 0; src < count; src++) {
        if (*(u32*)(list + stride * src) != 0) {
            memcpy((u8*)buf + stride * dst, list + stride * src, stride);
            out[src] = (u16)dst;
            dst++;
        }
    }
    memcpy(list, buf, total);
    if (buf != 0) mtl::MemManager::deallocate(buf);
    return 1;
}

extern "C" u32 func_801589A0(CItemFamilyRec* a, CItemFamilyRec* b) {
    return a->mpFamily->mId < b->mpFamily->mId;
}

// Reassign the per-kind record ranks: for the given kind, collect the
// non-empty records from func_801579C4's list into a scratch buffer, sort
// them by family id (func_801589A0) and write each record's new 1-based
// rank into the u16 at record+4. Returns one past the highest rank (1 when
// the list is empty, 2 when a single record was found).
extern "C" s32 __dt__801589BC(u16 arg) {
    CItemFamilyRec* list;   // r31
    s32 result = 1;         // r30
    CItemFamilyBuf* buf;    // r29
    s32 count, stride;      // stack out-params (count@0xc, stride@0x8)
    list = (CItemFamilyRec*)func_801579C4(arg, &count, &stride);
    if (list == 0) return 1;
    buf = (CItemFamilyBuf*)mtl::MemManager::allocate(0x644, mtl::MemManager::getHandleMEM2());
    buf->mCount = 0;
    for (s32 i = 0; i < count; i++) {
        CItemFamilyRec* rec = (CItemFamilyRec*)((char*)list + stride * i);
        if (rec->mpFamily != 0) {
            buf->mRecs[buf->mCount] = rec;
            buf->mCount++;
        }
    }
    s32 n = buf->mCount;
    if (n == 1) {
        buf->mRecs[0]->mRank = 1;
        result = 2;
    } else if (n >= 2) {
        CItemFamilyRec** pBase = buf->mRecs;
        CItemFamilyRec** pEnd = pBase + n;
        func_80158AF4(pBase, pEnd, func_801589A0);
        CItemFamilyRec** p = buf->mRecs;
        s32 i = 0;
        while (i < buf->mCount) {
            (*p)->mRank = (u16)result;
            p++;
            result++;
            i++;
        }
    }
    if (buf != 0) operator delete(buf);
    return result;
}

extern "C" void func_80158AF4(CItemFamilyRec** base, CItemFamilyRec** end,
                              u32 (*cmp)(CItemFamilyRec*, CItemFamilyRec*)) {}

void func_80158E74(){}

// Sort the three 4-byte values *a/*b/*c using the comparator function
// pointer *pCmp (returns 0 for equal). When both the (c,a) and (b,c)
// comparisons are equal nothing moves; when both differ the two are
// swapped; otherwise the odd element is pushed to the right slot.
void func_801591F4(u32* a, u32* b, u32* c, int (**pCmp)(u32*, u32*)) {
    int eq_ca = ((*pCmp)(c, a) == 0);
    int eq_bc = ((*pCmp)(b, c) == 0);
    if (eq_ca && eq_bc) {
        // both pairs equal: no reorder needed
    } else if (!eq_ca && !eq_bc) {
        u32 t = *a;
        *a = *b;
        *b = t;
    } else {
        if ((*pCmp)(b, a) != 0) {
            u32 t = *a;
            *a = *b;
            *b = t;
        }
        if (eq_ca) {
            u32 t = *b;
            *b = *c;
            *c = t;
        } else {
            u32 t = *a;
            *a = *c;
            *c = t;
        }
    }
}

void func_801592EC(){}

void func_80159348(){}

void func_80159524(){}

// Look up an item record for the family/row in self: when the BDAT name
// column (string column +0xBD) is in [0x0A, 0x0D] the record is created
// directly via func_80158420; otherwise a free record is found via
// func_80158700, every family-kind record's rank field is bumped, and the
// record is copied into the slot. *pOut receives the slot index (or -1 when
// no record exists); the created record pointer is returned.
extern "C" CItemExt* func_801599D4(CItemData* self, s16* pOut) {
    if (pOut != 0) {
        *pOut = -1;
    }
    if (self == 0 || (self->field_00 >> 20) == 0) {
        return 0;
    }
    u32 out2;   // 0x1C
    s32 stride; // 0x18
    union {
        u32 v;
        u16 h[2];
    } t;        // 0x14
    s32 count;  // 0x10
    s32 stride2;// 0x0C
    s16 out1;   // 0x08
    t.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, self->field_00 >> 20);
    u32 flag = 0;
    if ((u32)(t.h[0] - 10) <= 3) flag = 1;
    CItemExt* r;
    if (flag) {
        r = func_80158420(self->field_00 >> 20, &out1, self->field_06, &out2);
        if (pOut != 0) *pOut = out1;
    } else {
        r = func_80158700(self, &stride, &out1);
        if (r != 0) {
            // Bump the rank field of every non-empty family-kind record so
            // the re-ranked families stay in sync with the item block.
            for (u16 kind = 2; kind < 14; kind++) {
                func_801579C4(kind, &count, &stride2);
                for (s32 i = 0; i < count; i++) {
                    CItemRec* rec = (CItemRec*)func_80157C4C(kind, (s16)i);
                    if (rec != 0 && rec->field_00 != 0 && rec->field_04 < 0xffffu) {
                        rec->field_04++;
                    }
                }
            }
            memcpy(r, self, (u32)stride);
            if (pOut != 0) *pOut = out1;
        }
    }
    return r;
}

void func_80159B40(){}

// Consume `count` items of family `family` from the item block: each
// iteration resolves the family's kind table via the +0xBD name column,
// finds the matching record, and consumes one record (bumping the kind-impl
// state via vf10). When the name column is in [0x0A, 0x0D] the record's +6
// byte caps how many items one record can absorb. Returns the leftover count.
extern "C" s32 func_80159C04(u32 family, s32 count) {
    if (family == 0) {
        return count;
    }
    union {
        u32 v;
        u16 h[2];
    } t;     // 0x14
    union {
        u32 v;
        u16 h[2];
    } t2;    // 0x10
    s32 stride; // 0x0C
    s32 n;   // 0x08
    t.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, family);
    u32 flag = 0;
    if ((u32)(t.h[0] - 10) <= 3) flag = 1;
    while (count > 0) {
        t2.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, family);
        CItemExt* rec = 0;
        if (t2.h[0] != 0) {
            void* list = func_801579C4(t2.h[0], &n, &stride);
            if (list != 0 && n > 0) {
                u8* base = (u8*)list;
                for (s32 i = 0; i < n; i++) {
                    CItemExt* r = (CItemExt*)(base + i * stride);
                    if ((r->field_00 >> 20) == family) {
                        rec = r;
                        break;
                    }
                }
            }
        }
        if (rec == 0) break;
        if (flag) {
            u8 v6 = rec->field_06;
            if (count < v6) {
                rec->field_06 = (u8)(v6 - count);
                count = 0;
            } else {
                count -= v6;
                CItem_initItemImplInstances((CItemData*)rec)->vf10((CItemData*)rec);
            }
        } else {
            CItem_initItemImplInstances((CItemData*)rec)->vf10((CItemData*)rec);
            count--;
        }
    }
    return count;
}

extern "C" __declspec(noinline) void func_80159D74(void* buf, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f){}

// Initialize an item slot: clamp the kind to [1,6], zero a record sized by
// func_80156F54(3) when the slot is non-null, then pack the family/kind/
// row ids into the slot word (bits 20-31 / 16-19 / 5-15 / 2-4) and refresh
// the per-category impl state via the vtable method at 0x1C.
void func_80159F6C(CItemData* self, u32 family, u32 row, u16 kind) {
    kind = kind < 1 ? 1 : (kind > 6 ? 6 : kind);
    s32 size = func_80156F54(3);
    if (self != 0 && size != 0) {
        memset(self, 0, (u32)size);
    }
    self->field_07 |= 3;
    u32 w = self->field_00;
    w = __rlwimi(w, family, 20, 0, 11);
    w = __rlwimi(w, 3, 16, 12, 15);
    w = __rlwimi(w, family, 5, 16, 26);
    self->field_06 = 1;
    w = __rlwimi(w, kind, 2, 27, 29);
    w = __rlwimi(w, row, 5, 16, 26);
    self->field_00 = w;
    CItem_initItemImplInstances(self)->vf1C(self);
}

void func_8015A054(){}

void func_8015A230() {}

void func_8015A234() {}

void func_8015A238(){}

// Roll a random item in the kind list: mutate the two column-name strings
// with the iteration digit, read the BDAT name columns for the family/row,
// and on a successful roll initialise the 52-byte scratch record, append it
// to self's record array and copy the record out. Returns 1 on success.
extern "C" s32 func_8015A3CC(CItemFour* self, u32 a, u32 unused, void* c, u32 d) {
    CItemScratch local;
    local.field_04 = 0;
    local.field_00 = 0;
    if (lbl_eu_80664214 == 0) {
        lbl_eu_80664210 = strlen(lbl_eu_806622BC) - 1;
        lbl_eu_80664214 = 1;
    }
    memset(&local, 0, 0x34);
    s32 idx = lbl_eu_80664210;
    for (s32 i = 0; i < 8; i++) {
        char* col1 = lbl_eu_806622BC;
        char* col2 = lbl_eu_806622C0;
        int ch = 0x31 + i;
        col1[idx] = ch;
        col2[idx] = ch;
        union {
            u32 v;
            u16 h[2];
        } r1v;
        union {
            u32 v;
            u8 b[4];
        } r2v;
        r1v.v = getBdatStringColumnValue(c, col1, a);
        r2v.v = getBdatStringColumnValue(c, lbl_eu_806622C0, a);
        int v2b = r2v.b[0];
        int r = ml::math::mtRand(100);
        if (r1v.h[0] != 0 && r < v2b) {
            func_80158118((CItemData*)&local, r1v.h[0], 1);
            CItem_initItemImplInstances((CItemData*)&local)->vf1C((CItemData*)&local);
            local.field_00 = __rlwimi(local.field_00, d, 0, 30, 31);
            u32 count = self->mCount;
            self->mCount = count + 1;
            self = (CItemFour*)((char*)self + count * 0x34);
            u32 size = (u32)func_80156F54((u16)((local.field_00 >> 16) & 0xF));
            memcpy(self, &local, size);
            return 1;
        }
    }
    return 0;
}

void func_8015A51C(){}

// Same item-roll helper as func_8015A3CC but operating on the
// lbl_eu_806622D0/lbl_eu_806622D4 column strings and the
// lbl_eu_80664220/lbl_eu_80664224 cache pair.
extern "C" s32 func_8015A6AC(CItemFour* self, u32 a, u32 unused, void* c, u32 d) {
    CItemScratch local;
    local.field_04 = 0;
    local.field_00 = 0;
    if (lbl_eu_80664224 == 0) {
        lbl_eu_80664220 = strlen(lbl_eu_806622D0) - 1;
        lbl_eu_80664224 = 1;
    }
    memset(&local, 0, 0x34);
    s32 idx = lbl_eu_80664220;
    for (s32 i = 0; i < 8; i++) {
        char* col1 = lbl_eu_806622D0;
        char* col2 = lbl_eu_806622D4;
        int ch = 0x31 + i;
        col1[idx] = ch;
        col2[idx] = ch;
        union {
            u32 v;
            u16 h[2];
        } r1v;
        union {
            u32 v;
            u8 b[4];
        } r2v;
        r1v.v = getBdatStringColumnValue(c, col1, a);
        r2v.v = getBdatStringColumnValue(c, lbl_eu_806622D4, a);
        int v2b = r2v.b[0];
        int r = ml::math::mtRand(100);
        if (r1v.h[0] != 0 && r < v2b) {
            func_80158118((CItemData*)&local, r1v.h[0], 1);
            CItem_initItemImplInstances((CItemData*)&local)->vf1C((CItemData*)&local);
            local.field_00 = __rlwimi(local.field_00, d, 0, 30, 31);
            u32 count = self->mCount;
            self->mCount = count + 1;
            self = (CItemFour*)((char*)self + count * 0x34);
            u32 size = (u32)func_80156F54((u16)((local.field_00 >> 16) & 0xF));
            memcpy(self, &local, size);
            return 1;
        }
    }
    return 0;
}

void func_8015A7FC(){}

// Roll up to maxCount items for the shared four-record block: mutate the two
// column-name strings' last character with the iteration digit, read the
// BDAT name/percent columns, and on a passing roll build a 52-byte item
// record (func_80158118 + vf1C) and append it to self's record array.
// forceA/forceB and the second-iteration guarantee force a 100% roll.
extern "C" void func_8015A930(CItemFour* self, u32 row, s32 maxCount,
                              u32 forceA, u32 randomize, u32 forceB) {
    s32 len = strlen(lbl_eu_806622E0) - 1;
    void* handle = lbl_eu_8066414C;
    CItemScratch local;   // 0x10
    union {
        u32 v;
        u16 h[2];
    } r1v;                // 0x0C
    union {
        u32 v;
        u8 b[4];
    } r2v;                // 0x08
    local.field_04 = 0;
    local.field_00 = 0;
    s32 added = 0;
    s32 start = 0;
    if (randomize != 0) {
        if (ml::math::mtRand(100) < 25) start = 2;
    }
    for (s32 i = 0; i < 2; i++) {
        char* col1 = lbl_eu_806622E0;
        char* col2 = lbl_eu_806622E4;
        int ch = start + (i & 1) + 0x31;
        col1[len] = (char)ch;
        col2[len] = (char)ch;
        r1v.v = getBdatStringColumnValue(handle, col1, row);
        r2v.v = getBdatStringColumnValue(handle, col2, row);
        int rnd = ml::math::mtRand(100);
        u8 pct = r2v.b[0];
        if (forceA != 0 && forceB == 0) pct = 100;
        if (i == 1 && added == 0) pct = 100;
        if (r1v.h[0] != 0 && rnd < pct) {
            func_80158118((CItemData*)&local, (u16)r1v.h[0], 1);
            CItem_initItemImplInstances((CItemData*)&local)->vf1C((CItemData*)&local);
            local.field_00 &= ~3u;
            u32 n = self->mCount;
            self->mCount = n + 1;
            CItemExt* dst = (CItemExt*)((u8*)self + n * 0x34);
            memcpy(dst, &local, (u32)func_80156F54((u16)((local.field_00 >> 16) & 0xF)));
            added++;
        }
        if (added >= maxCount) break;
    }
}

// Roll one more item into the shared block (body not yet recovered).
extern "C" void __declspec(noinline) func_8015AAB4(CItemFour* self, u32 item, u32 count, u32 row){}

// Roll a third item into the shared block (body not yet recovered).
extern "C" void __declspec(noinline) func_8015ACAC(CItemFour* self, u32 item, u32 count){}

// Refresh every item in the four-record list: records whose packed kind
// (field_00 bits 12-15) is 9, or whose field_07 low bits are 2, get the
// 4-iteration vf4C refresh; others get the count-driven vf2C/vf30 refresh.
extern "C" void func_8015AE9C(CItemFour* self) {
    for (u32 i = 0; i < self->mCount; i++) {
        CItemExt* rec = &self->mItems[i];
        CItemData* item = reinterpret_cast<CItemData*>(rec);
        u32 kind = (rec->field_00 >> 12) & 0xF;
        bool first = kind == 9 || (rec->field_07 & 3) == 2;
        if (first) {
            for (u32 j = 0; j < 4; j++) {
                CItem_initItemImplInstances(item)->vf4C(item, j);
            }
        } else {
            for (u32 j = 0; j < (u16)CItem_initItemImplInstances(item)->vf30(item); j++) {
                CItem_initItemImplInstances(item)->vf2C(item, j);
            }
        }
    }
}

// Open the item box for an area event: read four BDAT string columns (the
// item ids and a mode byte), reset the shared four-record block, then
// dispatch on the event kind (1: roll up to 2 items; 2: roll 1 + one more;
// 3: roll 1 + one more + a third item).
extern "C" void func_8015AFA4(u32 kind, u32 row) {
    void* handle = lbl_eu_806640CC;
    union {
        u32 v;
        u8 b[4];
    } t1;     // 0x14
    union {
        u32 v;
        u8 b[4];
    } t2;     // 0x10
    union {
        u32 v;
        u8 b[4];
    } t3;     // 0x0C
    union {
        u32 v;
        u8 b[4];
    } t4;     // 0x08
    t1.v = getBdatStringColumnValue(handle, lbl_eu_80501C58, row);
    u8 item1 = t1.b[0];
    t2.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x9, row);
    u8 item2 = t2.b[0];
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x12, row);
    u8 item3 = t3.b[0];
    t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0xb7, row);
    u8 v8 = t4.b[0];
    CItemFour* block = &lbl_eu_80573E18;
    block->mCount = 0;
    u32 flag = (v8 == 2);
    if (kind == 1) {
        func_8015A930(block, item1, 2, 0, 0, 0);
    } else if (kind == 2) {
        func_8015A930(block, item1, 1, 1, 0, (u16)flag != 0);
        func_8015AAB4(block, item2, 2, row);
    } else if (kind == 3) {
        func_8015A930(block, item1, 1, 1, 0, (u16)flag != 0);
        func_8015AAB4(block, item2, 1, row);
        func_8015ACAC(block, item3, 2);
    }
}

extern u8 lbl_eu_80573EEC[];

void func_8015B11C() {
    *(u32*)(lbl_eu_80573EEC + 0xd0) = 0;
}

void func_8015B130(){}

// Create the item box contents for the given box id: scan the +0x13A name
// column rows for a match, then for each of the four +0x140 sub-columns
// build a 52-byte item record (func_80158118 + vf1C) and append it to the
// shared four-record block. Refreshes the block and re-syncs it afterwards.
// Returns 1 when any record was created.
extern "C" s32 func_8015B25C(u32 id) {
    CItemScratch scratch;   // 0x20
    char buf[16];           // 0x10
    union {
        u32 v;
        u16 h[2];
    } t;                    // 0x0C
    union {
        u32 v;
        u16 h[2];
    } t2;                   // 0x08
    CItemFour* block = &lbl_eu_80573E18;
    block->mCount = 0;
    scratch.field_04 = 0;
    scratch.field_00 = 0;
    memset(&scratch, 0, 0x34);
    void* handle = lbl_eu_806640E8;
    s32 base = (s32)func_8003B41C(handle);
    s32 total = (s32)func_8003B1EC(handle);
    for (s32 i = 0; i < total; i++) {
        s32 row = i + base;
        t.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x13a, row);
        if (id == t.h[0]) {
            for (s32 j = 1; j <= 4; j++) {
                sprintf(buf, lbl_eu_80501C58 + 0x140, j);
                t2.v = getBdatStringColumnValue(handle, buf, row);
                if (t2.h[0] != 0) {
                    func_80158118((CItemData*)&scratch, (u16)t2.h[0], 1);
                    CItem_initItemImplInstances((CItemData*)&scratch)->vf1C((CItemData*)&scratch);
                    u32 n = block->mCount;
                    block->mCount = n + 1;
                    CItemExt* dst = (CItemExt*)((u8*)block + n * 0x34);
                    memcpy(dst, &scratch, (u32)func_80156F54((u16)((scratch.field_00 >> 16) & 0xF)));
                }
            }
            func_8015AE9C(block);
            func_8013E424(block, 0);
            return block->mCount != 0;
        }
    }
    return 0;
}

int CItemData_isFalse0(CItemData*) { return 0; }

int CItemData_isTrue(CItemData*) { return 1; }

int CItemData_isFalse1(CItemData*) { return 0; }

int CItemData_isFalse2(CItemData*) { return 0; }

int CItemData_isFalse3(CItemData*) { return 0; }

extern "C" void func_8015B404(void* u, u8* p, u32 val) { ((ItemWord8*)(p + 8))->f7 = val; }

// retail: lwz r0,0x8(r4); clrlwi r3,r0,25 = (x>>8) & 0x7F... = *(u32*)(p+8) & 0x7F
extern "C" u32 func_8015B414(void* self, const void* p) { return *(const u32*)((const char*)p + 8) & 0x7F; }

extern "C" void func_8015B420(void* u, u8* p, u32 val) { ((ItemWord8*)(p + 8))->f11 = val; }

extern "C" s16 func_8015B430(void* u, u8* p) {
    u32 x = *(u32*)(p + 8);
    return (s16)((x >> 10) & 0x7FF);
}

u32 CItemParam_getCategory(u32, CItemParam* obj) { return (*(u16*)((char*)obj + 12) >> 1) & 7; }

u32 CItemParam_getId(u32, CItemParam* obj) { return (*(u16*)((char*)obj + 12) >> 4) & 0xFFF; }

void* CItemData_getBuffer(u32, CItemData* obj) { return (void*)((char*)obj + 8); }

u32 CItemData_getInvByte6(u32, CItemData* obj) { return 1 - *(u8*)((char*)obj + 6); }

// Clear a size-bytes buffer; the size comes from the object's vtable slot 0x14.
void func_8015B46C(CItemVtblSize* obj, u8* buf) {
    memset(buf, 0, obj->vf14());
}

int CItemData_getSize16(CItemData*) { return 16; }

u32 CItemData_getBits7to9(u32, CItemData* obj) { return (*(u32*)((char*)obj + 8) >> 7) & 7; }

extern "C" void func_8015B4C8(void* u, u8* p, u32 val) { ((ItemByte7*)(p + 7))->b01 = (u8)val; }

u32 CItemData_getByte7Bits01(u32, CItemData* obj) { return *(u8*)((char*)obj + 7) & 3; }

void func_8015B4E4() {}

int CItemData_isFalse4(CItemData*) { return 0; }

// Pack the family id into u16 field_0C bits 0-11 and the rank (from
// func_80155854) into bits 12-14.
void func_8015B4F0(u32 unused, CItemParam* obj, u32 val) {
    u16 h = obj->field_0C;
    h = __rlwimi(h, val, 4, 16, 27);
    obj->field_0C = (u16)h;

    int v = func_80155854(val);
    h = obj->field_0C;
    h = __rlwimi(h, v, 1, 28, 30);
    obj->field_0C = (u16)h;
}

void func_8015B538() {}

int CItemParam_isFalse0(CItemParam*) { return 0; }

int CItemParam_isFalse1(CItemParam*) { return 0; }

int CItemParam_isFalse2(CItemParam*) { return 0; }

int CItemParam_isFalse3(CItemParam*) { return 0; }

u32 CItemParam_setField16(u32 unused, CItemParam* obj, u16 val) { *(u16*)((char*)obj + 16) = val; return unused; }

u16 CItemParam_getField16(u32, CItemParam* obj) { return *(u16*)((char*)obj + 16); }

extern "C" void func_8015B56C(void* u, u8* p, u32 val) { ((ItemHalf18*)(p + 0x18))->bit15 = (u16)val; }

u32 CItemParam_getField24Bit15(u32, CItemParam* obj) { return (*(u16*)((char*)obj + 24) >> 15) & 1; }

extern "C" void func_8015B588(void* u, u8* p, u32 val) { ((ItemHalf18*)(p + 0x18))->low15 = (u16)val; }

u32 CItemParam_getField24Mask(u32, CItemParam* obj) { return *(u16*)((char*)obj + 24) & 0x7FFF; }

extern "C" void func_8015B5A4(void* u, u8* p, u32 val) { ((ItemByte7*)(p + 7))->b01 = (u8)val; }

u32 CItemData_getByte7Bits01_dup(u32, CItemData* obj) { return *(u8*)((char*)obj + 7) & 3; }

u32 CItemData_getInvByte6_dup(u32, CItemData* obj) { return 1 - *(u8*)((char*)obj + 6); }

extern "C" u8* func_8015B5CC(u32 unused, u8* obj, u32 idx, u8 val) {
    obj[idx + 18] = val;
    return obj + idx;
}

extern "C" u8 func_8015B5D8(u32 unused, u8* obj, u32 idx) { return obj[idx + 0x12]; }

extern "C" u32 func_8015B5E4(void* u, u8* p, u32 idx) { return (*(u16*)(p + idx * 2 + 8) >> 1) & 7; }

extern "C" u32 func_8015B5F8(void* u, u8* p, u32 idx) { return (*(u16*)(p + idx * 2 + 8) >> 4) & 0xFFF; }

// Zero a buffer sized by the object's virtual size (vtable slot 0x14);
// same shape as the matched func_8015B46C.
void func_8015B60C(CItemVtblSize* obj, u8* buf) {
    memset(buf, 0, obj->vf14());
}

int CItemExt_getSize28(CItemExt*) { return 28; }

void func_8015B65C(){}

u32 CItemData_getInvByte6_dup2(u32, CItemData* obj) { return 1 - *(u8*)((char*)obj + 6); }

// Clear used item sub-records: for every entry whose index is below the
// live count, reset the matching 8-byte record at 0x08 and, when the slot
// still resolves to a character, refresh its equipment state.
void func_8015B6B4(u32 unused, CItemExt* obj) {
    for (s32 i = 0; i < obj->mCount; i++) {
        // Volatile re-read: retail reloads the count (lbz) for the bounds
        // check instead of reusing the loop-condition load.
        s16 v = (i < *(volatile u8*)&obj->mCount) ? obj->mEntries[i] : 0;
        if (v != -1 && i < obj->mCount) {
            obj->mEntries[i] = -1;
            memset(obj->field_08 + i * 8, 0, 8);
            s32 r = func_800A33C8(obj);
            if (r > 0) {
                func_800A30E4(func_8009EC9C((u16)r));
            }
        }
    }
}

// Set the equipment slot entry at index to value. When replacing a valid
// item (-1 clears the slot instead), refresh the 8-byte sub-record at
// field_08[index*8] from the family-3 impl record; a cleared slot zeroes
// the sub-record. Finally re-sync the character's equipment state.
void func_8015B75C(CItemExt* self, u32 index, s16 value) {
    if (index < self->mCount) {
        self->mEntries[index] = value;
        if (value != -1) {
            // Volatile re-read: retail reloads the count (lbz) for the bounds
            // check instead of reusing the earlier load.
            s16 v = (index < *(volatile u8*)&self->mCount) ? self->mEntries[index] : 0;
            CItemExt* rec = 0;
            if (v != -1) {
                rec = func_80157C4C(3, v);
                if (rec->field_00 == 0) rec = 0;
            }
            if (rec != 0) {
                CItemRec* r = CItem_initItemImplInstances((CItemData*)rec)->vf2C((CItemData*)rec, 0);
                CItemRec* dst = (CItemRec*)&self->field_08[index * 8];
                dst->field_00 = r->field_00;
                dst->field_04 = r->field_04;
            }
        } else {
            memset(&self->field_08[index * 8], 0, 8);
        }
    }
    s32 c = func_800A33C8(self);
    if (c > 0) {
        func_800A30E4(func_8009EC9C((u16)c));
    }
}

s16 func_8015B86C(u32 unused, CItemExt* obj, int idx) {
    s16 result = 0;
    if (idx < obj->mCount) result = obj->mEntries[idx];
    return result;
}

extern "C" u32 func_8015B88C(void* u, u8* p, u32 idx) { return (*(u16*)(p + idx * 8 + 0xc) >> 4) & 0xFFF; }

u32 CItemExt_setByte49(u32 unused, CItemExt* obj, u8 val) { *(u8*)((char*)obj + 49) = val; return unused; }

u8 CItemExt_getByte49(u32, CItemExt* obj) { return *(u8*)((char*)obj + 49); }

u32 CItemExt_setByte48(u32 unused, CItemExt* obj, u8 val) { *(u8*)((char*)obj + 48) = val; return unused; }

u8 CItemExt_getByte48(u32, CItemExt* obj) { return *(u8*)((char*)obj + 48); }

extern "C" void* func_8015B8C0(void* u, u8* p, u32 idx) { return p + idx * 8 + 8; }

// Zero a buffer sized by the object's virtual size (vtable slot 0x14).
void func_8015B8D0(CItemVtblSize* obj, u8* buf) {
    memset(buf, 0, obj->vf14());
}

int CItemExt_getSize52(CItemExt*) { return 52; }

u32 CItemExt_get99minusByte6(u32, CItemExt* obj) { return 99 - *(u8*)((char*)obj + 6); }

// Zero a buffer sized by the object's virtual size (vtable slot 0x14).
void func_8015B92C(CItemVtblSize* obj, u8* buf) {
    memset(buf, 0, obj->vf14());
}

int CItemExt_getSize8(CItemExt*) { return 8; }

u32 CItemExt_getInvByte6(u32, CItemExt* obj) { return 1 - *(u8*)((char*)obj + 6); }

void func_8015B988(){}

int CItemExt_getSize8_dup(CItemExt*) { return 8; }

// --- hard-symbol stubs (scaffold_hard_symbols) ---
void sinit_8015B9D8(){}
