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

// Sum the party inventory counts for two item kinds, read six BDAT
// probability/entry columns, weight-adjust them, then roll a weighted
// random item index (3..1) for the box contents. Returns the rolled index
// (0 when nothing is rolled).
s32 func_80155660(u32 row, u32 flag) {
    s32 sum1 = 0;
    for (s32 i = 0; i < 3; i++) {
        if (func_80082E50__Q22cf13CfGameManagerFv(i, 0x92) != 0) {
            sum1 += func_80082EC4__Q22cf13CfGameManagerFv(i, 0x92);
        }
    }
    s32 sum2 = 0;
    for (s32 i = 0; i < 3; i++) {
        if (func_80082E50__Q22cf13CfGameManagerFv(i, 0x94) != 0) {
            sum2 += func_80082EC4__Q22cf13CfGameManagerFv(i, 0x94);
        }
    }
    void* handle = lbl_eu_806640CC;
    s32 weights[4];
    u16 entries[4];
    union {
        u32 v;
        u8 b[4];
    } t0;
    union {
        u32 v;
        u8 b[4];
    } t1;
    union {
        u32 v;
        u8 b[4];
    } t2;
    union {
        u32 v;
        u8 b[4];
    } t3;
    union {
        u32 v;
        u8 b[4];
    } t4;
    union {
        u32 v;
        u8 b[4];
    } t5;
    t0.v = getBdatStringColumnValue(handle, lbl_eu_80501C58, row);
    entries[1] = (u16)t0.b[0];
    t1.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x9, row);
    entries[2] = (u16)t1.b[0];
    t2.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x12, row);
    entries[3] = (u16)t2.b[0];
    entries[0] = 0;
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x1b, row);
    weights[1] = t3.b[0];
    t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x28, row);
    weights[2] = t4.b[0];
    t5.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x35, row);
    weights[3] = t5.b[0];
    if (flag != 0) {
        weights[3] = weights[3] * 3;
        if (weights[2] != 0) weights[2] = 100;
    }
    if (weights[2] != 0) weights[2] += sum1;
    if (weights[3] != 0) weights[3] += sum2;
    for (s32 i = 3; i > 0; i--) {
        if (ml::math::mtRand(100) < weights[i]) {
            if (entries[i] != 0) return i;
            return 0;
        }
    }
    return 0;
}

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

// Lazy-initialize the per-category item-impl instances (4-byte vtable-only
// shells at lbl_eu_806641C0..F8, guarded by the s8 flags at BC..F4), then
// dispatch on the packed record's kind field: kinds 9 (byte-7 low bits 2/3)
// and 3 (byte-7 low bits 3) map to dedicated instances; anything else falls
// back to the per-kind table at lbl_8052F67C.
extern "C" __declspec(noinline) CItemImpl* CItem_initItemImplInstances(CItemData* self) {
    if (lbl_eu_806641BC == 0) {
        lbl_eu_806641C0.vtable = (void*)&lbl_eu_8052FC58;
        lbl_eu_806641BC = 1;
    }
    if (lbl_eu_806641C4 == 0) {
        lbl_eu_806641C8.vtable = (void*)&lbl_eu_8052FBA8;
        lbl_eu_806641C4 = 1;
    }
    if (lbl_eu_806641CC == 0) {
        lbl_eu_806641D0.vtable = (void*)&lbl_eu_8052FAF8;
        lbl_eu_806641CC = 1;
    }
    if (lbl_eu_806641D4 == 0) {
        lbl_eu_806641D8.vtable = (void*)&lbl_eu_8052FA48;
        lbl_eu_806641D4 = 1;
    }
    if (lbl_eu_806641DC == 0) {
        lbl_eu_806641E0.vtable = (void*)&lbl_eu_8052F990;
        lbl_eu_806641DC = 1;
    }
    if (lbl_eu_806641E4 == 0) {
        lbl_eu_806641E8.vtable = (void*)&lbl_eu_8052F8D8;
        lbl_eu_806641E4 = 1;
    }
    if (lbl_eu_806641EC == 0) {
        lbl_eu_806641F0.vtable = (void*)&lbl_eu_8052F828;
        lbl_eu_806641EC = 1;
    }
    if (lbl_eu_806641F4 == 0) {
        lbl_eu_806641F8.vtable = (void*)&lbl_eu_8052F770;
        lbl_eu_806641F4 = 1;
    }
    u16 kind = (self->field_00 >> 16) & 0xF;
    u32 flag = 0;
    if (kind == 9u && (self->field_07 & 3u) == 2u) flag = 1;
    if (flag) return (CItemImpl*)&lbl_eu_806641E0;
    flag = 0;
    if (kind == 9u && (self->field_07 & 3u) == 3u) flag = 1;
    if (flag) return (CItemImpl*)&lbl_eu_806641E8;
    flag = 0;
    if (kind == 3u && (self->field_07 & 3u) == 3u) flag = 1;
    if (flag) return (CItemImpl*)&lbl_eu_806641F8;
    // u16 index: MWCC keeps the retail's clrlslwi 16-bit mask for the
    // table load (a u32 index with proven range gets mask 0-29 instead).
    return lbl_eu_8052F67C[kind];
}

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

// Refresh an item slot: reset the four u16 entry flags and the count, then
// when the packed word has a family id resolve the per-kind BDAT handle
// (kind 2 / 4..8), re-derive the item-name byte, and rebuild the 8-byte
// records at field_08 from the +0x4A/+0x51 columns ('1'.. suffixed name
// column).
void func_80156350(u32 unused, CItemExt* self) {
    u32 word = self->field_00;
    self->mEntries[0] = -1;
    self->mEntries[1] = -1;
    self->mEntries[2] = -1;
    self->mEntries[3] = -1;
    self->mCount = 0;
    if ((self->field_00 >> 20) != 0) {
        u32 kind = (self->field_00 >> 16) & 0xF;
        void* h = 0;
        if (kind == 2) {
            h = lbl_eu_806640F4;
        } else {
            // Materialized boolean: nested ifs keep the retail's two
            // cmplwi (MWCC folds `x >= 4 && x <= 8` into subi+cmplwi).
            u32 b = 0;
            if (kind >= 4) {
                if (kind <= 8) {
                    b = 1;
                }
            }
            if (b) {
                h = lbl_eu_806640F8;
            }
        }
        if (h != 0) {
            union {
                u32 v;
                u8 b[4];
            } t;
            t.v = getBdatStringColumnValue(h, lbl_eu_80501C58 + 0x5a,
                                           (self->field_00 >> 5) & 0x7FF);
            self->mCount = t.b[0];
        }
    }
    if ((self->field_00 >> 20) != 0) {
        s32 idx = strlen(lbl_eu_80662298) - 1;
        u32 kind = (self->field_00 >> 16) & 0xF;
        u32 row = (self->field_00 >> 5) & 0x7FF;
        void* handle = 0;
        s32 bound = 0;
        if (kind == 2) {
            handle = lbl_eu_806640F4;
            bound = 3;
        } else {
            u32 b = 0;
            if (kind >= 4) {
                if (kind <= 8) {
                    b = 1;
                }
            }
            if (b) {
                handle = lbl_eu_806640F8;
                bound = 1;
            }
        }
        if (bound != 0) {
            for (s32 i = 0; i < bound; i++) {
                char* col = lbl_eu_80662298;
                col[idx] = (char)(0x31 + i);
                union {
                    u32 v;
                    u16 h[2];
                } t;
                t.v = getBdatStringColumnValue(handle, col, row);
                if (t.h[0] != 0) {
                    // Retail re-checks the same value (second beq on the
                    // same CR) before the name-column lookup.
                    u16 v2 = 0;
                    if (t.h[0] != 0) {
                        union {
                            u32 v;
                            u16 h[2];
                        } t3;
                        t3.v = getBdatStringColumnValue(lbl_eu_806640EC,
                                                        lbl_eu_80501C58 + 0x4a,
                                                        t.h[0]);
                        v2 = t3.h[0];
                    }
                    union {
                        u32 v;
                        u8 b[4];
                    } t4;
                    t4.v = getBdatStringColumnValue(lbl_eu_806640EC,
                                                    lbl_eu_80501C58 + 0x51,
                                                    t.h[0]);
                    func_801558B4((CItemRec*)&self->field_08[i * 8],
                                  t.h[0], v2, 1, t4.b[0]);
                }
            }
        }
    }
}

// Refresh an item slot from its BDAT row (same family as func_80156934):
// resolve the family's kind/row ids, refresh the +0x16 name byte and the
// field_07 name bits, roll a random amount for the impl state (vf0C), then
// build a shuffled 8-entry slot table (slot ids offset by the category's
// base, shuffled via mtRand pairs) and hand the first four entries to
// vf50/vf68 with a per-entry random amount in [min, max).
extern "C" void func_8015650C(u32 unused, CItemData* self) {
    u16 tbl[0x100];  // 0x28 (only [0..7] is filled/shuffled; retail frame reserves the full 0x200)
    union { u32 v; u8 b[4]; } t1;  // 0x20
    union { u32 v; u8 b[4]; } t2;  // 0x1c
    union { u32 v; u8 b[4]; } t3;  // 0x18
    union { u32 v; u8 b[4]; } t4;  // 0x14
    union { u32 v; u8 b[4]; } t5;  // 0x10
    union { u32 v; u8 b[4]; } t6;  // 0x0c
    u16 kind;   // 0x0a
    u16 row;    // 0x08
    void* handle = func_80157F04(self->field_00 >> 20, &kind, &row);
    t1.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x64, row);
    u32 byte1 = t1.b[0];
    self->field_08[0xE] = (u8)byte1;
    t2.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x6d, row);
    u32 f7 = self->field_07;
    f7 = __rlwimi(f7, t2.b[0], 2, 24, 29);
    self->field_07 = (u8)f7;
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x76, row);
    u32 byte3 = t3.b[0];
    t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x84, row);
    u32 byte4 = t4.b[0];
    u32 rnd;
    if (byte3 != byte4) {
        rnd = (u16)ml::math::mtRand(byte3, byte4 + 1);
    } else {
        rnd = byte3;
    }
    CItem_initItemImplInstances(self)->vf0C(self, (u16)rnd);
    t5.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x6d, row);
    u32 byte5 = t5.b[0];
    t6.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x91, row);
    u32 rnd2 = (u16)ml::math::mtRand(1, t6.b[0] + 1);
    u32 idx = (byte1 == 2) ? 2 : 0;
    if (ml::math::mtRand(100) < 25) idx++;
    u8 min = lbl_eu_80662290[2 * idx];
    u8 max = lbl_eu_80662290[2 * idx + 1];
    s32 slot = 0;
    s32 total = 0;
    // if-else-if chain: retail lowers the per-category dispatch as bne-skip
    // case bodies (a switch statement would lower to beq-to-body instead).
    if (byte5 == 4) {
        slot = 1; total = 8;
    } else if (byte5 == 5) {
        slot = 0x11; total = 0x18;
    } else if (byte5 == 6) {
        slot = 0x21; total = 0x28;
    } else if (byte5 == 7) {
        slot = 0x31; total = 0x38;
    } else if (byte5 == 8) {
        slot = 0x41; total = 0x48;
    } else if (byte5 == 9) {
        slot = 0x51; total = 0x58;
    }
    s32 count = total - slot;
    for (s32 i = 0; i <= count; i++) {
        tbl[i] = (u16)(slot + i);
    }
    for (s32 i = 0; i <= count; i++) {
        u32 a = ml::math::mtRand(count + 1);
        u32 b = ml::math::mtRand(count + 1);
        u16 tmp = tbl[a];
        tbl[a] = tbl[b];
        tbl[b] = tmp;
    }
    for (s32 i = 0; i < 4; i++) {
        if (i < (s32)rnd2) {
            u16 slotId = tbl[i];
            u32 rnd3 = (min < max) ? (u32)ml::math::mtRand(min, max) : max;
            CItem_initItemImplInstances(self)->vf50(self, i, slotId);
            CItem_initItemImplInstances(self)->vf68(self, i, (u16)rnd3);
        } else {
            CItem_initItemImplInstances(self)->vf50(self, i, 0);
            CItem_initItemImplInstances(self)->vf68(self, i, 0);
        }
    }
}

void func_80156924() {}

void func_80156928() {}

u8 CItemInfo_getByte22(u32, CItemInfo* obj) { return *(u8*)((char*)obj + 22); }

// Refresh an item slot from its BDAT row: query the per-category impl state
// (vf78/vf0C/vf84), clear the u16 at +0x10, set the +0x16 byte, pack the
// +0x6D name byte into field_07, then derive two u16 halfword columns
// (+0xA9/+0xB0) and a slot-size roll: a boolean plus a 25% chance picks a
// (min, max) pair from the lbl_eu_80662290 table, and each of the two
// halfword entries is initialized via vf50/vf68 with a random amount in
// [min, max) (halved for the second entry).
void func_80156934(u32 unused, CItemData* self) {
    u16 halfs[2];
    union {
        u32 v;
        u8 b[4];
    } t1;
    union {
        u32 v;
        u8 b[4];
    } t2;
    union {
        u32 v;
        u8 b[4];
    } t3;
    union {
        u32 v;
        u8 b[4];
    } t4;
    union {
        u32 v;
        u8 b[4];
    } t5;
    union {
        u32 v;
        u8 b[4];
    } t6;
    u32 word = self->field_00;
    void* handle = lbl_eu_80664100;
    u32 row = (word >> 5) & 0x7FF;
    u32 n = CItem_initItemImplInstances(self)->vf78(self);
    *(u16*)&self->field_08[8] = 0;
    t1.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x9b, row);
    CItem_initItemImplInstances(self)->vf0C(self, t1.b[0]);
    t2.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0xa0, row);
    CItem_initItemImplInstances(self)->vf84(self, t2.b[0]);
    self->field_08[0xE] = 1;
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x6d, row);
    u32 f7 = self->field_07;
    f7 = __rlwimi(f7, t3.b[0], 2, 24, 29);
    self->field_07 = (u8)f7;
    // Duplicate lookup kept: retail emits a second getBdatStringColumnValue
    // call with identical args whose result is discarded (side-effect call).
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x6d, row);
    t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0xa9, row);
    halfs[0] = (u16)t4.b[0];
    t5.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0xb0, row);
    halfs[1] = (u16)t5.b[0];
    t6.v = getBdatStringColumnValue(lbl_eu_806640CC, lbl_eu_80501C58 + 0xb7, (u16)n);
    u32 b0 = t6.b[0];
    u32 idx = (b0 != 0);
    if (ml::math::mtRand(100) < 25) idx++;
    u8 min = lbl_eu_80662290[2 * idx];
    u8 max = lbl_eu_80662290[2 * idx + 1];
    for (s32 i = 0; i < 4; i++) {
        if (halfs[i] == 0 || i >= 2) {
            CItem_initItemImplInstances(self)->vf50(self, i, 0);
            CItem_initItemImplInstances(self)->vf68(self, i, 0);
        } else {
            u16 rnd = (min < max) ? (u16)ml::math::mtRand(min, max) : max;
            CItem_initItemImplInstances(self)->vf50(self, i, halfs[i]);
            if (i == 1) {
                CItem_initItemImplInstances(self)->vf68(self, i, (rnd >> 1) & 0x7FFF);
            } else {
                CItem_initItemImplInstances(self)->vf68(self, i, rnd);
            }
        }
    }
}

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

// Set or clear the family flag for kind v: set (or clear) bit (1<<v) in the
// word at block+0x1210C (kind 2) or the u16 at block+0x12114/0x12116/
// 0x12118/0x1211A/0x1211C (kinds 4-8). Returns 1 when the operation changed
// the flag's state (set path: the bit was clear; clear path: it was set),
// 0 for an out-of-range kind or an invalid flag id.
u32 func_8015730C(u32 v, u32 set, u32 kind) {
    u32 result = 0;
    if (v >= 1) {
        if (v <= 0xb) {
            u32 bit = 1u << v;
            if (set != 0) {
            if (kind == 2) {
                u32* w = (u32*)((u8*)lbl_eu_806641B8 + 0x1210c);
                if ((*w & bit) == 0) result = 1;
                *w |= bit;
                return result;
            }
            if (kind == 4) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x12114);
                if ((*w & bit) == 0) result = 1;
                *w = (u16)(*w | bit);
                return result;
            }
            if (kind == 5) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x12116);
                if ((*w & bit) == 0) result = 1;
                *w = (u16)(*w | bit);
                return result;
            }
            if (kind == 6) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x12118);
                if ((*w & bit) == 0) result = 1;
                *w = (u16)(*w | bit);
                return result;
            }
            if (kind == 7) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x1211a);
                if ((*w & bit) == 0) result = 1;
                *w = (u16)(*w | bit);
                return result;
            }
            if (kind == 8) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x1211c);
                if ((*w & bit) == 0) result = 1;
                *w = (u16)(*w | bit);
                return result;
            }
        } else {
            if (kind == 2) {
                u32* w = (u32*)((u8*)lbl_eu_806641B8 + 0x1210c);
                if ((*w & bit) != 0) result = 1;
                *w &= ~bit;
                return result;
            }
            if (kind == 4) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x12114);
                if ((*w & bit) != 0) result = 1;
                *w = (u16)(*w & ~bit);
                return result;
            }
            if (kind == 5) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x12116);
                if ((*w & bit) != 0) result = 1;
                *w = (u16)(*w & ~bit);
                return result;
            }
            if (kind == 6) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x12118);
                if ((*w & bit) != 0) result = 1;
                *w = (u16)(*w & ~bit);
                return result;
            }
            if (kind == 7) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x1211a);
                if ((*w & bit) != 0) result = 1;
                *w = (u16)(*w & ~bit);
                return result;
            }
            if (kind == 8) {
                u16* w = (u16*)((u8*)lbl_eu_806641B8 + 0x1211c);
                if ((*w & bit) != 0) result = 1;
                *w = (u16)(*w & ~bit);
                return result;
            }
        }
        }
    }
    return result;
}

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

extern "C" void* func_801579C4(u32 arg, s32* out1, s32* out2) {
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

// Find a free record in the item-block list for arg. For kinds 10-13 the
// search first prefers a family-matching record with room (field_06 < 0x63):
// a matching record already fully used aborts with 0, otherwise the
// free-record search runs. For other kinds a record is only taken when the
// kind-2..8 randomizer has free capacity. The found record index is written
// to *pOut.
CItemExt* func_80157D6C(u32 arg, s16* pOut, u32 family) {
    *pOut = -1;
    s32 count;
    s32 stride;
    CItemExt* list = (CItemExt*)func_801579C4(arg, &count, &stride);
    if (list != 0) {
        // Mixed operand forms (truncated vs full) keep the retail's two
        // signed cmpi; identical operands make MWCC fold the range check
        // into subi+cmplwi.
        u32 flag = 0;
        if ((int)(u16)arg >= 10 && (int)arg <= 13) flag = 1;
        if (flag) {
            // Pass 1: prefer a family-matching record with capacity.
            for (s32 i = 0; i < count; i++) {
                CItemExt* rec = (CItemExt*)((char*)list + i * stride);
                u32 word = rec->field_00;
                if (family == (word >> 20) && rec->field_06 < 0x63) {
                    if (word == 0) return 0;
                    *pOut = (s16)i;
                    return rec;
                }
            }
            // Pass 2: any zero record.
            for (s32 i = 0; i < count; i++) {
                CItemExt* rec = (CItemExt*)((char*)list + i * stride);
                if (rec->field_00 == 0) {
                    *pOut = (s16)i;
                    return rec;
                }
            }
        } else {
            if ((u32)(arg - 2) <= 6 && func_80157CD0(arg) == 0) return 0;
            for (s32 i = 0; i < count; i++) {
                CItemExt* rec = (CItemExt*)((char*)list + i * stride);
                if (rec->field_00 == 0) {
                    *pOut = (s16)i;
                    return rec;
                }
            }
        }
    }
    return 0;
}

// Retail signature recovered from the func_80155DBC/func_80155E30 call
// sites: family/row id in r3, two out pointers, BDAT handle in r3. Body not
// yet recovered (not a target); noinline keeps the retail `bl` at the two
// call sites above (MWCC would fold the trivial body). C linkage to match
// the header declaration (plain-name reloc). The first param is u32 so call
// sites pass the id unmasked (retail mr, not clrlwi).
extern "C" __declspec(noinline) void* func_80157F04(u32 v, void* outA, void* outB) { return 0; }

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
// Initialise an item slot record: resolve the kind/row ids for the family
// via func_80157F04, clear a kind-sized record, pack the family/kind/row
// ids into the record word, restore the u16 at +0x04, then dispatch on the
// kind to set the +0x06 count byte and refresh the category impl state.
extern "C" void __declspec(noinline) func_80158118(CItemData* self, u32 a, u32 b) {
    u16 kind;
    u16 row;
    func_80157F04(a, &kind, &row);
    u16 old04 = *(u16*)&self->field_04;
    s32 size = func_80156F54(kind);
    if (self != 0 && size != 0) {
        memset(self, 0, (u32)size);
    }
    self->field_00 = __rlwimi(self->field_00, a, 20, 0, 11);
    self->field_00 = __rlwimi(self->field_00, kind, 16, 12, 15);
    self->field_00 = __rlwimi(self->field_00, row, 5, 16, 26);
    *(u16*)&self->field_04 = old04;
    if (kind == 1) {
        self->field_06 = (u8)b;
    } else if (kind == 2) {
        self->field_06 = 1;
        CItem_initItemImplInstances(self)->vf3C(self, 6);
    } else if (kind >= 4 && kind <= 8) {
        self->field_06 = 1;
        u32 sub = 0;
        if (kind == 4) {
            sub = 1;
        } else if (kind == 5) {
            sub = 2;
        } else if (kind == 6) {
            sub = 3;
        } else if (kind == 7) {
            sub = 4;
        } else if (kind == 8) {
            sub = 5;
        }
        CItem_initItemImplInstances(self)->vf3C(self, sub);
    } else if (kind == 9) {
        self->field_06 = (u8)b;
    } else if (kind == 3) {
        self->field_06 = (u8)b;
    } else if (kind == 0xa) {
        self->field_06 = (u8)b;
    } else if (kind == 0xb) {
        self->field_06 = (u8)b;
    } else if (kind == 0xc) {
        self->field_06 = (u8)b;
    } else if (kind == 0xd) {
        self->field_06 = (u8)b;
    }
}

void func_801582FC() {}

void func_80158300(){}

void func_801583DC() {}

// Enable the family flag: set low 2 bits of byte 7 (ItemByte7::b01).
void func_801583E0(CItemData* self, u32 a) {
    func_80158118(self, a, 1);
    ((ItemByte7*)&self->field_07)->b01 = 1;
}

// Create/locate an item record for the family id: when the +0xBD name column
// is in [0x0A, 0x0D] the record is located by family scan (falling back to a
// free record), otherwise a free record is found via func_80157D6C. The
// amount (*pOut2) and the record's per-kind rank fields are bumped, the
// record is (re)initialized via func_80158118, and kind-3 records get their
// +0x42 name byte re-derived through the impl vtable. Returns the record.
extern "C" __declspec(noinline) CItemExt* func_80158420(u32 a, s16* pOut1, u32 b, u32* pOut2) {
    *pOut1 = -1;
    *pOut2 = 0;
    if (a == 0) return 0;
    union {
        u32 v;
        u16 h[2];
    } t1;
    t1.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, a);
    u32 flag = 0;
    if ((u32)(t1.h[0] - 10) <= 3) flag = 1;
    CItemExt* result;
    if (flag) {
        // Duplicate lookup kept: retail calls the +0xBD column twice.
        union {
            u32 v;
            u16 h[2];
        } t2;
        t2.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0xbd, a);
        result = 0;
        if (t2.h[0] != 0) {
            s32 n, stride;
            CItemExt* list = (CItemExt*)func_801579C4(t2.h[0], &n, &stride);
            if (list != 0) {
                for (s32 i = 0; i < n; i++) {
                    CItemExt* r = (CItemExt*)((u8*)list + i * stride);
                    if (a == (r->field_00 >> 20)) {
                        result = r;
                        break;
                    }
                }
            }
        }
        if (result == 0) {
            result = func_80157D6C(t1.h[0], pOut1, a);
        }
    } else {
        result = func_80157D6C(t1.h[0], pOut1, a);
    }
    if (result != 0) {
        u32 count;
        if (flag) {
            s32 rem = 99 - result->field_06;
            if (rem == 0) return 0;
            if ((s32)b > rem) {
                count = 99;
                *pOut2 += (u32)rem;
            } else {
                count = b + result->field_06;
                *pOut2 += b;
            }
        } else {
            count = 1;
            *pOut2 = 1;
        }
        // Bump the rank field of every non-empty record in the kind list.
        for (u32 kind = 2; kind < 14; kind++) {
            s32 n, stride;
            func_801579C4(kind, &n, &stride);
            for (s32 i = 0; i < n; i++) {
                CItemRec* rec = (CItemRec*)func_80157C4C(kind, (s16)i);
                if (rec != 0 && rec->field_00 != 0 && rec->field_04 < 0xffffu) {
                    rec->field_04++;
                }
            }
        }
        func_80158118((CItemData*)result, a, count);
        ((CItemRec*)result)->field_04 = 0;
    }
    if (result != 0) {
        CItem_initItemImplInstances((CItemData*)result)->vf1C((CItemData*)result);
        if (((result->field_00 >> 16) & 0xF) == 3) {
            union {
                u32 v;
                u8 b[4];
            } t3;
            t3.v = getBdatStringColumnValue(lbl_eu_806640EC, lbl_eu_80501C58 + 0x42, a);
            if (t3.b[0] != 0) {
                CItem_initItemImplInstances((CItemData*)result)->vf94((CItemData*)result, t3.b[0]);
            }
        }
    }
    return result;
}

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
int func_80158894(u16 arg, u16* out, s32 capacity) {
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

// Introsort over the pointer range [base, end): small ranges (<= 20
// elements) are selection-sorted; larger ranges pick a median-of-3 pivot
// (two depth-jittered sample points plus the last element, sorted via
// func_801591F4), Hoare-partition around the last element, recurse on the
// smaller half through func_80158E74 and keep looping on the larger half.
// The depth counter (lbl_eu_8066229C) jitters the sample points to avoid
// quadratic behavior on sorted input.
extern "C" void func_80158AF4(CItemFamilyRec** base, CItemFamilyRec** end,
                              u32 (*cmp)(CItemFamilyRec*, CItemFamilyRec*)) {
    while (true) {
        s32 count = ((char*)end - (char*)base) / 4;
        if (count <= 1) return;
        if (count <= 20) {
            // Selection sort: put the minimum of [p, end) at p.
            CItemFamilyRec** last = end - 1;
            CItemFamilyRec** p = base;
            while (p != last) {
                CItemFamilyRec** min = p;
                for (CItemFamilyRec** q = p + 1; q != end; q++) {
                    if (cmp(*q, *min) != 0) min = q;
                }
                if (min != p) {
                    CItemFamilyRec* t = *min;
                    *min = *p;
                    *p = t;
                }
                p++;
            }
            return;
        }
        // Pivot selection: two depth-jittered sample points + the last
        // element, sorted so the last element becomes the pivot value.
        s32 depth = lbl_eu_8066229C;
        s32 off1 = (count / 4) + (depth - 5 * (depth / 10));
        CItemFamilyRec** p1 = base + off1;
        s32 x = depth + 1;
        if (x >= 5) x = -4;
        s32 off2 = ((3 * count) / 4) + (x - 5 * (x / 10));
        CItemFamilyRec** p2 = base + off2;
        lbl_eu_8066229C = x + 1;
        if (lbl_eu_8066229C >= 5) lbl_eu_8066229C = -4;
        CItemFamilyRec** pivot = end - 1;
        func_801591F4((u32*)p1, (u32*)p2, (u32*)pivot,
                      (int (**)(u32*, u32*))&cmp);
        // Hoare partition around *pivot.
        CItemFamilyRec** i = base;
        CItemFamilyRec** j = pivot;
        do {
            i++;
        } while (cmp(*i, *pivot) != 0);
        do {
            j--;
            if (i == j) break;
        } while (cmp(*j, *pivot) == 0);
        while (i < j) {
            CItemFamilyRec* t = *i;
            *i = *j;
            *j = t;
            i++;
            while (cmp(*i, *pivot) != 0) i++;
            do {
                j--;
            } while (cmp(*j, *pivot) == 0);
        }
        if (i == base) {
            // The pivot ended up at the start: move it there and repartition
            // the rest with the pivot value at base.
            CItemFamilyRec* t = *i;
            *i = *pivot;
            *pivot = t;
            i++;
            j = end - 1;
            if (cmp(*base, *j) == 0) {
                while (i != end && cmp(*base, *i) == 0) i++;
                if (i < j) {
                    CItemFamilyRec* u = *i;
                    *i = *j;
                    *j = u;
                }
            }
            while (i < j) {
                while (cmp(*base, *i) == 0) i++;
                do {
                    j--;
                } while (cmp(*base, *j) != 0);
                if (i < j) {
                    CItemFamilyRec* u = *i;
                    *i = *j;
                    *j = u;
                    i++;
                }
            }
            base = i;
        } else {
            s32 left = ((char*)i - (char*)base) / 4;
            s32 right = ((char*)end - (char*)i) / 4;
            if (left < right) {
                func_80158E74(base, i, &cmp);
                base = i;
            } else {
                func_80158E74(i, end, &cmp);
                end = i;
            }
        }
    }
}

// Recursive half-sort helper (twin of func_80158AF4): introsort over the
// pointer range [base, end). Small ranges (<= 20 elements) are selection-
// sorted; larger ranges pick a median-of-3 pivot (two depth-jittered sample
// points plus the last element, sorted via func_801591F4), Hoare-partition
// around the last element, recurse on the smaller half and keep looping on
// the larger half. Uses its own depth counter (lbl_eu_806622A0) and its
// own recursion target (itself).
extern "C" void __declspec(noinline) func_80158E74(CItemFamilyRec** base, CItemFamilyRec** end,
                                                    u32 (**cmp)(CItemFamilyRec*, CItemFamilyRec*)) {
    while (true) {
        s32 count = ((char*)end - (char*)base) / 4;
        if (count <= 1) return;
        if (count > 20) goto partition;
        // Selection sort: put the minimum of [p, end) at p.
        CItemFamilyRec** last = end - 1;
        if (base != end) {
            for (CItemFamilyRec** p = base; p != last; p++) {
                CItemFamilyRec** min = p;
                if (p != end) {
                    for (CItemFamilyRec** q = p + 1; q != end; q++) {
                        if ((*cmp)(*q, *min) != 0) min = q;
                    }
                }
                if (min != p) {
                    CItemFamilyRec* t = *min;
                    *min = *p;
                    *p = t;
                }
            }
        }
        return;
    partition:
        // Quicksort: pivot selection with two depth-jittered sample points +
        // the last element, sorted so the last element becomes the pivot.
        s32 depth = lbl_eu_806622A0;
        s32 off1 = (count / 4) + (depth - 5 * (depth / 10));
        CItemFamilyRec** p1 = base + off1;
        s32 x = depth + 1;
        if (x >= 5) x = -4;
        s32 off2 = ((3 * count) / 4) + (x - 5 * (x / 10));
        CItemFamilyRec** p2 = base + off2;
        lbl_eu_806622A0 = x + 1;
        if (lbl_eu_806622A0 >= 5) lbl_eu_806622A0 = -4;
        CItemFamilyRec** pivot = end - 1;
        func_801591F4((u32*)p1, (u32*)p2, (u32*)pivot,
                      (int (**)(u32*, u32*))cmp);
        // Hoare partition around *pivot.
        CItemFamilyRec** i = base;
        CItemFamilyRec** j = pivot;
        while ((*cmp)(*i, *pivot) != 0) i++;
        do {
            j--;
            if (i == j) break;
        } while ((*cmp)(*j, *pivot) == 0);
        while (i < j) {
            CItemFamilyRec* t = *i;
            *i = *j;
            *j = t;
            i++;
            while ((*cmp)(*i, *pivot) != 0) i++;
            do {
                j--;
            } while ((*cmp)(*j, *pivot) == 0);
        }
        if (i == base) {
            // The pivot ended up at the start: move it there and
            // repartition the rest with the pivot value at base.
            CItemFamilyRec* t = *i;
            *i = *pivot;
            *pivot = t;
            i++;
            j = end - 1;
            if ((*cmp)(*base, *j) == 0) {
                while (i != end && (*cmp)(*base, *i) == 0) i++;
                if (i < j) {
                    CItemFamilyRec* u = *i;
                    *i = *j;
                    *j = u;
                }
            }
            while (i < j) {
                while ((*cmp)(*base, *i) == 0) i++;
                do {
                    j--;
                } while ((*cmp)(*base, *j) != 0);
                if (i < j) {
                    CItemFamilyRec* u = *i;
                    *i = *j;
                    *j = u;
                    i++;
                }
            }
            base = i;
        } else {
            s32 left = ((char*)i - (char*)base) / 4;
            s32 right = ((char*)end - (char*)i) / 4;
            if (left < right) {
                func_80158E74(base, i, cmp);
                base = i;
            } else {
                func_80158E74(i, end, cmp);
                end = i;
            }
        }
    }
}

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

// Refresh the party item-slot tables for all 13 characters: for each
// character id, clear the three per-character u16 tables and the six equip
// slots, then for each valid slot resolve the item record, copy its
// (item id, slot, sub-index) entries into the parallel arrays and re-sync
// the slot through the row-table helpers.
void func_80159348(CItemPartySlots* self) {
    s16* slots = &self->mSlots[0][0];
    // Base pointer for the three parallel arrays: retail materialises
    // self+0x10 (== &self->mSlots[0][2]) and walks it by 8 halfwords per
    // character, addressing mArr1/2/3 at displacements 0x9c/0x16c/0x23c.
    s16* p = &self->mSlots[0][2];
    for (s32 id = 1; id <= 13; id++) {
        u8* cd = (u8*)func_8009EC9C((u16)id);
        // Clear the three parallel per-character u16 tables (8 entries each).
        for (s32 k = 0; k <= 7; k++) {
            p[0x4E + k] = -1;    // mArr1[id-1][k]
            p[0xB6 + k] = -1;    // mArr2[id-1][k]
            p[0x11E + k] = -1;   // mArr3[id-1][k]
        }
        u32 m = 0;
        for (s32 s = 0; s <= 5; s++) {
            u16 t;
            slots[s] = -1;
            if (((const s16*)cd)[0xe + s] > -1) {
                CItemData* rec =
                    (CItemData*)func_8009D790((s16*)&cd[0x1c], (u32)s);
                if (rec->field_00 == 0) {
                    func_8009DBF4(cd, (u32)s, (void*)-1);
                } else {
                    u16 count =
                        (u16)CItem_initItemImplInstances(rec)->vf30(rec);
                    for (s32 k = 0; k < count; k++) {
                        u32 v = CItem_initItemImplInstances(rec)->vf40(rec, (u32)k);
                        if ((s16)v != -1) {
                            p[0x4E + m] = (s16)v;
                            p[0xB6 + m] = (s16)s;
                            p[0x11E + m] = (s16)k;
                            m++;
                        }
                    }
                }
                CItem_initItemImplInstances(rec)->vf48(rec);
            }
            t = *(u16*)&cd[(u16)s * 2 + 2];
            slots[s] = (s16)func_8009E0B4(cd, (u32)s, &cd[(u16)s * 2]);
            func_8009DBF4(cd, (u32)s, (void*)-1);
            func_8009E0C4(cd, (u16)s, t);
        }
        slots += 6;
        p += 8;
    }
}

// Rebuild the item-box tables: refresh the per-kind impl state for every
// non-empty record of kinds 2,4,5,6,7,8 (vf48), compact each kind's record
// list into the scratch buffer via func_80158894 (kinds 3 and 9..13 into
// the stack table), then for each character re-sync the six equip slots
// against the compacted indices and the party slot arrays (mArr1/mArr2/
// mArr3), handing each (amount, item id) pair to the kind-impl vf44.
extern "C" s32 func_80159524() {
    func_8007F11C__Q22cf13CfGameManagerFv();
    s16* buf = (s16*)mtl::MemManager::allocate_head(
        mtl::MemManager::getHandleMEM2(), 0x960, 4);
    CItemPartySlots slots;  // 0x290
    u16 out9[0x12c];        // 0x38
    func_80159348(&slots);
    // Refresh the per-kind impl state for the six item categories.
    s32 stride2; s32 count2;   // 0x34, 0x30
    s32 stride4; s32 count4;   // 0x2c, 0x28
    s32 stride5; s32 count5;   // 0x24, 0x20
    s32 stride6; s32 count6;   // 0x1c, 0x18
    s32 stride7; s32 count7;   // 0x14, 0x10
    s32 stride8; s32 count8;   // 0x0c, 0x08
    CItemExt* list2 = (CItemExt*)func_801579C4(2, &count2, &stride2);
    if (list2 != 0) {
        for (s32 i = 0; i < count2; i++) {
            CItemExt* rec = (CItemExt*)((char*)list2 + stride2 * i);
            // Retail loads field_00 via the raw index (lwzx list+stride*i).
            if (*(u32*)((char*)list2 + stride2 * i) != 0) {
                CItem_initItemImplInstances((CItemData*)rec)->vf48((CItemData*)rec);
            }
        }
    }
    CItemExt* list4 = (CItemExt*)func_801579C4(4, &count4, &stride4);
    if (list4 != 0) {
        for (s32 i = 0; i < count4; i++) {
            CItemExt* rec = (CItemExt*)((char*)list4 + stride4 * i);
            if (*(u32*)((char*)list4 + stride4 * i) != 0) {
                CItem_initItemImplInstances((CItemData*)rec)->vf48((CItemData*)rec);
            }
        }
    }
    CItemExt* list5 = (CItemExt*)func_801579C4(5, &count5, &stride5);
    if (list5 != 0) {
        for (s32 i = 0; i < count5; i++) {
            CItemExt* rec = (CItemExt*)((char*)list5 + stride5 * i);
            if (*(u32*)((char*)list5 + stride5 * i) != 0) {
                CItem_initItemImplInstances((CItemData*)rec)->vf48((CItemData*)rec);
            }
        }
    }
    CItemExt* list6 = (CItemExt*)func_801579C4(6, &count6, &stride6);
    if (list6 != 0) {
        for (s32 i = 0; i < count6; i++) {
            CItemExt* rec = (CItemExt*)((char*)list6 + stride6 * i);
            if (*(u32*)((char*)list6 + stride6 * i) != 0) {
                CItem_initItemImplInstances((CItemData*)rec)->vf48((CItemData*)rec);
            }
        }
    }
    CItemExt* list7 = (CItemExt*)func_801579C4(7, &count7, &stride7);
    if (list7 != 0) {
        for (s32 i = 0; i < count7; i++) {
            CItemExt* rec = (CItemExt*)((char*)list7 + stride7 * i);
            if (*(u32*)((char*)list7 + stride7 * i) != 0) {
                CItem_initItemImplInstances((CItemData*)rec)->vf48((CItemData*)rec);
            }
        }
    }
    CItemExt* list8 = (CItemExt*)func_801579C4(8, &count8, &stride8);
    if (list8 != 0) {
        for (s32 i = 0; i < count8; i++) {
            CItemExt* rec = (CItemExt*)((char*)list8 + stride8 * i);
            if (*(u32*)((char*)list8 + stride8 * i) != 0) {
                CItem_initItemImplInstances((CItemData*)rec)->vf48((CItemData*)rec);
            }
        }
    }
    // Compact each kind's record list into the scratch buffer (the third
    // argument is the out-table capacity; the callee ignores it).
    func_80158894(3, (u16*)buf, 0x12c);
    func_80158894(2, (u16*)buf + 0x258 / 2, 0x96);
    func_80158894(4, (u16*)buf + 0x384 / 2, 0x96);
    func_80158894(5, (u16*)buf + 0x4b0 / 2, 0x96);
    func_80158894(6, (u16*)buf + 0x5dc / 2, 0x96);
    func_80158894(7, (u16*)buf + 0x708 / 2, 0x96);
    func_80158894(8, (u16*)buf + 0x834 / 2, 0x96);
    for (s32 k = 9; k < 14; k++) {
        func_80158894((u16)k, out9, 0x12c);
    }
    // Re-sync every character's equip slots against the compacted tables.
    for (s32 id = 1; id <= 13; id++) {
        u8* cd = (u8*)func_8009EC9C((u16)id);
        s16* p = &slots.mSlots[id - 1][0];
        s16* pb = &slots.mSlots[id - 1][2];
        for (s32 i = 0; i <= 5; i++) {
            s16 id2 = p[i];
            if (id2 == -1) continue;
            s16 slotVal = -1;
            // if-else-if chain: retail lowers this dispatch as bne-skip case
            // bodies (a switch would lower to beq-to-body instead).
            if (i == 0) {
                slotVal = buf[0x384 / 2 + id2];
            } else if (i == 1) {
                slotVal = buf[0x4b0 / 2 + id2];
            } else if (i == 2) {
                slotVal = buf[0x5dc / 2 + id2];
            } else if (i == 3) {
                slotVal = buf[0x708 / 2 + id2];
            } else if (i == 4) {
                slotVal = buf[0x834 / 2 + id2];
            } else if (i == 5) {
                slotVal = buf[0x258 / 2 + id2];
            }
            func_8009DBF4(cd, (u32)i, (void*)(s32)slotVal);
            CItemData* rec = (CItemData*)func_8009D790((s16*)&cd[0x1c], (u32)i);
            for (s32 j = 0; j < 8; j++) {
                if (pb[0xb6 + j] == (s16)i) {
                    s16 slot2 = pb[0x4e + j];
                    if (slot2 != -1) {
                        CItem_initItemImplInstances(rec)->vf44(
                            rec, (u32)pb[0x11e + j], (u32)buf[slot2]);
                    }
                }
            }
        }
    }
    if (buf != 0) mtl::MemManager::deallocate(buf);
    func_8007F11C__Q22cf13CfGameManagerFv();
    return 1;
}

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
    // Saved-register order matters: declared first -> highest (i->r31,
    // n->r30, kind->r29, r->r28).
    s32 i;       // inner loop counter
    s32 n;       // count cache (count's address escapes to func_801579C4)
    s32 kind;    // outer loop counter
    CItemExt* r;
    if (flag) {
        r = func_80158420(self->field_00 >> 20, &out1, self->field_06, &out2);
        if (pOut != 0) *pOut = out1;
    } else {
        r = func_80158700(self, &stride, &out1);
        if (r != 0) {
            // Bump the rank field of every non-empty family-kind record so
            // the re-ranked families stay in sync with the item block.
            for (kind = 2; kind < 14; kind++) {
                func_801579C4(kind, &count, &stride2);
                n = count;
                for (i = 0; i < n; i++) {
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
        CItemExt* rec;
        if (t2.h[0] == 0) {
            rec = 0;
        } else {
            void* list = func_801579C4(t2.h[0], &n, &stride);
            if (list != 0) {
                u8* base = (u8*)list;
                for (s32 i = 0; i < n; i++) {
                    CItemExt* r = (CItemExt*)(base + i * stride);
                    if (family == (r->field_00 >> 20)) {
                        rec = r;
                        goto found;
                    }
                }
            }
            rec = 0;
        }
found:
        if (rec == 0) break;
        if (flag) {
            u8 v6 = rec->field_06;
            if (count >= v6) {
                count -= v6;
                CItem_initItemImplInstances((CItemData*)rec)->vf10((CItemData*)rec);
            } else {
                rec->field_06 = (u8)(v6 - count);
                count = 0;
            }
        } else {
            CItem_initItemImplInstances((CItemData*)rec)->vf10((CItemData*)rec);
            count--;
        }
    }
    return count;
}

// Build the sort-scratch item-name string: reset the buffer, copy the
// per-family item-name prefix, then resolve the three name parts (family
// name, item name, sub-name) from BDAT columns and concatenate them in a
// console-language-specific order (some languages weave the prefix in
// between the parts). The running lengths are tracked per stage and the
// final length is written to buf->field_40.
extern "C" __declspec(noinline) void func_80159D74(CItemSortBuf* buf, u32 a,
                                                    u32 b, u32 c, u32 d,
                                                    u32 e, u32 f) {
    CItemNameBuf s1;      // 0x628
    CItemNameBuf s2;      // 0x5e4
    CItemNameBuf s3;      // 0x5a0
    CItemNameBuf prefix;  // 0x55c
    CItemNameBuf t0[2];   // 0x4d4 (lang 0)
    CItemNameBuf t2[3];   // 0x408 (lang 2)
    CItemNameBuf t16[4];  // 0x2f8 (lang 1/6)
    CItemNameBuf t3[4];   // 0x1e8 (lang 3)
    CItemNameBuf t5[3];   // 0x11c (lang 5)
    CItemNameBuf t4[4];   // 0x0c (lang 4)
    union { u32 v; u8 b[4]; } tmp;  // 0x08
    buf->field_00 = 0;
    buf->field_40 = 0;
    s1.mStr[0] = 0;
    s1.mLen = 0;
    s2.mStr[0] = 0;
    s2.mLen = 0;
    s3.mStr[0] = 0;
    s3.mLen = 0;
    prefix.mLen = (s32)strlen(lbl_eu_806622A4);
    strcpy(prefix.mStr, lbl_eu_806622A4);
    if (b - 1 > 5) b = 1;
    if (d < 4 || b >= 11) d = 4;
    void* h170 = lbl_eu_80664170;
    const char* base = lbl_eu_80501C58;
    const char* s = (const char*)getBdatStringColumnValue(h170, base + 0xd3, b + 0x94);
    s1.mLen = (s32)strlen(s);
    strcpy(s1.mStr, s);
    if (a == 2) {
        if (c != 0) {
            s = (const char*)getBdatStringColumnValue(lbl_eu_806640D8, base + 0xe1, f);
        } else {
            tmp.v = getBdatStringColumnValue(lbl_eu_80664094, base + 0xef, c);
            s = (const char*)getBdatStringColumnValue(lbl_eu_8066416C, base + 0xd3, tmp.b[0]);
        }
        s2.mLen = (s32)strlen(s);
        strcpy(s2.mStr, s);
    } else if (a == 3) {
        s = (const char*)getBdatStringColumnValue(h170, base + 0xd3, d + 0x8b);
        s2.mLen = (s32)strlen(s);
        strcpy(s2.mStr, s);
    } else {
        s = (const char*)getBdatStringColumnValue(h170, base + 0xd3, d + 0x8b);
        s2.mLen = (s32)strlen(s);
        strcpy(s2.mStr, s);
    }
    s = (const char*)getBdatStringColumnValue(h170, base + 0xd3, e + 0x9a);
    s3.mLen = (s32)strlen(s);
    strcpy(s3.mStr, s);
    u8 lang = (u8)getLanguage__9CDeviceSCFv();
    switch (lang) {
    case 2:  // s1 + prefix + s2 + s3
        t2[0].mLen = strlen(s1.mStr);
        strcpy(t2[0].mStr, s1.mStr);
        strcat(t2[0].mStr, prefix.mStr);
        t2[0].mLen += strlen(prefix.mStr);
        t2[1].mLen = strlen(t2[0].mStr);
        strcpy(t2[1].mStr, t2[0].mStr);
        strcat(t2[1].mStr, s2.mStr);
        t2[1].mLen += strlen(s2.mStr);
        t2[2].mLen = strlen(t2[1].mStr);
        strcpy(t2[2].mStr, t2[1].mStr);
        strcat(t2[2].mStr, s3.mStr);
        t2[2].mLen += strlen(s3.mStr);
        buf->field_40 = (u32)strlen(t2[2].mStr);
        strcpy((char*)buf, t2[2].mStr);
        break;
    case 1:
    case 6:  // s1 + prefix + s2 + prefix + s3
        t16[0].mLen = strlen(s1.mStr);
        strcpy(t16[0].mStr, s1.mStr);
        strcat(t16[0].mStr, prefix.mStr);
        t16[0].mLen += strlen(prefix.mStr);
        t16[1].mLen = strlen(t16[0].mStr);
        strcpy(t16[1].mStr, t16[0].mStr);
        strcat(t16[1].mStr, s2.mStr);
        t16[1].mLen += strlen(s2.mStr);
        t16[2].mLen = strlen(t16[1].mStr);
        strcpy(t16[2].mStr, t16[1].mStr);
        strcat(t16[2].mStr, prefix.mStr);
        t16[2].mLen += strlen(prefix.mStr);
        t16[3].mLen = strlen(t16[2].mStr);
        strcpy(t16[3].mStr, t16[2].mStr);
        strcat(t16[3].mStr, s3.mStr);
        t16[3].mLen += strlen(s3.mStr);
        buf->field_40 = (u32)strlen(t16[3].mStr);
        strcpy((char*)buf, t16[3].mStr);
        break;
    case 3:  // s3 + prefix + s2 + prefix + s1
        t3[0].mLen = strlen(s3.mStr);
        strcpy(t3[0].mStr, s3.mStr);
        strcat(t3[0].mStr, prefix.mStr);
        t3[0].mLen += strlen(prefix.mStr);
        t3[1].mLen = strlen(t3[0].mStr);
        strcpy(t3[1].mStr, t3[0].mStr);
        strcat(t3[1].mStr, s2.mStr);
        t3[1].mLen += strlen(s2.mStr);
        t3[2].mLen = strlen(t3[1].mStr);
        strcpy(t3[2].mStr, t3[1].mStr);
        strcat(t3[2].mStr, prefix.mStr);
        t3[2].mLen += strlen(prefix.mStr);
        t3[3].mLen = strlen(t3[2].mStr);
        strcpy(t3[3].mStr, t3[2].mStr);
        strcat(t3[3].mStr, s1.mStr);
        t3[3].mLen += strlen(s1.mStr);
        buf->field_40 = (u32)strlen(t3[3].mStr);
        strcpy((char*)buf, t3[3].mStr);
        break;
    case 5:  // s1 + s3 + prefix + s2
        t5[0].mLen = strlen(s1.mStr);
        strcpy(t5[0].mStr, s1.mStr);
        strcat(t5[0].mStr, s3.mStr);
        t5[0].mLen += strlen(s3.mStr);
        t5[1].mLen = strlen(t5[0].mStr);
        strcpy(t5[1].mStr, t5[0].mStr);
        strcat(t5[1].mStr, prefix.mStr);
        t5[1].mLen += strlen(prefix.mStr);
        t5[2].mLen = strlen(t5[1].mStr);
        strcpy(t5[2].mStr, t5[1].mStr);
        strcat(t5[2].mStr, s2.mStr);
        t5[2].mLen += strlen(s2.mStr);
        buf->field_40 = (u32)strlen(t5[2].mStr);
        strcpy((char*)buf, t5[2].mStr);
        break;
    case 4:  // s3 + prefix + s2 + prefix + s1
        t4[0].mLen = strlen(s3.mStr);
        strcpy(t4[0].mStr, s3.mStr);
        strcat(t4[0].mStr, prefix.mStr);
        t4[0].mLen += strlen(prefix.mStr);
        t4[1].mLen = strlen(t4[0].mStr);
        strcpy(t4[1].mStr, t4[0].mStr);
        strcat(t4[1].mStr, s2.mStr);
        t4[1].mLen += strlen(s2.mStr);
        t4[2].mLen = strlen(t4[1].mStr);
        strcpy(t4[2].mStr, t4[1].mStr);
        strcat(t4[2].mStr, prefix.mStr);
        t4[2].mLen += strlen(prefix.mStr);
        t4[3].mLen = strlen(t4[2].mStr);
        strcpy(t4[3].mStr, t4[2].mStr);
        strcat(t4[3].mStr, s1.mStr);
        t4[3].mLen += strlen(s1.mStr);
        buf->field_40 = (u32)strlen(t4[3].mStr);
        strcpy((char*)buf, t4[3].mStr);
        break;
    default:  // lang 0: s1 + s2 + s3
        t0[0].mLen = strlen(s1.mStr);
        strcpy(t0[0].mStr, s1.mStr);
        strcat(t0[0].mStr, s2.mStr);
        t0[0].mLen += strlen(s2.mStr);
        t0[1].mLen = strlen(t0[0].mStr);
        strcpy(t0[1].mStr, t0[0].mStr);
        strcat(t0[1].mStr, s3.mStr);
        t0[1].mLen += strlen(s3.mStr);
        buf->field_40 = (u32)strlen(t0[1].mStr);
        strcpy((char*)buf, t0[1].mStr);
        break;
    }
}

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

// Roll one item from the two-column roll list: mutate the two column-name
// strings with the iteration digit, roll against the percent column, and on
// success build a kind-9 scratch record, refresh it via the impl vtable,
// append it to self's record array and copy it out. Returns 1 on success.
s32 func_8015A054(CItemFour* self, u32 a, u32 unused, void* c, u32 d, u32 e) {
    CItemScratch local;
    local.field_04 = 0;
    local.field_00 = 0;
    if (lbl_eu_80664204 == 0) {
        lbl_eu_80664200 = strlen(lbl_eu_806622A8) - 1;
        lbl_eu_80664204 = 1;
    }
    memset(&local, 0, 0x34);
    s32 idx = lbl_eu_80664200;
    for (s32 i = 0; i < 2; i++) {
        char* colA = lbl_eu_806622A8;
        char* colB = lbl_eu_806622AC;
        int ch = 0x31 + i;
        colA[idx] = (char)ch;
        colB[idx] = (char)ch;
        union {
            u32 v;
            u16 h[2];
        } r1v;
        union {
            u32 v;
            u8 b[4];
        } r2v;
        r1v.v = getBdatStringColumnValue(c, colA, a);
        r2v.v = getBdatStringColumnValue(c, colB, a);
        u8 pct = r2v.b[0];
        int rnd = ml::math::mtRand(100);
        if (r1v.h[0] != 0 && rnd < pct) {
            s32 size = func_80156F54(9);
            if (size != 0) {
                memset(&local, 0, (u32)size);
            }
            CItemData* ld = (CItemData*)&local;
            ld->field_06 = 1;
            u8 b7 = ld->field_07;
            b7 = (u8)__rlwimi(b7, 2, 0, 30, 31);
            ld->field_07 = b7;
            u32 w = ld->field_00;
            w = __rlwimi(w, r1v.h[0], 20, 0, 11);
            w = __rlwimi(w, 9, 16, 12, 15);
            w = __rlwimi(w, r1v.h[0], 5, 16, 26);
            ld->field_00 = w;
            CItem_initItemImplInstances((CItemData*)&local)->vf7C((CItemData*)&local, (u16)e);
            CItem_initItemImplInstances((CItemData*)&local)->vf1C((CItemData*)&local);
            CItem_initItemImplInstances((CItemData*)&local)->vf8C((CItemData*)&local, a);
            u32 w2 = ld->field_00;
            w2 = __rlwimi(w2, d, 0, 30, 31);
            ld->field_00 = w2;
            u32 n = self->mCount;
            self->mCount = n + 1;
            CItemExt* dst = &self->mItems[n];
            u32 size2 = (u32)func_80156F54((u16)((ld->field_00 >> 16) & 0xF));
            memcpy(dst, &local, size2);
            return 1;
        }
    }
    return 0;
}

void func_8015A230() {}

void func_8015A234() {}

// Roll a random item in the kind list (three-column variant): mutate the
// three column-name strings with the iteration digit, read the BDAT name/
// percent columns, and on a successful roll initialise the 52-byte scratch
// record, refresh the kind-impl state (vf1C/vf34), append it to self's
// record array and copy the record out. Returns 1 on success.
extern "C" s32 func_8015A238(CItemFour* self, u32 a, u32 unused, void* c, u32 d) {
    CItemScratch local;
    local.field_04 = 0;
    local.field_00 = 0;
    if (lbl_eu_8066420C == 0) {
        lbl_eu_80664208 = strlen(lbl_eu_806622B0) - 1;
        lbl_eu_8066420C = 1;
    }
    memset(&local, 0, 0x34);
    s32 idx = lbl_eu_80664208;
    for (s32 i = 0; i < 4; i++) {
        char* col1 = lbl_eu_806622B0;
        char* col2 = lbl_eu_806622B4;
        char* col3 = lbl_eu_806622B8;
        int ch = 0x31 + i;
        col1[idx] = (char)ch;
        col2[idx] = (char)ch;
        col3[idx] = (char)ch;
        union {
            u32 v;
            u16 h[2];
        } r1v;
        union {
            u32 v;
            u8 b[4];
        } r2v;
        union {
            u32 v;
            u8 b[4];
        } r3v;
        r1v.v = getBdatStringColumnValue(c, col1, a);
        r2v.v = getBdatStringColumnValue(c, col2, a);
        int v2b = r2v.b[0] * 100;
        r3v.v = getBdatStringColumnValue(c, col3, a);
        int r = ml::math::mtRand(10000);
        int v3b = r3v.b[0];
        if (r1v.h[0] != 0 && (u32)r < (u32)v2b) {
            func_80158118((CItemData*)&local, r1v.h[0], 1);
            CItem_initItemImplInstances((CItemData*)&local)->vf1C((CItemData*)&local);
            CItem_initItemImplInstances((CItemData*)&local)->vf34((CItemData*)&local, (u32)v3b);
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

// Roll a random item in the kind list (three-column variant): mutate the
// three column-name strings with the iteration digit, read the BDAT name/
// percent columns, and on a successful roll initialise the 52-byte scratch
// record, refresh the kind-impl state (vf1C/vf34), append it to self's
// record array and copy the record out. Returns 1 on success.
s32 func_8015A51C(CItemFour* self, u32 a, u32 unused, void* c, u32 d) {
    CItemScratch local;
    local.field_04 = 0;
    local.field_00 = 0;
    if (lbl_eu_8066421C == 0) {
        lbl_eu_80664218 = strlen(lbl_eu_806622C4) - 1;
        lbl_eu_8066421C = 1;
    }
    memset(&local, 0, 0x34);
    s32 idx = lbl_eu_80664218;
    for (s32 i = 0; i < 4; i++) {
        char* col1 = lbl_eu_806622C4;
        char* col2 = lbl_eu_806622C8;
        char* col3 = lbl_eu_806622CC;
        int ch = 0x31 + i;
        col1[idx] = (char)ch;
        col2[idx] = (char)ch;
        col3[idx] = (char)ch;
        union {
            u32 v;
            u16 h[2];
        } r1v;
        union {
            u32 v;
            u8 b[4];
        } r2v;
        union {
            u32 v;
            u8 b[4];
        } r3v;
        r1v.v = getBdatStringColumnValue(c, col1, a);
        r2v.v = getBdatStringColumnValue(c, col2, a);
        int v2b = r2v.b[0];
        r3v.v = getBdatStringColumnValue(c, col3, a);
        int r = ml::math::mtRand(100);
        int v3b = r3v.b[0];
        if (r1v.h[0] != 0 && r < v2b) {
            func_80158118((CItemData*)&local, r1v.h[0], 1);
            CItem_initItemImplInstances((CItemData*)&local)->vf1C((CItemData*)&local);
            local.field_00 = __rlwimi(local.field_00, d, 0, 30, 31);
            CItem_initItemImplInstances((CItemData*)&local)->vf34((CItemData*)&local, (u32)v3b);
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

// Roll up to maxCount items for the shared four-record block (body not yet
// recovered; noinline keeps the retail `bl` at the func_8015ACAC call site).
extern "C" __declspec(noinline) s32 func_8015A7FC(CItemFour* self, u32 item, u32 count, void* handle, u32 x) { return 0; }

// Roll up to maxCount items for the shared four-record block: mutate the two
// column-name strings' last character with the iteration digit, read the
// BDAT name/percent columns, and on a passing roll build a 52-byte item
// record (func_80158118 + vf1C) and append it to self's record array.
// forceA/forceB and the second-iteration guarantee force a 100% roll.
extern "C" void func_8015A930(CItemFour* self, u32 row, s32 maxCount,
                              u32 forceA, u32 randomize, u32 forceB) {
    void* handle = lbl_eu_8066414C;
    s32 len = strlen(lbl_eu_806622E0) - 1;
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
        u8 pct = r2v.b[0];
        int rnd = ml::math::mtRand(100);
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

// Roll up to `count` items into the shared four-record block using the four
// per-item probability columns: read the four u8 percent weights (columns
// +0xF9/+0x103/+0x10B/+0x117), then repeatedly pick a column by weighted
// roll (retrying up to 100 times), roll the matching kind's record
// (func_8015A51C / func_8015A238 / func_8015A054 / func_8015A7FC), and
// dedupe the last two records when they share the same family id.
extern "C" void __declspec(noinline) func_8015AAB4(CItemFour* self, u32 item, u32 count, u32 row) {
    const char* base = lbl_eu_80501C58;
    void* handle = lbl_eu_80664150;
    u16 probs[4];
    union {
        u32 v;
        u8 b[4];
    } t1;
    union {
        u32 v;
        u8 b[4];
    } t2;
    union {
        u32 v;
        u8 b[4];
    } t3;
    union {
        u32 v;
        u8 b[4];
    } t4;
    t1.v = getBdatStringColumnValue(handle, base + 0xf9, item);
    probs[0] = (u16)t1.b[0];
    t2.v = getBdatStringColumnValue(handle, base + 0x103, item);
    probs[1] = (u16)t2.b[0];
    t3.v = getBdatStringColumnValue(handle, base + 0x10b, item);
    probs[2] = (u16)t3.b[0];
    t4.v = getBdatStringColumnValue(handle, base + 0x117, item);
    probs[3] = (u16)t4.b[0];
    s32 added = 0;
    while (added < (s32)count) {
        s32 pick = 0;
        s32 tries = 0;
        do {
            pick = 0;
            while (pick < 4) {
                if (ml::math::mtRand(100) < probs[pick]) goto roll;
                pick++;
            }
            tries++;
        } while (tries < 100);
        pick = 0;
    roll:
        if (pick == 0) {
            added += func_8015A51C(self, item, count, handle, 1);
        } else if (pick == 1) {
            added += func_8015A238(self, item, count, handle, 1);
        } else if (pick == 2) {
            added += func_8015A054(self, item, count, handle, 1, row);
        } else if (pick == 3) {
            added += func_8015A7FC(self, item, count, handle, 1);
        }
        if ((s32)count > 1 && added > 1) {
            s32 last = (s32)self->mCount - 1;
            if (last >= 1 &&
                (self->mItems[last].field_00 >> 20) ==
                    (self->mItems[last - 1].field_00 >> 20)) {
                added--;
                self->mCount = self->mCount - 1;
            }
        }
    }
}

// Roll a third item into the shared block (body not yet recovered).
// Roll up to `count` items for the shared four-record block from the four
// per-item probability columns: pick a column by weighted roll (retrying up
// to 100 times), roll the matching kind's record, and dedupe the last two
// records when they share the same family id.
extern "C" void func_8015ACAC(CItemFour* self, u32 item, u32 count) {
    void* handle = lbl_eu_80664154;
    u16 probs[4];
    union {
        u32 v;
        u8 b[4];
    } t1;
    union {
        u32 v;
        u8 b[4];
    } t2;
    union {
        u32 v;
        u8 b[4];
    } t3;
    union {
        u32 v;
        u8 b[4];
    } t4;
    t1.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x117, item);
    probs[0] = (u16)t1.b[0];
    t2.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x103, item);
    probs[1] = (u16)t2.b[0];
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x120, item);
    probs[2] = (u16)t3.b[0];
    t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x12c, item);
    probs[3] = (u16)t4.b[0];
    s32 added = 0;
    while (added < (s32)count) {
        s32 pick = 0;
        s32 tries = 0;
        do {
            pick = 0;
            while (pick < 4) {
                if (ml::math::mtRand(100) < probs[pick]) goto roll;
                pick++;
            }
            tries++;
        } while (tries < 100);
        pick = 0;
    roll:
        if (pick == 0) {
            added += func_8015A7FC(self, item, count, handle, 2);
        } else if (pick == 1) {
            added += func_8015A238(self, item, count, handle, 2);
        } else if (pick == 2) {
            added += func_8015A3CC(self, item, count, handle, 2);
        } else if (pick == 3) {
            added += func_8015A6AC(self, item, count, handle, 2);
        }
        if ((s32)count > 1 && added > 1) {
            s32 last = (s32)self->mCount - 1;
            if (last >= 1 &&
                (self->mItems[last].field_00 >> 20) ==
                    (self->mItems[last - 1].field_00 >> 20)) {
                added--;
                self->mCount = self->mCount - 1;
            }
        }
    }
}

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
extern "C" void func_8015AFA4(s32 kind, u32 row) {
    u32 item1;
    u32 item2;
    u32 item3;
    u32 v8;
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
    item1 = t1.b[0];
    t2.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x9, row);
    item2 = t2.b[0];
    t3.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0x12, row);
    item3 = t3.b[0];
    t4.v = getBdatStringColumnValue(handle, lbl_eu_80501C58 + 0xb7, row);
    v8 = t4.b[0];
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
                    CItemExt* dst = (CItemExt*)((u8*)block + n * 0x34);
                    block->mCount = n + 1;
                    u32 size = (u32)func_80156F54((u16)((scratch.field_00 >> 16) & 0xF));
                    memcpy(dst, &scratch, size);
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
