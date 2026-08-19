#include "kyoshin/cf/object/CBattleState.hpp"

// C-ABI runtime helpers: extern "C" so MWCC emits the plain `memset`/`memcpy`
// reloc names (retail `bl memset`; C++ mangling would give memset__FPviUl).
extern "C" void* memset(void* dest, int val, size_t count);
extern "C" void* memcpy(void* dest, const void* src, size_t count);

// sdata2 float constants used by CBattleState_UnkVirtualFunc5
extern const float lbl_eu_80667400;  // 0x80146E48: id==0x35 unk24
extern const float lbl_eu_80667404;  // 0x80146E90: unk20 *= float
extern const double lbl_eu_80667408; // 0x80146EC4: unk20 *= double
extern const float lbl_eu_80667410;  // 0x80146ED4: compared with unk20
extern const float lbl_eu_80667414;  // 0x80148134: 0.9f scaling (same as vfunc6)
extern u8 lbl_eu_80662248[8];      // .sdata object; CBattleState_UnkVirtualFunc3 returns its address

void func_80109784(void* ptr, u32 id, int arg);
void func_8013DB6C(int a, u32 id, int b, int c);
int func_80148778(cf::CBattleState* self, u32 id);

namespace cf {

CBattleState::CBattleState() {
    u8* p;
    u8* end;

    unk4 = 0;
    unk6 = 0;
    memset(unk8, 0, 0x1520);

    p = unk152C;
    end = (u8*)this + 0x15AC;
    do {
        memset(p, 0, 0x10);
        p += 0x10;
    } while (p < end);

    memset(unk152C, 0, 0x80);
    memset(this, 0, 0x15D4);
}

// Batch 2026-07-14j: battlestate-vfunc29 owns CBattleState_UnkVirtualFunc29
// exclusively. Do not touch the ctor / other vfuncs.
//
// True Fv (r3=this only). Clears the 8 CBattleStateEntry slots at +0x1388;
// for each prior id, if it is still present among the 0x68 entries at +0x8
// (13x8 halfword scan) the this+0x15AC status bit stays; otherwise cleared.
// Ids >= 0x12f always clear (skip the scan). Ends with memset(+0x152C,0,0x80).
//
// findBattleEntry is a static search helper inlined via -ipa file: the
// return-based form reproduces the retail's exact mtctr/bdnz loop with the
// dead +(checks-1)=+7 shadow counter (MWCC_CASES §inlined search
// helpers), indexing entries[j] straight from `this` so the +0x8 array base
// folds into the load displacements (0x14, 0x48, ...) and the scan base
// stays `this` (retail mr r4, r26).
static int findBattleEntry(cf::CBattleState* self, u32 id) {
    cf::CBattleStateEntryArray* v;
    int j;

    v = (cf::CBattleStateEntryArray*)self;
    for (j = 0; j < 0x68; j++) {
        if (id == v->entries[j].unk0C) {
            return 1;
        }
    }
    return 0;
}

void CBattleState::CBattleState_UnkVirtualFunc29() {
    CBattleStateEntry* entry;
    int i;

    entry = (CBattleStateEntry*)((u8*)this + 0x1388);
    for (i = 0; i < 8; i++, entry++) {
        u32 id;
        int found;

        id = entry->unk0C;
        memset(entry, 0, 0x34);

        if (id >= 0x12f) {
            found = 0;
        } else {
            found = findBattleEntry(this, id);
        }

        if (!found) {
            u8* wordPtr = unk15AC + ((id >> 3) & ~3u);
            *(u32*)wordPtr &= ~(1u << (id & 0x1F));
        }
    }

    memset(unk152C, 0, 0x80);
}

} // namespace cf

// Batch 2026-07-14g: battlestate-vfunc6 owns CBattleState_UnkVirtualFunc6
// exclusively. Do not touch CBattleState::CBattleState() above.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// pattern as cf::CAIAction's UnkVirtualFunc1/2 -- see
// docs/MWCC_CASES.md).
//
// sdata2 float pool constant read via lbl_eu_80667414@sda21 (0.9f).
extern const float lbl_eu_80667414;

// Cast-only SI iface for vt+0x48 tail-call (same RTTI omit as BattleStateV8If).
struct BattleStateV6If {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void vf48(cf::CBattleStateEntry* entry); // UnkVirtualFunc17 @0x48
};

void cf::CBattleState::CBattleState_UnkVirtualFunc6(cf::CBattleStateEntry* arg) {
    cf::CBattleStateEntry* entries;
    cf::CBattleStateEntry* p;
    int n;
    u32 one;
    u32 bit;
    u32 id;

    // Bit `arg->unk0C` set into the this+0x15AC bitfield (word-aligned byte
    // offset = (id >> 3) & ~3, bit position = id & 0x1F).
    id = arg->unk0C;
    one = 1;
    entries = (cf::CBattleStateEntry*)((u8*)this + 0x1388);
    p = entries;
    *(u32*)(unk15AC + ((id >> 3) & ~3u)) |= one << (id & 0x1F);
    for (n = 8; n != 0; n--, p++) {
        if (p->unk0C == arg->unk0C) {
            if (p->unk10 < arg->unk10) {
                p->unk10 = arg->unk10;
            }
            if (arg->unk18 != 0) {
                if (p->unk10 > arg->unk18) {
                    p->unk10 = arg->unk18;
                }
            }
            reinterpret_cast<BattleStateV6If*>(this)->vf48(p);
            return;
        }
    }

    for (n = 8; n != 0; n--, entries++) {
        if (entries->unk0C == 0) {
            entries->unk00 = arg->unk00;
            entries->unk04 = arg->unk04;
            entries->unk08 = arg->unk08;
            entries->unk0C = arg->unk0C;
            entries->unk10 = arg->unk10;
            entries->unk14 = arg->unk14;
            entries->unk16 = arg->unk16;
            entries->unk18 = arg->unk18;
            entries->unk1A = arg->unk1A;
            entries->unk1C = arg->unk1C;
            entries->unk20 = arg->unk20;
            entries->unk24 = arg->unk24;
            f32 scaled = lbl_eu_80667414 * entries->unk24;
            entries->unk28 = arg->unk28;
            entries->unk2C = arg->unk2C;
            entries->unk2E = arg->unk2E;
            entries->unk30 = arg->unk30;
            entries->unk1C = entries->unk20;
            entries->unk28 = scaled;
            reinterpret_cast<BattleStateV6If*>(this)->vf48(entries);
            return;
        }
    }
}

// symbols.txt mangles Fv, but retail leaves the id in r4. This lookup reads
// the independent halfword state at +0x6.
int cf::CBattleState::CBattleState_UnkVirtualFunc33(u32 id) {
    u16 mask;

    if (id >= 0x12f) {
        return 0;
    }

    mask = 0;

    switch (id) {
    case 0x4:
        mask = 0x1;
        break;
    case 0x7:
        mask = 0x2;
        break;
    case 0x6:
        mask = 0x4;
        break;
    case 0x11:
        mask = 0x8;
        break;
    case 0x9:
        mask = 0x10;
        break;
    case 0x13:
        mask = 0x20;
        break;
    case 0xb:
        mask = 0x40;
        break;
    case 0xc:
        mask = 0x80;
        break;
    case 0xdc:
        mask = 0x100;
        break;
    case 0x12:
        mask = 0x200;
        break;
    case 0x3c:
        mask = 0x400;
        break;
    case 0x3d:
        mask = 0x800;
        break;
    case 0x52:
        mask = 0x1000;
        break;
    case 0x53:
        mask = 0x2000;
        break;
    case 0x54:
        mask = 0x4000;
        break;
    case 0x117:
        mask = 0x8000;
        break;
    }

    return (unk6 & mask) != 0;
}

// Cast-only SI iface for the vt+0x4C tail-call (UnkVirtualFunc18; same
// RTTI-omit pattern as BattleStateV6If). Virtual dispatch emits retail
// lwz r12,0(this) / lwz r12,0x4c(r12) / bctr instead of coloring the
// vptr as r5 like a function-pointer vslot load.
struct BattleStateV11If {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void _v02C();
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void _v048();
    virtual void vf4C(cf::CBattleStateEntry* entry); // UnkVirtualFunc18 @0x4C
};

// Batch 2026-07-14h: battlestate-vfunc11 owns CBattleState_UnkVirtualFunc11
// exclusively. Do not touch the ctor / vfunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the caller's mask in r4 (same
// ABI pattern as CBattleState_UnkVirtualFunc6). Walks the this+0x8 entry
// array (stride 0x34, count 0x68 == sizeof(unk8)/0x34) and, for any slot
// whose unk30 flags intersect the mask: fires the vt+0x4C callback
// (UnkVirtualFunc18) with that slot, remembers its id, then clears the
// slot. If another live entry still shares that id (scan of the same
// array, 13 groups of 8, matching MWCC's fixed-trip-count unroll), the
// this+0x15AC status bit for that id is left alone; otherwise it's
// cleared (ids >= 0x12f always clear, skipping the scan).
void cf::CBattleState::CBattleState_UnkVirtualFunc11(u32 mask) {
    u32 one;
    int thirteen;
    int i;
    cf::CBattleStateEntry* entry;

    one = 1;
    thirteen = 0xd;
    entry = (cf::CBattleStateEntry*)((u8*)this + 0x8);
    i = 0;
    do {
        u32 id;
        int stillActive;

        if ((entry->unk30 & mask) != 0) {
            reinterpret_cast<BattleStateV11If*>(this)->vf4C(entry);
            id = entry->unk0C;
            memset(entry, 0, 0x34);

            if (id >= 0x12f) {
                stillActive = 0;
            } else {
                // Retail: scan base recomputed from `this` into volatile r4
                // (mr r4, r29) with entry offsets folded (0x14/0x48/...) and a
                // dead +7 trip counter -- MWCC's auto-unroll of the linear 0x68
                // scan into a 13x8 mtctr/bdnz loop (same shape as the matched
                // func_801490A0 / vfunc29). Indexing v->entries[g] keeps the
                // base `this` so no separate this+8 value needs a callee-saved
                // reg; the post-loop stillActive=0 + goto mirrors the retail
                // `li r0,0` fall-through so the merge coalesces to one register.
                cf::CBattleStateEntryArray* v = (cf::CBattleStateEntryArray*)this;
                int g;

                stillActive = 0;
                for (g = 0; g < thirteen * 8; g++) {
                    if (id == v->entries[g].unk0C) {
                        stillActive = 1;
                        goto scan_done;
                    }
                }
                stillActive = 0;
            scan_done:
                ;
            }

            if (!stillActive) {
                u8* wordPtr = this->unk15AC + ((id >> 3) & ~3u);
                *(u32*)wordPtr &= ~(one << (id & 0x1F));
            }
        }

        i++;
        entry++;
    } while (i < 0x68);
}

// Batch 2026-07-14h: battlestate-vfunc31 owns CBattleState_UnkVirtualFunc31
// exclusively. Do not touch ctor / UnkVirtualFunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the id in r4 (same fake-Fv ABI
// as UnkVirtualFunc6 -- see docs/MWCC_CASES.md).
//
// Leaf / no stack frame: maps specific ids to single-bit masks (or 0 for
// unmapped ids), then returns (this->unk4 & mask) != 0 via the standard
// MWCC branchless neg/or/srwi boolify idiom (see MWCC_CASES section 8c9).
int cf::CBattleState::CBattleState_UnkVirtualFunc31(u32 id) {
    u16 mask;

    if (id >= 0x12f) {
        return 0;
    }

    mask = 0;

    switch (id) {
    case 0x4:
        mask = 0x1;
        break;
    case 0x7:
        mask = 0x2;
        break;
    case 0x6:
        mask = 0x4;
        break;
    case 0x11:
        mask = 0x8;
        break;
    case 0x9:
        mask = 0x10;
        break;
    case 0x13:
        mask = 0x20;
        break;
    case 0xb:
        mask = 0x40;
        break;
    case 0xc:
        mask = 0x80;
        break;
    case 0xdc:
        mask = 0x100;
        break;
    case 0x12:
        mask = 0x200;
        break;
    case 0x3c:
        mask = 0x400;
        break;
    case 0x3d:
        mask = 0x800;
        break;
    case 0x52:
        mask = 0x1000;
        break;
    case 0x53:
        mask = 0x2000;
        break;
    case 0x54:
        mask = 0x4000;
        break;
    case 0x117:
        mask = 0x8000;
        break;
    }

    return (unk4 & mask) != 0;
}

// Batch 2026-07-14h: battlestate-vfunc26 owns CBattleState_UnkVirtualFunc26
// exclusively. Do not touch ctor / UnkVirtualFunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the source table pointer in r4
// (same ABI pattern as UnkVirtualFunc6). 0x10-byte record read from the
// incoming table; 8 consecutive records precede the raw copy landed at
// CBattleState::unk152C.
namespace cf {
struct CBattleStateSrcEntry {
    u16 unk00; // 0x00
    u16 unk02; // 0x02
    u16 unk04; // 0x04 - id, copied into CBattleStateEntry::unk0C
    u16 unk06; // 0x06 - copied into CBattleStateEntry::unk1A
    s16 unk08; // 0x08 - sign-extended into CBattleStateEntry::unk10
    s16 unk0A; // 0x0A - copied into CBattleStateEntry::unk14
    s16 unk0C; // 0x0C - copied into CBattleStateEntry::unk18
    u16 unk0E; // 0x0E - bit 0x8000 selects CBattleStateEntry::unk08
};
} // namespace cf

// Cast-only SI: manual vslot loads color vptr as r5; virtual dispatch emits
// retail lwz r12,0(this) / lwz r12,0x1c(r12) / bctr. Omit RTTI _v000/_v004 so
// vf1C lands at 0x1C (same pattern as BattleStateV8If).
struct BattleStateV26If {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void vf1C(cf::CBattleStateEntry* entry); // UnkVirtualFunc6 @0x1C
};

void cf::CBattleState::CBattleState_UnkVirtualFunc26(const cf::CBattleStateSrcEntry* src) {
    const cf::CBattleStateSrcEntry* rec;
    const cf::CBattleStateSrcEntry* recFlags;
    int n;
    int allZero;
    u32 flag4000;
    u32 flag2000;

    this->CBattleState_UnkVirtualFunc29();

    // Retail: li r26,0x4000 then li r25,0x2000.
    flag4000 = 0x4000;
    flag2000 = 0x2000;

    for (n = 0, rec = src, recFlags = src; n < 8; n++, rec++, recFlags++) {
        cf::CBattleStateEntry entry;

        allZero = 0;
        if (rec->unk00 == 0) {
            if (rec->unk02 == 0) {
                if (rec->unk04 == 0) {
                    allZero = 1;
                }
            }
        }
        if (allZero) {
            continue;
        }

        memset(&entry, 0, sizeof(entry));
        // Retail schedule: id/flags/2000/fields, then extrwi on unk0E.
        // `unk30 |= 1` beats an explicit flags temp here (~78.8%).
        entry.unk0C = rec->unk04;
        entry.unk30 |= 1;
        entry.unk08 = flag2000;
        entry.unk10 = rec->unk08;
        entry.unk14 = rec->unk0A;
        entry.unk18 = rec->unk0C;
        entry.unk1A = (s16)rec->unk06;
        if ((((u32)recFlags->unk0E >> 15) & 1) != 0) {
            entry.unk08 = flag4000;
        }

        reinterpret_cast<BattleStateV26If*>(this)->vf1C(&entry);
    }

    // Retail: mtctr/bdnz + lwzu/stwu from this+0x1528 / src-4. do-while(--i)
    // stays compact (for!=0 fully unrolls to 0x208); addic./bne soft-cap.
    {
        u32* dst = (u32*)((u8*)this + 0x1528);
        u32* s = (u32*)src - 1;
        u32 i = 0x10;

        do {
            u32 a = *(s + 1);
            u32 b = *(s += 2);
            *(dst + 1) = a;
            *(dst += 2) = b;
        } while (--i);
    }
}

// Batch 2026-07-14j: battlestate-vfunc8 owns CBattleState_UnkVirtualFunc8
// exclusively. Do not touch ctor / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc6). Nested cmpwi on entry->unk0C picks a
// kind; kind==3 clears this+0x1528. Calls vt+0x2C (UnkVirtualFunc10) with
// the arg, then walks this+0x8 entries (stride 0x34, count 0x68): matching
// id (+ optional field eq unless unk30 bit 0x200) -> stack-copy/clear ->
// vt+0x2C -> id-dup scan / clear unk15AC bit -> vt+0x4C; stop early if
// arg->unk0C == 0.
//
// Cast-only SI iface: function-pointer vslot loads color the vptr as r5;
// virtual dispatch emits retail lwz r12,0(this) / lwz r12,off(r12) / bctr.
// RTTI on: omit _v000/_v004 so _v008 lands at retail 0x8 (MenuBpsActorIf).
struct BattleStateV8If {
    virtual void _v008();
    virtual void _v00C();
    virtual void _v010();
    virtual void _v014();
    virtual void _v018();
    virtual void _v01C();
    virtual void _v020();
    virtual void _v024();
    virtual void _v028();
    virtual void vf2C(cf::CBattleStateEntry* entry); // UnkVirtualFunc10 @0x2C
    virtual void _v030();
    virtual void _v034();
    virtual void _v038();
    virtual void _v03C();
    virtual void _v040();
    virtual void _v044();
    virtual void vf48(cf::CBattleStateEntry* entry); // UnkVirtualFunc17 @0x48
    virtual void vf4C(cf::CBattleStateEntry* entry); // UnkVirtualFunc18 @0x4C
};

void cf::CBattleState::CBattleState_UnkVirtualFunc8(cf::CBattleStateEntry* entry) {
    // Function-scope slot/i reserve r31/r30 so Chaitin parks this/entry in
    // r28/r29 and one/thirteen in r26/r27 (block-local slot/i stole r27/r28).
    cf::CBattleStateEntry* slot;
    int i;
    int one;
    int thirteen;
    int id;
    int kind;

    id = entry->unk0C;

    // Flat if+goto mirrors retail's cmpwi/beq/bge chain 1:1. Nested if/else
    // preserves pivots but duplicates kind leaves (oversized); nested if+goto
    // to shared leaves lets MWCC rebuild a different decision tree.
    if (id >= 0xd4)
        goto L_801486EC;
    if (id >= 0x3e)
        goto L_80148684;
    if (id >= 0x2c)
        goto L_80148658;
    if (id == 0x27)
        goto kind0;
    if (id >= 0x27)
        goto L_8014864C;
    if (id >= 4)
        goto L_80148640;
    if (id >= 2)
        goto kind0;
    goto kind2;

L_80148640:
    if (id >= 0x14)
        goto kind2;
    goto kind1;

L_8014864C:
    if (id >= 0x2a)
        goto kind1;
    goto kind2;

L_80148658:
    if (id == 0x36)
        goto kind0;
    if (id >= 0x36)
        goto L_80148678;
    if (id >= 0x35)
        goto kind2;
    if (id >= 0x33)
        goto kind0;
    goto kind2;

L_80148678:
    if (id >= 0x3c)
        goto kind1;
    goto kind2;

L_80148684:
    if (id == 0x5f)
        goto kind0;
    if (id >= 0x5f)
        goto L_801486C0;
    if (id >= 0x52)
        goto L_801486AC;
    if (id >= 0x46)
        goto kind2;
    if (id >= 0x44)
        goto kind0;
    goto kind2;

L_801486AC:
    if (id >= 0x5d)
        goto kind2;
    if (id >= 0x58)
        goto kind0;
    goto kind1;

L_801486C0:
    if (id == 0x93)
        goto kind0;
    if (id >= 0x93)
        goto L_801486E0;
    if (id >= 0x6a)
        goto kind2;
    if (id >= 0x65)
        goto kind1;
    goto kind2;

L_801486E0:
    if (id >= 0xce)
        goto kind0;
    goto kind2;

L_801486EC:
    if (id >= 0x103)
        goto L_8014874C;
    if (id == 0xeb)
        goto kind2;
    if (id >= 0xeb)
        goto L_80148728;
    if (id >= 0xdf)
        goto L_80148714;
    if (id == 0xdc)
        goto kind1;
    goto kind2;

L_80148714:
    if (id >= 0xea)
        goto kind3;
    if (id >= 0xe3)
        goto kind2;
    goto kind1;

L_80148728:
    if (id == 0xf7)
        goto kind0;
    if (id >= 0xf7)
        goto L_80148740;
    if (id >= 0xed)
        goto kind3;
    goto kind0;

L_80148740:
    if (id == 0xff)
        goto kind2;
    goto kind3;

L_8014874C:
    if (id == 0x117)
        goto kind1;
    if (id >= 0x117)
        goto L_80148780;
    if (id >= 0x109)
        goto L_8014876C;
    if (id == 0x106)
        goto kind3;
    goto kind2;

L_8014876C:
    if (id >= 0x111)
        goto kind3;
    if (id >= 0x10d)
        goto kind2;
    goto kind3;

L_80148780:
    if (id == 0x12d)
        goto kind0;
    if (id >= 0x12d)
        goto kind2;
    if (id == 0x11e)
        goto kind0;
    goto kind2;

kind0:
    kind = 0;
    goto kind_done;
kind1:
    kind = 1;
    goto kind_done;
kind3:
    kind = 3;
    goto kind_done;
kind2:
    kind = 2;
kind_done:

    if (kind == 3) {
        *(u32*)this->unk1528 = 0;
    }

    reinterpret_cast<BattleStateV8If*>(this)->vf2C(entry);

    // Retail init order after the first vt+0x2C call:
    //   slot=this+8, i=0, then one=1 / thirteen=13 into r26/r27.
    slot = (cf::CBattleStateEntry*)((u8*)this + 0x8);
    i = 0;
    one = 1;
    thirteen = 0xd;

    for (; i < 0x68; i++, slot++) {
        u32 savedId;
        int stillActive;
        u32 savedWords[0x34 / 4];
        u32* s;
        u32 a;
        u32 b;
        void* clearPtr;
        int clearVal;
        int clearLen;
        int trip;
        int g;
        cf::CBattleStateEntryArray* v;

        if (slot->unk0C != entry->unk0C) {
            continue;
        }

        if ((entry->unk30 & 0x200) == 0) {
            if (slot->unk00 != entry->unk00) {
                continue;
            }
            if (slot->unk04 != entry->unk04) {
                continue;
            }
            if (slot->unk08 != entry->unk08) {
                continue;
            }
            if (slot->unk2E != entry->unk2E) {
                continue;
            }
        }

        // Retail schedules memset args into the first pair of the
        // word-copy (lwz r6/r0, mr dest, li val/len, stw pair hi/lo).
        s = (u32*)slot;
        a = s[0];
        clearPtr = slot;
        b = s[1];
        clearVal = 0;
        savedWords[1] = b;
        clearLen = 0x34;
        savedWords[0] = a;
        a = s[2];
        b = s[3];
        savedWords[3] = b;
        savedWords[2] = a;
        a = s[4];
        b = s[5];
        savedWords[5] = b;
        savedWords[4] = a;
        a = s[6];
        b = s[7];
        savedWords[7] = b;
        savedWords[6] = a;
        a = s[8];
        b = s[9];
        savedWords[9] = b;
        savedWords[8] = a;
        a = s[10];
        b = s[11];
        savedWords[11] = b;
        savedWords[10] = a;
        savedWords[12] = s[12];
        memset(clearPtr, clearVal, clearLen);

        reinterpret_cast<BattleStateV8If*>(this)->vf2C(
            (cf::CBattleStateEntry*)savedWords);

        // Load halfword id into a wide local first (retail lhz -> r5).
        savedId = *(u16*)((u8*)savedWords + 0xc);
        if (savedId >= 0x12f) {
            stillActive = 0;
        } else {
            // Retail: savedId in r5, dead trip in r3 (li 0 / addi +7 /
            // unused after bdnz), scan base in r4, ctr=thirteen.
            // Indexing entries[g*8+j] straight from `this` keeps the base
            // `this` (offsets 0x14+0x34*j folded into the loads, +0x1a0
            // increment) so no separate this+8 value needs a callee-saved
            // reg -- the p = this+8 form costs stmw r25 vs retail stmw r26.
            trip = 0;
            stillActive = 0;
            for (g = thirteen; g != 0; g--) {
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 0].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 1].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 2].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 3].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 4].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 5].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 6].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId ==
                    ((cf::CBattleStateEntryArray*)this)
                        ->entries[g * 8 + 7].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                trip += 7;
            }
            stillActive = 0;
        scan_done:
            stillActive = stillActive + (trip & 0);
        }

        if (stillActive == 0) {
            u8* wordPtr = this->unk15AC + ((savedId >> 3) & ~3u);
            *(u32*)wordPtr &= ~(one << (savedId & 0x1F));
        }

        reinterpret_cast<BattleStateV8If*>(this)->vf4C(
            (cf::CBattleStateEntry*)savedWords);

        if (entry->unk0C == 0) {
            break;
        }
    }
}

// Batch 2026-07-14k: battlestate-vfunc10 owns CBattleState_UnkVirtualFunc10
// exclusively. Do not touch ctor / other vfuncs.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc8). Early-out when arg->unk2E == 0. Walks
// this+0x8 (0x68 x stride 0x34): match on slot->unk2E (== arg->unk2E);
// when arg->unk30 bit 0x200 is clear also require unk00/04/08 eq. Same
// flat if+goto kind tree on the *slot* id as UnkVirtualFunc8; kind==3
// clears this+0x1528. Stack-copy + memset slot, id-dup scan / clear
// unk15AC bit, then vt+0x4C (UnkVirtualFunc18). No recursive vt+0x2C;
// walks all slots (no early break).
void cf::CBattleState::CBattleState_UnkVirtualFunc10(cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    int one;
    int thirteen;
    cf::CBattleStateEntry* slot;
    int i;
    // Full arg spill (retail sp+0x3c..0x6c). Do not keep stackedWords[0] in a
    // separate local -- retail reloads it from the frame on the optional-eq
    // path; holding it live costs an extra CSR (stmw r21 vs retail stmw r22).
    u32 stackedWords[0x34 / 4];
    u32* aw;
    u16 key;
    u32 flagBit;
    u32 a04;
    u32 a08;

    if (arg->unk2E == 0) {
        return;
    }

    aw = (u32*)arg;
    stackedWords[0] = aw[0];
    stackedWords[1] = aw[1];
    stackedWords[2] = aw[2];
    stackedWords[3] = aw[3];
    stackedWords[4] = aw[4];
    stackedWords[5] = aw[5];
    stackedWords[6] = aw[6];
    stackedWords[7] = aw[7];
    stackedWords[8] = aw[8];
    stackedWords[9] = aw[9];
    stackedWords[10] = aw[10];
    stackedWords[11] = aw[11];
    stackedWords[12] = aw[12];

    // Match key = lhz of low half of word at arg+0x2C (unk2E). a04/a08 stay
    // live in GPRs (retail r29/r30); a00 is only read back from the spill.
    key = *(u16*)((u8*)stackedWords + 0x2e);
    flagBit = stackedWords[12] & 0x200;
    a04 = stackedWords[1];
    a08 = stackedWords[2];

    one = 1;
    thirteen = 0xd;
    slot = (cf::CBattleStateEntry*)((u8*)this + 0x8);
    i = 0;

    for (; i < 0x68; i++, slot++) {
        int id;
        int kind;
        u32 savedId;
        int stillActive;
        u32 savedWords[0x34 / 4];
        u32* s;
        u32 a;
        u32 b;
        void* clearPtr;
        int clearVal;
        int clearLen;
        int trip;
        int g;
        cf::CBattleStateEntry* p;

        if (slot->unk2E != key) {
            continue;
        }

        if (flagBit == 0) {
            if (slot->unk00 != stackedWords[0]) {
                continue;
            }
            if (slot->unk04 != a04) {
                continue;
            }
            if (slot->unk08 != a08) {
                continue;
            }
        }

        // Flat if+goto mirrors retail's cmpwi/beq/bge chain on slot->unk0C
        // (same leaf set as UnkVirtualFunc8).
        id = slot->unk0C;

        if (id >= 0xd4)
            goto L10_80148BE0;
        if (id >= 0x3e)
            goto L10_80148B78;
        if (id >= 0x2c)
            goto L10_80148B4C;
        if (id == 0x27)
            goto kind0;
        if (id >= 0x27)
            goto L10_80148B40;
        if (id >= 4)
            goto L10_80148B34;
        if (id >= 2)
            goto kind0;
        goto kind2;

    L10_80148B34:
        if (id >= 0x14)
            goto kind2;
        goto kind1;

    L10_80148B40:
        if (id >= 0x2a)
            goto kind1;
        goto kind2;

    L10_80148B4C:
        if (id == 0x36)
            goto kind0;
        if (id >= 0x36)
            goto L10_80148B6C;
        if (id >= 0x35)
            goto kind2;
        if (id >= 0x33)
            goto kind0;
        goto kind2;

    L10_80148B6C:
        if (id >= 0x3c)
            goto kind1;
        goto kind2;

    L10_80148B78:
        if (id == 0x5f)
            goto kind0;
        if (id >= 0x5f)
            goto L10_80148BB4;
        if (id >= 0x52)
            goto L10_80148BA0;
        if (id >= 0x46)
            goto kind2;
        if (id >= 0x44)
            goto kind0;
        goto kind2;

    L10_80148BA0:
        if (id >= 0x5d)
            goto kind2;
        if (id >= 0x58)
            goto kind0;
        goto kind1;

    L10_80148BB4:
        if (id == 0x93)
            goto kind0;
        if (id >= 0x93)
            goto L10_80148BD4;
        if (id >= 0x6a)
            goto kind2;
        if (id >= 0x65)
            goto kind1;
        goto kind2;

    L10_80148BD4:
        if (id >= 0xce)
            goto kind0;
        goto kind2;

    L10_80148BE0:
        if (id >= 0x103)
            goto L10_80148C40;
        if (id == 0xeb)
            goto kind2;
        if (id >= 0xeb)
            goto L10_80148C1C;
        if (id >= 0xdf)
            goto L10_80148C08;
        if (id == 0xdc)
            goto kind1;
        goto kind2;

    L10_80148C08:
        if (id >= 0xea)
            goto kind3;
        if (id >= 0xe3)
            goto kind2;
        goto kind1;

    L10_80148C1C:
        if (id == 0xf7)
            goto kind0;
        if (id >= 0xf7)
            goto L10_80148C34;
        if (id >= 0xed)
            goto kind3;
        goto kind0;

    L10_80148C34:
        if (id == 0xff)
            goto kind2;
        goto kind3;

    L10_80148C40:
        if (id == 0x117)
            goto kind1;
        if (id >= 0x117)
            goto L10_80148C74;
        if (id >= 0x109)
            goto L10_80148C60;
        if (id == 0x106)
            goto kind3;
        goto kind2;

    L10_80148C60:
        if (id >= 0x111)
            goto kind3;
        if (id >= 0x10d)
            goto kind2;
        goto kind3;

    L10_80148C74:
        if (id == 0x12d)
            goto kind0;
        if (id >= 0x12d)
            goto kind2;
        if (id == 0x11e)
            goto kind0;
        goto kind2;

    kind0:
        kind = 0;
        goto kind_done;
    kind1:
        kind = 1;
        goto kind_done;
    kind3:
        kind = 3;
        goto kind_done;
    kind2:
        kind = 2;
    kind_done:

        if (kind == 3) {
            *(u32*)this->unk1528 = 0;
        }

        // Retail schedules memset args into the first pair of the
        // word-copy (lwz r6/r0, mr dest, li val/len, stw pair hi/lo).
        s = (u32*)slot;
        a = s[0];
        clearPtr = slot;
        b = s[1];
        clearVal = 0;
        savedWords[1] = b;
        clearLen = 0x34;
        savedWords[0] = a;
        a = s[2];
        b = s[3];
        savedWords[3] = b;
        savedWords[2] = a;
        a = s[4];
        b = s[5];
        savedWords[5] = b;
        savedWords[4] = a;
        a = s[6];
        b = s[7];
        savedWords[7] = b;
        savedWords[6] = a;
        a = s[8];
        b = s[9];
        savedWords[9] = b;
        savedWords[8] = a;
        a = s[10];
        b = s[11];
        savedWords[11] = b;
        savedWords[10] = a;
        savedWords[12] = s[12];
        memset(clearPtr, clearVal, clearLen);

        // Load halfword id into a wide local first (retail lhz -> r5).
        savedId = *(u16*)((u8*)savedWords + 0xc);
        if (savedId >= 0x12f) {
            stillActive = 0;
        } else {
            // Retail: found in r0, dead trip in r3 (li 0 / addi +7 /
            // unused after bdnz), scan base in r4.
            p = (cf::CBattleStateEntry*)((u8*)this + 0x8);
            trip = 0;
            stillActive = 0;
            for (g = thirteen; g != 0; g--) {
                if (savedId == p[0].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[1].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[2].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[3].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[4].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[5].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[6].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == p[7].unk0C) {
                    stillActive = 1;
                    goto scan_done;
                }
                p += 8;
                trip += 7;
            }
            stillActive = 0;
        scan_done:
            stillActive |= trip & 0;
        }

        if (stillActive == 0) {
            u8* wordPtr = this->unk15AC + ((savedId >> 3) & ~3u);
            *(u32*)wordPtr &= ~(one << (savedId & 0x1F));
        }

        ((Vfunc18Fn)(*(void***)this)[19])(
            this, (cf::CBattleStateEntry*)savedWords);
    }
}

// Batch 2026-07-16: battlestate-vfunc5 owns CBattleState_UnkVirtualFunc5
// exclusively. Do not touch ctor / other vfuncs.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc6). Core battle-state-machine: id-specific
// init, kind-based routing through vfunc1/2 helpers + sound/event dispatch,
// then slot scan + copy/accumulate for entries sharing the same id/keys.
void cf::CBattleState::CBattleState_UnkVirtualFunc5(cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    u32 id;
    int kind2;

    id = arg->unk0C;

    // -- Phase 1: id-specific init (flat if/goto to match retail's
    //   cmpwi/beq/bge chain) -----------------------------------------
    if (id == 0xf)
        goto P1_setzero;
    if (id >= 0xf)
        goto P1_geF;
    if (id == 0x9)
        goto P1_setzero;
    if (id >= 0x9)
        goto P1_ge9;
    if (id == 0x6)
        goto P1_setzero;
    goto P1_done;

P1_ge9:
    if (id >= 0xd)
        goto P1_done;
    if (id >= 0xb)
        goto P1_setzero;
    goto P1_done;

P1_geF:
    if (id >= 0x2e)
        goto P1_ge2E;
    if (id >= 0x2c)
        goto P1_2c;
    if (id >= 0x11)
        goto P1_done;
    goto P1_10;

P1_2c:
    if (arg->unk10 == 0) {
        arg->unk10 = 0x64;
    }
    goto P1_done;

P1_ge2E:
    if (id == 0x35) {
        arg->unk24 = lbl_eu_80667400;
    }
    goto P1_done;

P1_10:
    arg->unk04 = 0;
    arg->unk00 = 0;
    {
        cf::CBattleState* obj;
        obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
        if (*(u32*)((u8*)obj + 0x3374) & 0x20) {
            arg->unk20 *= lbl_eu_80667404;
        } else {
            obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
            if (*(u32*)((u8*)obj + 0x3374) & 0x40) {
                arg->unk20 = (float)((double)arg->unk20 * lbl_eu_80667408);
            }
        }
    }
    goto P1_done;

P1_setzero:
    arg->unk04 = 0;
    arg->unk00 = 0;

P1_done:

    // -- Phase 2: set unk30 bit 0 based on unk20 vs constant ---------
    if (lbl_eu_80667410 == arg->unk20) {
        arg->unk30 |= 1;
    } else {
        arg->unk30 &= ~1u;
    }

    // -- Phase 3: kind classification #1 (r0 in retail) --------------
    {
        int k;
        if (id >= 0xd4) goto K1_5;
        if (id >= 0x3e) goto K1_4;
        if (id >= 0x2c) goto K1_3;
        if (id == 0x27) { k = 0; goto K1_done; }
        if (id >= 0x27) goto K1_2;
        if (id >= 4) goto K1_1;
        if (id >= 2) { k = 0; goto K1_done; }
        k = 2; goto K1_done;
    K1_1: if (id >= 0x14) { k = 2; goto K1_done; } k = 1; goto K1_done;
    K1_2: if (id >= 0x2a) { k = 1; goto K1_done; } k = 2; goto K1_done;
    K1_3: if (id == 0x36) { k = 0; goto K1_done; }
        if (id >= 0x36) { goto K1_3b; }
        if (id >= 0x35) { k = 2; goto K1_done; }
        if (id >= 0x33) { k = 0; goto K1_done; }
        k = 2; goto K1_done;
    K1_3b: if (id >= 0x3c) { k = 1; goto K1_done; } k = 2; goto K1_done;
    K1_4: if (id == 0x5f) { k = 0; goto K1_done; }
        if (id >= 0x5f) goto K1_4b;
        if (id >= 0x52) goto K1_4c;
        if (id >= 0x46) { k = 2; goto K1_done; }
        if (id >= 0x44) { k = 0; goto K1_done; }
        k = 2; goto K1_done;
    K1_4c: if (id >= 0x5d) { k = 2; goto K1_done; } if (id >= 0x58) { k = 0; goto K1_done; } k = 1; goto K1_done;
    K1_4b: if (id == 0x93) { k = 0; goto K1_done; }
        if (id >= 0x93) goto K1_4d;
        if (id >= 0x6a) { k = 2; goto K1_done; }
        if (id >= 0x65) { k = 1; goto K1_done; }
        k = 2; goto K1_done;
    K1_4d: if (id >= 0xce) { k = 0; goto K1_done; } k = 2; goto K1_done;
    K1_5: if (id >= 0x103) goto K1_5b;
        if (id == 0xeb) { k = 2; goto K1_done; }
        if (id >= 0xeb) goto K1_5c;
        if (id >= 0xdf) goto K1_5d;
        if (id == 0xdc) { k = 1; goto K1_done; }
        k = 2; goto K1_done;
    K1_5d: if (id >= 0xea) { k = 3; goto K1_done; } if (id >= 0xe3) { k = 2; goto K1_done; } k = 1; goto K1_done;
    K1_5c: if (id == 0xf7) { k = 0; goto K1_done; }
        if (id >= 0xf7) goto K1_5e;
        if (id >= 0xed) { k = 3; goto K1_done; }
        k = 0; goto K1_done;
    K1_5e: if (id == 0xff) { k = 2; goto K1_done; } k = 3; goto K1_done;
    K1_5b: if (id == 0x117) { k = 1; goto K1_done; }
        if (id >= 0x117) goto K1_5f;
        if (id >= 0x109) goto K1_5g;
        if (id == 0x106) { k = 3; goto K1_done; }
        k = 2; goto K1_done;
    K1_5g: if (id >= 0x111) { k = 3; goto K1_done; } if (id >= 0x10d) { k = 2; goto K1_done; } k = 3; goto K1_done;
    K1_5f: if (id == 0x12d) { k = 0; goto K1_done; } if (id >= 0x12d) { k = 2; goto K1_done; } if (id == 0x11e) { k = 0; goto K1_done; } k = 2;
    K1_done:
        if (k == 3) {
            *(u32*)this->unk1528 = id;
        }
    }

    // -- Phase 4: set bitfield at this+0x15AC ------------------------
    {
        u32 wordOff = (id >> 3) & ~3u;
        u32 bitPos = id & 0x1F;
        *(u32*)(this->unk15AC + wordOff) |= (1u << bitPos);
    }

    // -- Phase 5: kind classification #2 (r31 in retail) -------------
    if (id >= 0xd4) goto K2_5;
    if (id >= 0x3e) goto K2_4;
    if (id >= 0x2c) goto K2_3;
    if (id == 0x27) { kind2 = 0; goto K2_done; }
    if (id >= 0x27) goto K2_2;
    if (id >= 4) goto K2_1;
    if (id >= 2) { kind2 = 0; goto K2_done; }
    kind2 = 2; goto K2_done;
K2_1: if (id >= 0x14) { kind2 = 2; goto K2_done; } kind2 = 1; goto K2_done;
K2_2: if (id >= 0x2a) { kind2 = 1; goto K2_done; } kind2 = 2; goto K2_done;
K2_3: if (id == 0x36) { kind2 = 0; goto K2_done; }
    if (id >= 0x36) { goto K2_3b; }
    if (id >= 0x35) { kind2 = 2; goto K2_done; }
    if (id >= 0x33) { kind2 = 0; goto K2_done; }
    kind2 = 2; goto K2_done;
K2_3b: if (id >= 0x3c) { kind2 = 1; goto K2_done; } kind2 = 2; goto K2_done;
K2_4: if (id == 0x5f) { kind2 = 0; goto K2_done; }
    if (id >= 0x5f) goto K2_4b;
    if (id >= 0x52) goto K2_4c;
    if (id >= 0x46) { kind2 = 2; goto K2_done; }
    if (id >= 0x44) { kind2 = 0; goto K2_done; }
    kind2 = 2; goto K2_done;
K2_4c: if (id >= 0x5d) { kind2 = 2; goto K2_done; } if (id >= 0x58) { kind2 = 0; goto K2_done; } kind2 = 1; goto K2_done;
K2_4b: if (id == 0x93) { kind2 = 0; goto K2_done; }
    if (id >= 0x93) goto K2_4d;
    if (id >= 0x6a) { kind2 = 2; goto K2_done; }
    if (id >= 0x65) { kind2 = 1; goto K2_done; }
    kind2 = 2; goto K2_done;
K2_4d: if (id >= 0xce) { kind2 = 0; goto K2_done; } kind2 = 2; goto K2_done;
K2_5: if (id >= 0x103) goto K2_5b;
    if (id == 0xeb) { kind2 = 2; goto K2_done; }
    if (id >= 0xeb) goto K2_5c;
    if (id >= 0xdf) goto K2_5d;
    if (id == 0xdc) { kind2 = 1; goto K2_done; }
    kind2 = 2; goto K2_done;
K2_5d: if (id >= 0xea) { kind2 = 3; goto K2_done; } if (id >= 0xe3) { kind2 = 2; goto K2_done; } kind2 = 1; goto K2_done;
K2_5c: if (id == 0xf7) { kind2 = 0; goto K2_done; }
    if (id >= 0xf7) goto K2_5e;
    if (id >= 0xed) { kind2 = 3; goto K2_done; }
    kind2 = 0; goto K2_done;
K2_5e: if (id == 0xff) { kind2 = 2; goto K2_done; } kind2 = 3; goto K2_done;
K2_5b: if (id == 0x117) { kind2 = 1; goto K2_done; }
    if (id >= 0x117) goto K2_5f;
    if (id >= 0x109) goto K2_5g;
    if (id == 0x106) { kind2 = 3; goto K2_done; }
    kind2 = 2; goto K2_done;
K2_5g: if (id >= 0x111) { kind2 = 3; goto K2_done; } if (id >= 0x10d) { kind2 = 2; goto K2_done; } kind2 = 3; goto K2_done;
K2_5f: if (id == 0x12d) { kind2 = 0; goto K2_done; } if (id >= 0x12d) { kind2 = 2; goto K2_done; } if (id == 0x11e) { kind2 = 0; goto K2_done; } kind2 = 2;
K2_done:

    // -- Phase 6: choose Branch A or B based on arg->unk08 -----------
    if (arg->unk08 == 0x2000 || arg->unk08 == 0x4000 || arg->unk08 == 0x8000) {
        if (!(arg->unk30 & 0x200)) {
            goto BranchA;
        }
    }

BranchB:
    // -- Branch B (retail jumps to 0x801476AC) -----------------------
    {
        int k;
        if (id >= 0xd4) goto B5_5;
        if (id >= 0x3e) goto B5_4;
        if (id >= 0x2c) goto B5_3;
        if (id == 0x27) { k = 0; goto B5_done; }
        if (id >= 0x27) goto B5_2;
        if (id >= 4) goto B5_1;
        if (id >= 2) { k = 0; goto B5_done; }
        k = 2; goto B5_done;
    B5_1: if (id >= 0x14) { k = 2; goto B5_done; } k = 1; goto B5_done;
    B5_2: if (id >= 0x2a) { k = 1; goto B5_done; } k = 2; goto B5_done;
    B5_3: if (id == 0x36) { k = 0; goto B5_done; }
        if (id >= 0x36) { goto B5_3b; }
        if (id >= 0x35) { k = 2; goto B5_done; }
        if (id >= 0x33) { k = 0; goto B5_done; }
        k = 2; goto B5_done;
    B5_3b: if (id >= 0x3c) { k = 1; goto B5_done; } k = 2; goto B5_done;
    B5_4: if (id == 0x5f) { k = 0; goto B5_done; }
        if (id >= 0x5f) goto B5_4b;
        if (id >= 0x52) goto B5_4c;
        if (id >= 0x46) { k = 2; goto B5_done; }
        if (id >= 0x44) { k = 0; goto B5_done; }
        k = 2; goto B5_done;
    B5_4c: if (id >= 0x5d) { k = 2; goto B5_done; } if (id >= 0x58) { k = 0; goto B5_done; } k = 1; goto B5_done;
    B5_4b: if (id == 0x93) { k = 0; goto B5_done; }
        if (id >= 0x93) goto B5_4d;
        if (id >= 0x6a) { k = 2; goto B5_done; }
        if (id >= 0x65) { k = 1; goto B5_done; }
        k = 2; goto B5_done;
    B5_4d: if (id >= 0xce) { k = 0; goto B5_done; } k = 2; goto B5_done;
    B5_5: if (id >= 0x103) goto B5_5b;
        if (id == 0xeb) { k = 2; goto B5_done; }
        if (id >= 0xeb) goto B5_5c;
        if (id >= 0xdf) goto B5_5d;
        if (id == 0xdc) { k = 1; goto B5_done; }
        k = 2; goto B5_done;
    B5_5d: if (id >= 0xea) { k = 3; goto B5_done; } if (id >= 0xe3) { k = 2; goto B5_done; } k = 1; goto B5_done;
    B5_5c: if (id == 0xf7) { k = 0; goto B5_done; }
        if (id >= 0xf7) goto B5_5e;
        if (id >= 0xed) { k = 3; goto B5_done; }
        k = 0; goto B5_done;
    B5_5e: if (id == 0xff) { k = 2; goto B5_done; } k = 3; goto B5_done;
    B5_5b: if (id == 0x117) { k = 1; goto B5_done; }
        if (id >= 0x117) goto B5_5f;
        if (id >= 0x109) goto B5_5g;
        if (id == 0x106) { k = 3; goto B5_done; }
        k = 2; goto B5_done;
    B5_5g: if (id >= 0x111) { k = 3; goto B5_done; } if (id >= 0x10d) { k = 2; goto B5_done; } k = 3; goto B5_done;
    B5_5f: if (id == 0x12d) { k = 0; goto B5_done; } if (id >= 0x12d) { k = 2; goto B5_done; } if (id == 0x11e) { k = 0; goto B5_done; } k = 2;
    B5_done:
        if (k == 0) {
            if (!(arg->unk30 & 0x200)) {
                if (arg->unk2E == 0 || (arg->unk30 & 2) || !(arg->unk30 & 0x400)) {
                    cf::CBattleState* obj;
                    obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
                    func_80109784((u8*)obj + 0x3F10, id, 1);
                    func_8013DB6C(6, id, 0, 0);
                    goto after_dispatch;
                }
            }
        }
    }

    {
        int k;
        if (id >= 0xd4) goto B6_5;
        if (id >= 0x3e) goto B6_4;
        if (id >= 0x2c) goto B6_3;
        if (id == 0x27) { k = 0; goto B6_done; }
        if (id >= 0x27) goto B6_2;
        if (id >= 4) goto B6_1;
        if (id >= 2) { k = 0; goto B6_done; }
        k = 2; goto B6_done;
    B6_1: if (id >= 0x14) { k = 2; goto B6_done; } k = 1; goto B6_done;
    B6_2: if (id >= 0x2a) { k = 1; goto B6_done; } k = 2; goto B6_done;
    B6_3: if (id == 0x36) { k = 0; goto B6_done; }
        if (id >= 0x36) { goto B6_3b; }
        if (id >= 0x35) { k = 2; goto B6_done; }
        if (id >= 0x33) { k = 0; goto B6_done; }
        k = 2; goto B6_done;
    B6_3b: if (id >= 0x3c) { k = 1; goto B6_done; } k = 2; goto B6_done;
    B6_4: if (id == 0x5f) { k = 0; goto B6_done; }
        if (id >= 0x5f) goto B6_4b;
        if (id >= 0x52) goto B6_4c;
        if (id >= 0x46) { k = 2; goto B6_done; }
        if (id >= 0x44) { k = 0; goto B6_done; }
        k = 2; goto B6_done;
    B6_4c: if (id >= 0x5d) { k = 2; goto B6_done; } if (id >= 0x58) { k = 0; goto B6_done; } k = 1; goto B6_done;
    B6_4b: if (id == 0x93) { k = 0; goto B6_done; }
        if (id >= 0x93) goto B6_4d;
        if (id >= 0x6a) { k = 2; goto B6_done; }
        if (id >= 0x65) { k = 1; goto B6_done; }
        k = 2; goto B6_done;
    B6_4d: if (id >= 0xce) { k = 0; goto B6_done; } k = 2; goto B6_done;
    B6_5: if (id >= 0x103) goto B6_5b;
        if (id == 0xeb) { k = 2; goto B6_done; }
        if (id >= 0xeb) goto B6_5c;
        if (id >= 0xdf) goto B6_5d;
        if (id == 0xdc) { k = 1; goto B6_done; }
        k = 2; goto B6_done;
    B6_5d: if (id >= 0xea) { k = 3; goto B6_done; } if (id >= 0xe3) { k = 2; goto B6_done; } k = 1; goto B6_done;
    B6_5c: if (id == 0xf7) { k = 0; goto B6_done; }
        if (id >= 0xf7) goto B6_5e;
        if (id >= 0xed) { k = 3; goto B6_done; }
        k = 0; goto B6_done;
    B6_5e: if (id == 0xff) { k = 2; goto B6_done; } k = 3; goto B6_done;
    B6_5b: if (id == 0x117) { k = 1; goto B6_done; }
        if (id >= 0x117) goto B6_5f;
        if (id >= 0x109) goto B6_5g;
        if (id == 0x106) { k = 3; goto B6_done; }
        k = 2; goto B6_done;
    B6_5g: if (id >= 0x111) { k = 3; goto B6_done; } if (id >= 0x10d) { k = 2; goto B6_done; } k = 3; goto B6_done;
    B6_5f: if (id == 0x12d) { k = 0; goto B6_done; } if (id >= 0x12d) { k = 2; goto B6_done; } if (id == 0x11e) { k = 0; goto B6_done; } k = 2;
    B6_done:
        if (k == 1) {
            if (!(arg->unk30 & 0x200)) {
                if (arg->unk2E == 0 || (arg->unk30 & 2) || !(arg->unk30 & 0x400)) {
                    cf::CBattleState* obj;
                    obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
                    if (arg->unk30 & 0x10000) {
                        func_80109784((u8*)obj + 0x3F10, id, 0x20);
                    } else {
                        func_80109784((u8*)obj + 0x3F10, id, 2);
                    }
                    func_8013DB6C(6, id, 0, 0);
                    goto after_dispatch;
                }
            }
        }
    }

    {
        int k;
        if (id >= 0xd4) goto B7_5;
        if (id >= 0x3e) goto B7_4;
        if (id >= 0x2c) goto B7_3;
        if (id == 0x27) { k = 0; goto B7_done; }
        if (id >= 0x27) goto B7_2;
        if (id >= 4) goto B7_1;
        if (id >= 2) { k = 0; goto B7_done; }
        k = 2; goto B7_done;
    B7_1: if (id >= 0x14) { k = 2; goto B7_done; } k = 1; goto B7_done;
    B7_2: if (id >= 0x2a) { k = 1; goto B7_done; } k = 2; goto B7_done;
    B7_3: if (id == 0x36) { k = 0; goto B7_done; }
        if (id >= 0x36) { goto B7_3b; }
        if (id >= 0x35) { k = 2; goto B7_done; }
        if (id >= 0x33) { k = 0; goto B7_done; }
        k = 2; goto B7_done;
    B7_3b: if (id >= 0x3c) { k = 1; goto B7_done; } k = 2; goto B7_done;
    B7_4: if (id == 0x5f) { k = 0; goto B7_done; }
        if (id >= 0x5f) goto B7_4b;
        if (id >= 0x52) goto B7_4c;
        if (id >= 0x46) { k = 2; goto B7_done; }
        if (id >= 0x44) { k = 0; goto B7_done; }
        k = 2; goto B7_done;
    B7_4c: if (id >= 0x5d) { k = 2; goto B7_done; } if (id >= 0x58) { k = 0; goto B7_done; } k = 1; goto B7_done;
    B7_4b: if (id == 0x93) { k = 0; goto B7_done; }
        if (id >= 0x93) goto B7_4d;
        if (id >= 0x6a) { k = 2; goto B7_done; }
        if (id >= 0x65) { k = 1; goto B7_done; }
        k = 2; goto B7_done;
    B7_4d: if (id >= 0xce) { k = 0; goto B7_done; } k = 2; goto B7_done;
    B7_5: if (id >= 0x103) goto B7_5b;
        if (id == 0xeb) { k = 2; goto B7_done; }
        if (id >= 0xeb) goto B7_5c;
        if (id >= 0xdf) goto B7_5d;
        if (id == 0xdc) { k = 1; goto B7_done; }
        k = 2; goto B7_done;
    B7_5d: if (id >= 0xea) { k = 3; goto B7_done; } if (id >= 0xe3) { k = 2; goto B7_done; } k = 1; goto B7_done;
    B7_5c: if (id == 0xf7) { k = 0; goto B7_done; }
        if (id >= 0xf7) goto B7_5e;
        if (id >= 0xed) { k = 3; goto B7_done; }
        k = 0; goto B7_done;
    B7_5e: if (id == 0xff) { k = 2; goto B7_done; } k = 3; goto B7_done;
    B7_5b: if (id == 0x117) { k = 1; goto B7_done; }
        if (id >= 0x117) goto B7_5f;
        if (id >= 0x109) goto B7_5g;
        if (id == 0x106) { k = 3; goto B7_done; }
        k = 2; goto B7_done;
    B7_5g: if (id >= 0x111) { k = 3; goto B7_done; } if (id >= 0x10d) { k = 2; goto B7_done; } k = 3; goto B7_done;
    B7_5f: if (id == 0x12d) { k = 0; goto B7_done; } if (id >= 0x12d) { k = 2; goto B7_done; } if (id == 0x11e) { k = 0; goto B7_done; } k = 2;
    B7_done:
        if (k == 3) {
            if (!(arg->unk30 & 0x200)) {
                cf::CBattleState* obj;
                obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
                func_80109784((u8*)obj + 0x3F10, id, 1);
                func_8013DB6C(6, id, 0, 0);
            }
        }
    }

    goto after_dispatch;

BranchA:
    // -- Branch A: kind #3 -> if kind==0, call func_80109784(ptr, id, 5) --
    {
        int k;
        if (id >= 0xd4) goto A3_5;
        if (id >= 0x3e) goto A3_4;
        if (id >= 0x2c) goto A3_3;
        if (id == 0x27) { k = 0; goto A3_done; }
        if (id >= 0x27) goto A3_2;
        if (id >= 4) goto A3_1;
        if (id >= 2) { k = 0; goto A3_done; }
        k = 2; goto A3_done;
    A3_1: if (id >= 0x14) { k = 2; goto A3_done; } k = 1; goto A3_done;
    A3_2: if (id >= 0x2a) { k = 1; goto A3_done; } k = 2; goto A3_done;
    A3_3: if (id == 0x36) { k = 0; goto A3_done; }
        if (id >= 0x36) { goto A3_3b; }
        if (id >= 0x35) { k = 2; goto A3_done; }
        if (id >= 0x33) { k = 0; goto A3_done; }
        k = 2; goto A3_done;
    A3_3b: if (id >= 0x3c) { k = 1; goto A3_done; } k = 2; goto A3_done;
    A3_4: if (id == 0x5f) { k = 0; goto A3_done; }
        if (id >= 0x5f) goto A3_4b;
        if (id >= 0x52) goto A3_4c;
        if (id >= 0x46) { k = 2; goto A3_done; }
        if (id >= 0x44) { k = 0; goto A3_done; }
        k = 2; goto A3_done;
    A3_4c: if (id >= 0x5d) { k = 2; goto A3_done; } if (id >= 0x58) { k = 0; goto A3_done; } k = 1; goto A3_done;
    A3_4b: if (id == 0x93) { k = 0; goto A3_done; }
        if (id >= 0x93) goto A3_4d;
        if (id >= 0x6a) { k = 2; goto A3_done; }
        if (id >= 0x65) { k = 1; goto A3_done; }
        k = 2; goto A3_done;
    A3_4d: if (id >= 0xce) { k = 0; goto A3_done; } k = 2; goto A3_done;
    A3_5: if (id >= 0x103) goto A3_5b;
        if (id == 0xeb) { k = 2; goto A3_done; }
        if (id >= 0xeb) goto A3_5c;
        if (id >= 0xdf) goto A3_5d;
        if (id == 0xdc) { k = 1; goto A3_done; }
        k = 2; goto A3_done;
    A3_5d: if (id >= 0xea) { k = 3; goto A3_done; } if (id >= 0xe3) { k = 2; goto A3_done; } k = 1; goto A3_done;
    A3_5c: if (id == 0xf7) { k = 0; goto A3_done; }
        if (id >= 0xf7) goto A3_5e;
        if (id >= 0xed) { k = 3; goto A3_done; }
        k = 0; goto A3_done;
    A3_5e: if (id == 0xff) { k = 2; goto A3_done; } k = 3; goto A3_done;
    A3_5b: if (id == 0x117) { k = 1; goto A3_done; }
        if (id >= 0x117) goto A3_5f;
        if (id >= 0x109) goto A3_5g;
        if (id == 0x106) { k = 3; goto A3_done; }
        k = 2; goto A3_done;
    A3_5g: if (id >= 0x111) { k = 3; goto A3_done; } if (id >= 0x10d) { k = 2; goto A3_done; } k = 3; goto A3_done;
    A3_5f: if (id == 0x12d) { k = 0; goto A3_done; } if (id >= 0x12d) { k = 2; goto A3_done; } if (id == 0x11e) { k = 0; goto A3_done; } k = 2;
    A3_done:
        if (k == 0) {
            cf::CBattleState* obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
            func_80109784((u8*)obj + 0x3F10, id, 5);
        }
    }

    // -- Branch A: kind #4 -> func_80109784 with 6 or 4 --------------
    {
        int k;
        if (id >= 0xd4) goto A4_5;
        if (id >= 0x3e) goto A4_4;
        if (id >= 0x2c) goto A4_3;
        if (id == 0x27) { k = 0; goto A4_done; }
        if (id >= 0x27) goto A4_2;
        if (id >= 4) goto A4_1;
        if (id >= 2) { k = 0; goto A4_done; }
        k = 2; goto A4_done;
    A4_1: if (id >= 0x14) { k = 2; goto A4_done; } k = 1; goto A4_done;
    A4_2: if (id >= 0x2a) { k = 1; goto A4_done; } k = 2; goto A4_done;
    A4_3: if (id == 0x36) { k = 0; goto A4_done; }
        if (id >= 0x36) { goto A4_3b; }
        if (id >= 0x35) { k = 2; goto A4_done; }
        if (id >= 0x33) { k = 0; goto A4_done; }
        k = 2; goto A4_done;
    A4_3b: if (id >= 0x3c) { k = 1; goto A4_done; } k = 2; goto A4_done;
    A4_4: if (id == 0x5f) { k = 0; goto A4_done; }
        if (id >= 0x5f) goto A4_4b;
        if (id >= 0x52) goto A4_4c;
        if (id >= 0x46) { k = 2; goto A4_done; }
        if (id >= 0x44) { k = 0; goto A4_done; }
        k = 2; goto A4_done;
    A4_4c: if (id >= 0x5d) { k = 2; goto A4_done; } if (id >= 0x58) { k = 0; goto A4_done; } k = 1; goto A4_done;
    A4_4b: if (id == 0x93) { k = 0; goto A4_done; }
        if (id >= 0x93) goto A4_4d;
        if (id >= 0x6a) { k = 2; goto A4_done; }
        if (id >= 0x65) { k = 1; goto A4_done; }
        k = 2; goto A4_done;
    A4_4d: if (id >= 0xce) { k = 0; goto A4_done; } k = 2; goto A4_done;
    A4_5: if (id >= 0x103) goto A4_5b;
        if (id == 0xeb) { k = 2; goto A4_done; }
        if (id >= 0xeb) goto A4_5c;
        if (id >= 0xdf) goto A4_5d;
        if (id == 0xdc) { k = 1; goto A4_done; }
        k = 2; goto A4_done;
    A4_5d: if (id >= 0xea) { k = 3; goto A4_done; } if (id >= 0xe3) { k = 2; goto A4_done; } k = 1; goto A4_done;
    A4_5c: if (id == 0xf7) { k = 0; goto A4_done; }
        if (id >= 0xf7) goto A4_5e;
        if (id >= 0xed) { k = 3; goto A4_done; }
        k = 0; goto A4_done;
    A4_5e: if (id == 0xff) { k = 2; goto A4_done; } k = 3; goto A4_done;
    A4_5b: if (id == 0x117) { k = 1; goto A4_done; }
        if (id >= 0x117) goto A4_5f;
        if (id >= 0x109) goto A4_5g;
        if (id == 0x106) { k = 3; goto A4_done; }
        k = 2; goto A4_done;
    A4_5g: if (id >= 0x111) { k = 3; goto A4_done; } if (id >= 0x10d) { k = 2; goto A4_done; } k = 3; goto A4_done;
    A4_5f: if (id == 0x12d) { k = 0; goto A4_done; } if (id >= 0x12d) { k = 2; goto A4_done; } if (id == 0x11e) { k = 0; goto A4_done; } k = 2;
    A4_done:
        {
            cf::CBattleState* obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)this)[1])(this);
            if (k == 1) {
                func_80109784((u8*)obj + 0x3F10, id, 6);
            } else {
                func_80109784((u8*)obj + 0x3F10, id, 4);
            }
        }
    }

    func_8013DB6C(6, id, 0, 0);

after_dispatch:
    // -- Final: entry-slot scan + copy -------------------------------
    {
        u32 entryId = arg->unk0C;

        // Decision tree matching retail's cmpwi/beq/bge chain
        if (entryId == 0x10d)
            goto F_skip_scan;
        if (entryId >= 0x10d)
            goto F_ge10d;
        if (entryId == 0xd7)
            goto F_do_scan;
        if (entryId >= 0xd7)
            goto F_geD7;
        if (entryId == 0x99)
            goto F_skip_scan;
        if (entryId < 0x99)
            goto F_do_scan;
        if (entryId >= 0xd4)
            goto F_skip_scan;
        goto F_do_scan;

    F_geD7:
        if (entryId >= 0x107) {
            if (entryId >= 0x109)
                goto F_do_scan;
            goto F_skip_scan;
        }
        if (entryId >= 0xe4)
            goto F_do_scan;
        goto F_skip_scan;

    F_ge10d:
        if (entryId == 0x11e)
            goto F_do_scan;
        if (entryId >= 0x11e) {
            if (entryId == 0x124)
                goto F_skip_scan;
            if (entryId >= 0x120)
                goto F_do_scan;
            goto F_skip_scan;
        }
        if (entryId == 0x11a)
            goto F_do_scan;
        if (entryId >= 0x11a)
            goto F_skip_scan;
        if (entryId >= 0x118)
            goto F_skip_scan;
        goto F_do_scan;

    F_do_scan:
        {
            cf::CBattleStateEntry* slot;
            int count;

            slot = (cf::CBattleStateEntry*)((u8*)this + 0x8);
            for (count = 0x68; count != 0; count--, slot++) {
                if (slot->unk0C != entryId)
                    goto F_next;
                if (slot->unk00 != arg->unk00)
                    goto F_next;
                if (slot->unk04 != arg->unk04)
                    goto F_next;
                if (slot->unk08 != arg->unk08)
                    goto F_next;

                if (entryId - 0xf <= 1u) {
                    f32 old1C = slot->unk1C;
                    f32 old20 = slot->unk20;
                    f32 old28 = slot->unk28;
                    slot->unk00 = arg->unk00;
                    slot->unk04 = arg->unk04;
                    slot->unk08 = arg->unk08;
                    slot->unk0C = arg->unk0C;
                    slot->unk10 = arg->unk10;
                    slot->unk14 = arg->unk14;
                    slot->unk16 = arg->unk16;
                    slot->unk18 = arg->unk18;
                    slot->unk1A = arg->unk1A;
                    slot->unk1C = arg->unk1C + old1C;
                    slot->unk20 = arg->unk20 + old20;
                    slot->unk24 = arg->unk24;
                    slot->unk28 = arg->unk28 + old28;
                    slot->unk2C = arg->unk2C;
                    slot->unk2E = arg->unk2E;
                    slot->unk30 = arg->unk30 | 8;
                    ((Vfunc18Fn)(*(void***)this)[18])(this, slot);
                    goto F_slot_done;
                }

                if ((slot->unk30 & 8) && (arg->unk30 & 8)) {
                    s32 old10 = slot->unk10;
                    f32 old28 = slot->unk28;
                    slot->unk00 = arg->unk00;
                    slot->unk04 = arg->unk04;
                    slot->unk08 = arg->unk08;
                    slot->unk0C = arg->unk0C;
                    slot->unk10 = arg->unk10 + old10;
                    slot->unk14 = arg->unk14;
                    slot->unk16 = arg->unk16;
                    slot->unk18 = arg->unk18;
                    slot->unk1A = arg->unk1A;
                    slot->unk1C = arg->unk1C;
                    slot->unk20 = arg->unk20;
                    slot->unk24 = arg->unk24;
                    slot->unk28 = arg->unk28;
                    slot->unk2C = arg->unk2C;
                    slot->unk2E = arg->unk2E;
                    slot->unk30 = arg->unk30;
                    slot->unk1C = slot->unk20;
                    if (slot->unk18 < slot->unk10)
                        slot->unk10 = slot->unk18;
                    slot->unk28 = old28;
                    slot->unk30 |= 8;
                    ((Vfunc18Fn)(*(void***)this)[18])(this, slot);
                    goto F_slot_done;
                }

                if (slot->unk10 > (s32)arg->unk10) {
                    if (!(slot->unk20 < arg->unk20))
                        goto F_next;
                }
                {
                    f32 old28 = slot->unk28;
                    slot->unk00 = arg->unk00;
                    slot->unk04 = arg->unk04;
                    slot->unk08 = arg->unk08;
                    slot->unk0C = arg->unk0C;
                    slot->unk10 = arg->unk10;
                    slot->unk14 = arg->unk14;
                    slot->unk16 = arg->unk16;
                    slot->unk18 = arg->unk18;
                    slot->unk1A = arg->unk1A;
                    slot->unk1C = arg->unk1C;
                    slot->unk20 = arg->unk20;
                    slot->unk24 = arg->unk24;
                    slot->unk28 = arg->unk28;
                    slot->unk2C = arg->unk2C;
                    slot->unk2E = arg->unk2E;
                    slot->unk30 = arg->unk30;
                    slot->unk1C = slot->unk20;
                    slot->unk28 = old28;
                    slot->unk30 |= 8;
                    ((Vfunc18Fn)(*(void***)this)[18])(this, slot);
                    goto F_slot_done;
                }

            F_next:
                ;
            }
        }

        // 3-array empty-slot scan
        {
            u8* base1 = (u8*)this + 0x8;
            u8* base2 = (u8*)this + 0x688;
            u8* base3 = (u8*)this + 0xD08;
            int trip;

            for (trip = 0x20; trip != 0; trip--) {
                cf::CBattleStateEntry* dst;

                if (kind2 == 0)
                    dst = (cf::CBattleStateEntry*)base1;
                else if (kind2 == 1)
                    dst = (cf::CBattleStateEntry*)base2;
                else
                    dst = (cf::CBattleStateEntry*)base3;

                if (dst->unk0C == 0) {
                    dst->unk00 = arg->unk00;
                    dst->unk04 = arg->unk04;
                    dst->unk08 = arg->unk08;
                    dst->unk0C = arg->unk0C;
                    dst->unk10 = arg->unk10;
                    dst->unk14 = arg->unk14;
                    dst->unk16 = arg->unk16;
                    dst->unk18 = arg->unk18;
                    dst->unk1A = arg->unk1A;
                    dst->unk1C = arg->unk1C;
                    dst->unk20 = arg->unk20;
                    dst->unk24 = arg->unk24;
                    dst->unk28 = arg->unk28;
                    dst->unk2C = arg->unk2C;
                    dst->unk2E = arg->unk2E;
                    dst->unk30 = arg->unk30;
                    dst->unk1C = dst->unk20;
                    if (lbl_eu_80667410 == dst->unk28) {
                        dst->unk28 = lbl_eu_80667414 * dst->unk24;
                    }
                    ((Vfunc18Fn)(*(void***)this)[18])(this, dst);
                    goto F_slot_done;
                }

                base1 += 0x34;
                base2 += 0x34;
                base3 += 0x34;
            }
        }
    }

F_skip_scan:
F_slot_done:
    ;
}

// func_80145C00: r3 = status id. Classifies the id through the same
// cmpwi/beq/bge decision tree as CBattleState_UnkVirtualFunc8 (kind 0/1/2/3)
// and returns (kind == 3) via the branchless subi/cntlzw/srwi boolify.
// Flat if+goto mirrors retail's tree 1:1; shared kind leaves keep the
// single merge at the end (retail 0x801467B0).
extern "C" bool func_80145C00(int value) {
    int kind;

    if (value >= 0xd4)
        goto L_E8;
    if (value >= 0x3e)
        goto L_80;
    if (value >= 0x2c)
        goto L_54;
    if (value == 0x27)
        goto kind0;
    if (value >= 0x27)
        goto L_48;
    if (value >= 4)
        goto L_3C;
    if (value >= 2)
        goto kind0;
    goto kind2;

L_3C:
    if (value >= 0x14)
        goto kind2;
    goto kind1;

L_48:
    if (value >= 0x2a)
        goto kind1;
    goto kind2;

L_54:
    if (value == 0x36)
        goto kind0;
    if (value >= 0x36)
        goto L_74;
    if (value >= 0x35)
        goto kind2;
    if (value >= 0x33)
        goto kind0;
    goto kind2;

L_74:
    if (value >= 0x3c)
        goto kind1;
    goto kind2;

L_80:
    if (value == 0x5f)
        goto kind0;
    if (value >= 0x5f)
        goto L_BC;
    if (value >= 0x52)
        goto L_A8;
    if (value >= 0x46)
        goto kind2;
    if (value >= 0x44)
        goto kind0;
    goto kind2;

L_A8:
    if (value >= 0x5d)
        goto kind2;
    if (value >= 0x58)
        goto kind0;
    goto kind1;

L_BC:
    if (value == 0x93)
        goto kind0;
    if (value >= 0x93)
        goto L_DC;
    if (value >= 0x6a)
        goto kind2;
    if (value >= 0x65)
        goto kind1;
    goto kind2;

L_DC:
    if (value >= 0xce)
        goto kind0;
    goto kind2;

L_E8:
    if (value >= 0x103)
        goto L_48b;
    if (value == 0xeb)
        goto kind2;
    if (value >= 0xeb)
        goto L_24;
    if (value >= 0xdf)
        goto L_10;
    if (value == 0xdc)
        goto kind1;
    goto kind2;

L_10:
    if (value >= 0xea)
        goto kind3;
    if (value >= 0xe3)
        goto kind2;
    goto kind1;

L_24:
    if (value == 0xf7)
        goto kind0;
    if (value >= 0xf7)
        goto L_3Cb;
    if (value >= 0xed)
        goto kind3;
    goto kind0;

L_3Cb:
    if (value == 0xff)
        goto kind2;
    goto kind3;

L_48b:
    if (value == 0x117)
        goto kind1;
    if (value >= 0x117)
        goto L_7C;
    if (value >= 0x109)
        goto L_68;
    if (value == 0x106)
        goto kind3;
    goto kind2;

L_68:
    if (value >= 0x111)
        goto kind3;
    if (value >= 0x10d)
        goto kind2;
    goto kind3;

L_7C:
    if (value == 0x12d)
        goto kind0;
    if (value >= 0x12d)
        goto kind2;
    if (value == 0x11e)
        goto kind0;
    goto kind2;

kind0:
    kind = 0;
    goto kind_done;
kind1:
    kind = 1;
    goto kind_done;
kind3:
    kind = 3;
    goto kind_done;
kind2:
    kind = 2;
kind_done:

    return kind == 3;
}
extern "C" bool func_80145DBC(int value) { int result; switch (value) { case 2: case 3: case 39: case 51: case 52: case 54: case 68: case 69: case 88: case 89: case 90: case 91: case 92: case 95: case 147: case 206: case 207: case 208: case 209: case 210: case 211: case 236: case 247: case 286: case 301: result = 0; break; case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 42: case 43: case 60: case 61: case 82: case 83: case 84: case 85: case 86: case 87: case 101: case 102: case 103: case 104: case 105: case 220: case 223: case 224: case 225: case 226: case 279: result = 1; break; case 234: case 237: case 238: case 239: case 240: case 241: case 242: case 243: case 244: case 245: case 246: case 248: case 249: case 250: case 251: case 252: case 253: case 254: case 256: case 257: case 258: case 262: case 265: case 266: case 267: case 268: case 273: case 274: case 275: case 276: case 277: case 278: result = 3; break; default: result = 2; break; } return result == 1; }
// func_80145F78: r3 = status id. Ids 0xf/0x10 return 0 immediately (retail
// subi/cmplwi/bgt guard); everything else is classified through the same
// cmpwi/beq/bge decision tree as func_80145C00 (kind 0/1/2/3) and the result
// is (kind == 1) via the branchless subi/cntlzw/srwi boolify. Flat if+goto
// mirrors retail's tree 1:1 (same convention as func_80145C00).
extern "C" bool func_80145F78(int value) {
    int kind;

    if (value - 0xf <= 1u) {
        return 0;
    }

    if (value >= 0xd4)
        goto L_E8;
    if (value >= 0x3e)
        goto L_80;
    if (value >= 0x2c)
        goto L_54;
    if (value == 0x27)
        goto kind0;
    if (value >= 0x27)
        goto L_48;
    if (value >= 4)
        goto L_3C;
    if (value >= 2)
        goto kind0;
    goto kind2;

L_3C:
    if (value >= 0x14)
        goto kind2;
    goto kind1;

L_48:
    if (value >= 0x2a)
        goto kind1;
    goto kind2;

L_54:
    if (value == 0x36)
        goto kind0;
    if (value >= 0x36)
        goto L_74;
    if (value >= 0x35)
        goto kind2;
    if (value >= 0x33)
        goto kind0;
    goto kind2;

L_74:
    if (value >= 0x3c)
        goto kind1;
    goto kind2;

L_80:
    if (value == 0x5f)
        goto kind0;
    if (value >= 0x5f)
        goto L_BC;
    if (value >= 0x52)
        goto L_A8;
    if (value >= 0x46)
        goto kind2;
    if (value >= 0x44)
        goto kind0;
    goto kind2;

L_A8:
    if (value >= 0x5d)
        goto kind2;
    if (value >= 0x58)
        goto kind0;
    goto kind1;

L_BC:
    if (value == 0x93)
        goto kind0;
    if (value >= 0x93)
        goto L_DC;
    if (value >= 0x6a)
        goto kind2;
    if (value >= 0x65)
        goto kind1;
    goto kind2;

L_DC:
    if (value >= 0xce)
        goto kind0;
    goto kind2;

L_E8:
    if (value >= 0x103)
        goto L_48b;
    if (value == 0xeb)
        goto kind2;
    if (value >= 0xeb)
        goto L_24;
    if (value >= 0xdf)
        goto L_10;
    if (value == 0xdc)
        goto kind1;
    goto kind2;

L_10:
    if (value >= 0xea)
        goto kind3;
    if (value >= 0xe3)
        goto kind2;
    goto kind1;

L_24:
    if (value == 0xf7)
        goto kind0;
    if (value >= 0xf7)
        goto L_3Cb;
    if (value >= 0xed)
        goto kind3;
    goto kind0;

L_3Cb:
    if (value == 0xff)
        goto kind2;
    goto kind3;

L_48b:
    if (value == 0x117)
        goto kind1;
    if (value >= 0x117)
        goto L_7C;
    if (value >= 0x109)
        goto L_68;
    if (value == 0x106)
        goto kind3;
    goto kind2;

L_68:
    if (value >= 0x111)
        goto kind3;
    if (value >= 0x10d)
        goto kind2;
    goto kind3;

L_7C:
    if (value == 0x12d)
        goto kind0;
    if (value >= 0x12d)
        goto kind2;
    if (value == 0x11e)
        goto kind0;
    goto kind2;

kind0:
    kind = 0;
    goto kind_done;
kind1:
    kind = 1;
    goto kind_done;
kind3:
    kind = 3;
    goto kind_done;
kind2:
    kind = 2;
kind_done:

    return kind == 1;
}
extern "C" bool func_80146148(int value) { int result; switch (value) { case 2: case 3: case 0x27: case 0x33: case 0x34: case 0x36: case 0x44: case 0x45: case 0x58: case 0x59: case 0x5a: case 0x5b: case 0x5c: case 0x5f: case 0x93: case 0xce: case 0xcf: case 0xd0: case 0xd1: case 0xd2: case 0xd3: case 0xec: case 0xf7: case 0x11e: case 0x12d: result = 0; break; case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 0x2a: case 0x2b: case 0x3c: case 0x3d: case 0x52: case 0x53: case 0x54: case 0x55: case 0x56: case 0x57: case 0x65: case 0x66: case 0x67: case 0x68: case 0x69: case 0xdc: case 0xdf: case 0xe0: case 0xe1: case 0xe2: case 0x117: result = 1; break; case 0xea: case 0xed: case 0xee: case 0xef: case 0xf0: case 0xf1: case 0xf2: case 0xf3: case 0xf4: case 0xf5: case 0xf6: case 0xf8: case 0xf9: case 0xfa: case 0xfb: case 0xfc: case 0xfd: case 0xfe: case 0x100: case 0x101: case 0x102: case 0x106: case 0x109: case 0x10a: case 0x10b: case 0x10c: case 0x111: case 0x112: case 0x113: case 0x114: case 0x115: case 0x116: result = 3; break; default: result = 2; break; } return result == 0; }
extern "C" bool func_80146384(unsigned int value) { return value - 0x125u <= 5u; }
extern "C" void CBattleState_UnkVirtualFunc19__Q22cf12CBattleStateFv() {}
extern "C" int CBattleState_UnkVirtualFunc1__Q22cf12CBattleStateFv() { return 0; }
extern "C" void CBattleState_UnkVirtualFunc17__Q22cf12CBattleStateFv() {}
extern "C" void CBattleState_UnkVirtualFunc18__Q22cf12CBattleStateFv() {}

// Batch 2026-08: battlestate-vfunc12 owns CBattleState_UnkVirtualFunc12
// exclusively. Retail symbol mangles Fv but the caller leaves the status id
// in r4 (fake-Fv ABI, same as UnkVirtualFunc6/7/9). extern "C" + explicit
// self/id params emits the exact Fv symbol (MWCC_CASES §3908). Walks the
// 8-entry array at self+0x1388 (stride 0x34); on an id match, clears the
// whole slot via a tail-call to memset (retail `b memset`).
extern "C" void CBattleState_UnkVirtualFunc12__Q22cf12CBattleStateFv(cf::CBattleState* self, u32 id) {
    if (id >= 0x12f) {
        return;
    }
    if (id == 0) {
        return;
    }

    cf::CBattleStateEntry* entry = (cf::CBattleStateEntry*)((u8*)self + 0x1388);
    for (u32 i = 0; i < 8; i++, entry++) {
        if (entry->unk0C == id) {
            memset(entry, 0, 0x34);
            return;
        }
    }
}
void* cf::CBattleState::CBattleState_UnkVirtualFunc13(int index) {
    return (char*)&((cf::CBattleStateEntry*)((u8*)this + 0x8))[index];
}
extern "C" void* CBattleState_UnkVirtualFunc14__Q22cf12CBattleStateFv(cf::CBattleState* self, unsigned long idx) {
    return (char*)&((cf::CBattleStateEntry*)((u8*)self + 0x8))[idx];
}
extern "C" void* CBattleState_UnkVirtualFunc15__Q22cf12CBattleStateFv(cf::CBattleState* self, unsigned long idx) {
    return (char*)&((cf::CBattleStateEntry*)((u8*)self + 0x688))[idx];
}
// Recovered extern "C" forms keep the retail Fv symbol (vfunc15 precedent).
extern "C" void* CBattleState_UnkVirtualFunc16__Q22cf12CBattleStateFv(void* self, int idx) {
    return (char*)self + idx * 0x34 + 0xd08;
}
// func_801490A0: r3 = self, r4 = id. Counts the 0x68 status slots at
// self+0x8 (stride 0x34) whose unk0C halfword equals id; ids >= 0x12f
// return 0 immediately. Retail keeps the count as a 13x8 unrolled
// mtctr/bdnz loop with a dead +7 trip counter.
int func_801490A0(cf::CBattleState* self, u32 id) {
    int count;

    if (id >= 0x12f) {
        return 0;
    }

    count = 0;
    {
        cf::CBattleStateEntry* p = (cf::CBattleStateEntry*)((u8*)self + 0x8);
        int i;

        for (i = 0; i < 0x68; i++, p++) {
            if (p->unk0C == id) {
                count++;
            }
        }
    }
    return count;
}

// Batch 2026-08: battlestate-idx-find owns func_80149154 / func_801491A4
// (byte-identical siblings). Bare retail symbols -> extern "C" free
// functions. The caller leaves the status id in r4; this scans the 0x68
// entry slots at self+8 (stride 0x34) and returns &slot whose unk0C id
// matches, else 0. Retail keeps a compact mtctr/bdnz loop with the return
// recomputed from the index (`mulli`/`add`/`addi`), so the body indexes
// base[i] rather than walking a pointer.
extern "C" cf::CBattleStateEntry* func_80149154(cf::CBattleState* self, unsigned int id) {
    if (id >= 0x12f) {
        return 0;
    }

    cf::CBattleStateEntryArray* v = (cf::CBattleStateEntryArray*)self;
    for (unsigned int i = 0; i < 0x68; i++) {
        if (v->entries[i].unk0C == id) {
            return &v->entries[i];
        }
    }
    return 0;
}
extern "C" cf::CBattleStateEntry* func_801491A4(cf::CBattleState* self, unsigned int id) {
    if (id >= 0x12f) {
        return 0;
    }

    cf::CBattleStateEntryArray* v = (cf::CBattleStateEntryArray*)self;
    for (unsigned int i = 0; i < 0x68; i++) {
        if (v->entries[i].unk0C == id) {
            return &v->entries[i];
        }
    }
    return 0;
}
// func_801491F4: r3 = self, r4 = id. Walks the 0x68 status slots at
// self+0x8 (stride 0x34) and returns the slot whose unk0C halfword equals
// id with the MAXIMUM unk10 (s32). ids >= 0x12f return 0 immediately;
// no match also returns 0. Retail keeps the scan as an mtctr/bdnz loop
// (26 groups x 4 entries) with the return recomputed from the running
// index (mulli/add/addi -- same convention as func_80149154).
extern "C" cf::CBattleStateEntry* func_801491F4(cf::CBattleState* self, unsigned int id) {
    int best;
    unsigned int i;

    if (id >= 0x12f) {
        return 0;
    }

    best = -1;
    cf::CBattleStateEntryArray* v = (cf::CBattleStateEntryArray*)self;
    for (i = 0; i < 0x68; i++) {
        if (v->entries[i].unk0C == id) {
            if (best >= 0) {
                if (v->entries[i].unk10 > v->entries[best].unk10) {
                    best = i;
                }
            } else {
                best = i;
            }
        }
    }

    if (best >= 0) {
        return &v->entries[best];
    }
    return 0;
}
// func_80149330: r3 = self, r4 = id, r5 = a, r6 = b, r7 = c. Scans the
// 0x68 status slots at self+0x8 (stride 0x34) for the first slot whose
// unk0C == id, unk00 == a, unk04 == b and (c == 0 || unk08 == c); returns
// &slot or 0. ids >= 0x12f return 0. Retail keeps an mtctr/bdnz loop
// (26 groups x 4 entries) with a running entry index recomputed via
// mulli/add/addi on success.
extern "C" cf::CBattleStateEntry* func_80149330(cf::CBattleState* self, unsigned int id, unsigned int a, unsigned int b, unsigned int c) {
    unsigned int i;

    if (id >= 0x12f) {
        return 0;
    }

    cf::CBattleStateEntryArray* v = (cf::CBattleStateEntryArray*)self;
    for (i = 0; i < 0x68; i++) {
        if (v->entries[i].unk0C == id && v->entries[i].unk00 == a && v->entries[i].unk04 == b && (c == 0 || v->entries[i].unk08 == c)) {
            return &v->entries[i];
        }
    }
    return 0;
}
extern "C" void CBattleState_UnkVirtualFunc30__Q22cf12CBattleStateFv(cf::CBattleState* self, u32 flags) {
    *(u16*)((u8*)self + 0x4) = (u16)(*(u16*)((u8*)self + 0x4) | flags);
}
extern "C" void CBattleState_UnkVirtualFunc32__Q22cf12CBattleStateFv(cf::CBattleState* self, u32 flags) {
    *(u16*)((u8*)self + 0x6) = (u16)(*(u16*)((u8*)self + 0x6) | flags);
}
int cf::CBattleState::CBattleState_UnkVirtualFunc3() { return (int)&lbl_eu_80662248; }
extern "C" int CBattleState_UnkVirtualFunc2__Q22cf12CBattleStateFv() { return 0; }

// Batch 2026-08: battlestate-vfunc4 owns CBattleState_UnkVirtualFunc4
// exclusively. Retail symbol mangles Fv but the caller leaves the status id
// in r4 (fake-Fv ABI). Builds a zeroed CBattleStateEntry on the stack with
// unk0C = id and unk30 bit 0 set, then dispatches through vt+0x18
// (UnkVirtualFunc5) to enter the new status.
extern "C" u8 func_80145BC4(int index) {
    // Low-byte of the bdat column value: retail truncates via stw/lbz (a
    // memory round-trip), which MWCC only emits for a union member read -
    // a plain (u8) cast compiles to rlwinm instead.
    union {
        u32 w;
        u8 b;
    } u;
    u.w = getBdatStringColumnValue(lbl_eu_806640E0, &lbl_eu_805018A8[0xf], index);
    return u.b;
}
extern "C" void CBattleState_UnkVirtualFunc4__Q22cf12CBattleStateFv(cf::CBattleState* self, u32 id) {
    if (id >= 0x12f) {
        return;
    }
    if (id == 0) {
        return;
    }

    cf::CBattleStateEntry entry;
    memset(&entry, 0, sizeof(entry));
    entry.unk0C = (u16)id;
    entry.unk30 |= 1;
    self->CBattleState_UnkVirtualFunc5(&entry);
}
// func_80146300: r3 = id, r4 = flag. When flag != 0 the three always-on ids
// 0xd5/0x107/0xdd return true. When flag == 0, ids in [0xd5, 0x107] are
// checked against a 64-bit bitmask: (1ULL << (id - 0xd5)) must land on bit
// 0, 8, 10, 11, 12 or 50 (mask 0x0004000000001D01 -- lo word andi 0x1D01,
// hi word bit 18 folded into retail's rlwimi r0,r3,0,13,13). Two separate
// result locals so the no-call path stays in r0 while the __shl2i path
// uses r31 (live across bl).
int func_80146300(u32 id, u32 flag) {
    if (flag != 0) {
        int r = 0;

        if (id == 0xd5 || id == 0x107 || id == 0xdd) {
            r = 1;
        }
        return r;
    } else {
        int r = 0;

        if (id - 0xd5 <= 0x32) {
            if ((1ULL << (id - 0xd5)) & 0x0004000000001D01ULL) {
                r = 1;
            }
        }
        return r;
    }
}

// Batch 2026-08: battlestate-vfunc9 owns CBattleState_UnkVirtualFunc9
// exclusively. Retail symbol mangles Fv but the caller leaves the slot
// index in r4 (fake-Fv ABI, same as UnkVirtualFunc7/12). Copies the
// 0x34-byte status slot at self + 0x8 + id*0x34 to a stack entry
// (13-word copy) and dispatches it through vt+0x24 (UnkVirtualFunc8).
// No bounds check on id in retail. extern "C" + explicit self/id params
// emits the exact Fv symbol (vfunc12 precedent). The retail copy is a raw
// 13-word block (lwz/stw pairs): struct assignment would emit per-field
// widths (lhz/lha) and memcpy would call out -- explicit u32-pair copies
// reproduce the word copy (same convention as vfunc8/10).
extern "C" void CBattleState_UnkVirtualFunc9__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id) {
    // Pure-u32 block view: a struct copy of this shape makes MWCC's block
    // copy backend emit the paired lwz/stw schedule with retail's r5/r0
    // allocation (a struct copy of CBattleStateEntry itself would emit
    // per-field-width lhz/lha copies instead).
    struct WordBlock {
        u32 w[13];
    };
    cf::CBattleStateEntry entry;
    WordBlock* dst = (WordBlock*)&entry;
    const WordBlock* src =
        (const WordBlock*)&((cf::CBattleStateEntryArray*)self)->entries[id];

    *dst = *src;
    self->CBattleState_UnkVirtualFunc8(&entry);
}

// Batch 2026-08: battlestate-vfunc7 owns CBattleState_UnkVirtualFunc7
// exclusively. Retail symbol mangles Fv but the caller leaves the status id
// in r4 (fake-Fv ABI, same as UnkVirtualFunc9/12). Guards: id >= 0x12f,
// id == 0, and func_80148778(self, id) == 0 all bail early. Otherwise
// builds a zeroed CBattleStateEntry (unk0C = id, unk30 |= 0x200,
// unk2E = id) and dispatches it through vt+0x24 (UnkVirtualFunc8).
extern "C" void CBattleState_UnkVirtualFunc7__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id) {
    cf::CBattleStateEntry entry;

    if (id >= 0x12f) {
        return;
    }
    if (id == 0) {
        return;
    }
    if (func_80148778(self, id) == 0) {
        return;
    }

    memset(&entry, 0, sizeof(entry));
    entry.unk0C = id;
    entry.unk30 |= 0x200;
    entry.unk2E = id;
    self->CBattleState_UnkVirtualFunc8(&entry);
}

// func_80148778: r3 = self, r4 = status id. Returns whether status `id` is
// active per the this+0x15AC bitfield (bit = id & 0x1F, word offset =
// (id >> 3) & ~3). ids < 0x12f test the bit directly (branchless boolify);
// ids >= 0x12f scan candidate statuses 1..0x12e for a match:
//   id == 0x131         -> kind(i) == 0 and bit i set
//   id == 0x132         -> kind(i) == 1 and bit i set
//   id in [0x133,0x138] -> bdat column value(i) low-byte - 4 == id - 0x139
//                          and bit i set
//   id in [0x139,0x13e] -> kind(i) == 0 and bdat match (id - 0x139)
//   id in [0x13f,0x144] -> kind(i) == 1 and bdat match (id - 0x13f)
//   else                -> 0
// (kind = the same 0/1/2/3 decision tree as func_80145F78, inlined once per
// loop; bdat column-value low-byte truncation is the stw/lbz union trick
// from func_80145BC4.)
int func_80148778(cf::CBattleState* self, u32 id) {
    int i;
    u32 one;

    if (id < 0x12f) {
        goto simple_test;
    }

    switch (id) {
    case 0x131:
        goto case131;
    case 0x132:
        goto case132;
    case 0x133:
    case 0x134:
    case 0x135:
    case 0x136:
    case 0x137:
    case 0x138:
        goto case133_138;
    case 0x139:
    case 0x13a:
    case 0x13b:
    case 0x13c:
    case 0x13d:
    case 0x13e:
        goto case139_13E;
    case 0x13f:
    case 0x140:
    case 0x141:
    case 0x142:
    case 0x143:
    case 0x144:
        goto case13F_144;
    default:
        goto ret0;
    }

case131:
    // kind(i) == 0 and bit i set. No call in this loop: retail keeps a
    // count-controlled mtctr/bdnz loop (i in r7, one in r6).
    one = 1;
    for (i = 1; i < 0x12f; i++) {
        int kind;
        u32 wordOff;
        u32 bitPos;

        if (i >= 0xd4)
            goto A5;
        if (i >= 0x3e)
            goto A4;
        if (i >= 0x2c)
            goto A3;
        if (i == 0x27)
            goto Akind0;
        if (i >= 0x27)
            goto A2;
        if (i >= 4)
            goto A1;
        if (i >= 2)
            goto Akind0;
        goto Akind2;
    A1:
        if (i >= 0x14)
            goto Akind2;
        goto Akind1;
    A2:
        if (i >= 0x2a)
            goto Akind1;
        goto Akind2;
    A3:
        if (i == 0x36)
            goto Akind0;
        if (i >= 0x36)
            goto A3b;
        if (i >= 0x35)
            goto Akind2;
        if (i >= 0x33)
            goto Akind0;
        goto Akind2;
    A3b:
        if (i >= 0x3c)
            goto Akind1;
        goto Akind2;
    A4:
        if (i == 0x5f)
            goto Akind0;
        if (i >= 0x5f)
            goto A4b;
        if (i >= 0x52)
            goto A4c;
        if (i >= 0x46)
            goto Akind2;
        if (i >= 0x44)
            goto Akind0;
        goto Akind2;
    A4c:
        if (i >= 0x5d)
            goto Akind2;
        if (i >= 0x58)
            goto Akind0;
        goto Akind1;
    A4b:
        if (i == 0x93)
            goto Akind0;
        if (i >= 0x93)
            goto A4d;
        if (i >= 0x6a)
            goto Akind2;
        if (i >= 0x65)
            goto Akind1;
        goto Akind2;
    A4d:
        if (i >= 0xce)
            goto Akind0;
        goto Akind2;
    A5:
        if (i >= 0x103)
            goto A5b;
        if (i == 0xeb)
            goto Akind2;
        if (i >= 0xeb)
            goto A5c;
        if (i >= 0xdf)
            goto A5d;
        if (i == 0xdc)
            goto Akind1;
        goto Akind2;
    A5d:
        if (i >= 0xea)
            goto Akind3;
        if (i >= 0xe3)
            goto Akind2;
        goto Akind1;
    A5c:
        if (i == 0xf7)
            goto Akind0;
        if (i >= 0xf7)
            goto A5e;
        if (i >= 0xed)
            goto Akind3;
        goto Akind0;
    A5e:
        if (i == 0xff)
            goto Akind2;
        goto Akind3;
    A5b:
        if (i == 0x117)
            goto Akind1;
        if (i >= 0x117)
            goto A5f;
        if (i >= 0x109)
            goto A5g;
        if (i == 0x106)
            goto Akind3;
        goto Akind2;
    A5g:
        if (i >= 0x111)
            goto Akind3;
        if (i >= 0x10d)
            goto Akind2;
        goto Akind3;
    A5f:
        if (i == 0x12d)
            goto Akind0;
        if (i >= 0x12d)
            goto Akind2;
        if (i == 0x11e)
            goto Akind0;
        goto Akind2;

    Akind0:
        kind = 0;
        goto Akind_done;
    Akind1:
        kind = 1;
        goto Akind_done;
    Akind3:
        kind = 3;
        goto Akind_done;
    Akind2:
        kind = 2;
    Akind_done:

        if (kind == 0) {
            wordOff = (i >> 3) & ~3u;
            bitPos = i & 0x1F;
            if ((one << bitPos) & *(u32*)((u8*)self + 0x15AC + wordOff)) {
                return 1;
            }
        }
    }
    goto ret0;

case132:
    one = 1;
    for (i = 1; i < 0x12f; i++) {
        int kind;
        u32 wordOff;
        u32 bitPos;

        if (i >= 0xd4)
            goto B5;
        if (i >= 0x3e)
            goto B4;
        if (i >= 0x2c)
            goto B3;
        if (i == 0x27)
            goto Bkind0;
        if (i >= 0x27)
            goto B2;
        if (i >= 4)
            goto B1;
        if (i >= 2)
            goto Bkind0;
        goto Bkind2;
    B1:
        if (i >= 0x14)
            goto Bkind2;
        goto Bkind1;
    B2:
        if (i >= 0x2a)
            goto Bkind1;
        goto Bkind2;
    B3:
        if (i == 0x36)
            goto Bkind0;
        if (i >= 0x36)
            goto B3b;
        if (i >= 0x35)
            goto Bkind2;
        if (i >= 0x33)
            goto Bkind0;
        goto Bkind2;
    B3b:
        if (i >= 0x3c)
            goto Bkind1;
        goto Bkind2;
    B4:
        if (i == 0x5f)
            goto Bkind0;
        if (i >= 0x5f)
            goto B4b;
        if (i >= 0x52)
            goto B4c;
        if (i >= 0x46)
            goto Bkind2;
        if (i >= 0x44)
            goto Bkind0;
        goto Bkind2;
    B4c:
        if (i >= 0x5d)
            goto Bkind2;
        if (i >= 0x58)
            goto Bkind0;
        goto Bkind1;
    B4b:
        if (i == 0x93)
            goto Bkind0;
        if (i >= 0x93)
            goto B4d;
        if (i >= 0x6a)
            goto Bkind2;
        if (i >= 0x65)
            goto Bkind1;
        goto Bkind2;
    B4d:
        if (i >= 0xce)
            goto Bkind0;
        goto Bkind2;
    B5:
        if (i >= 0x103)
            goto B5b;
        if (i == 0xeb)
            goto Bkind2;
        if (i >= 0xeb)
            goto B5c;
        if (i >= 0xdf)
            goto B5d;
        if (i == 0xdc)
            goto Bkind1;
        goto Bkind2;
    B5d:
        if (i >= 0xea)
            goto Bkind3;
        if (i >= 0xe3)
            goto Bkind2;
        goto Bkind1;
    B5c:
        if (i == 0xf7)
            goto Bkind0;
        if (i >= 0xf7)
            goto B5e;
        if (i >= 0xed)
            goto Bkind3;
        goto Bkind0;
    B5e:
        if (i == 0xff)
            goto Bkind2;
        goto Bkind3;
    B5b:
        if (i == 0x117)
            goto Bkind1;
        if (i >= 0x117)
            goto B5f;
        if (i >= 0x109)
            goto B5g;
        if (i == 0x106)
            goto Bkind3;
        goto Bkind2;
    B5g:
        if (i >= 0x111)
            goto Bkind3;
        if (i >= 0x10d)
            goto Bkind2;
        goto Bkind3;
    B5f:
        if (i == 0x12d)
            goto Bkind0;
        if (i >= 0x12d)
            goto Bkind2;
        if (i == 0x11e)
            goto Bkind0;
        goto Bkind2;

    Bkind0:
        kind = 0;
        goto Bkind_done;
    Bkind1:
        kind = 1;
        goto Bkind_done;
    Bkind3:
        kind = 3;
        goto Bkind_done;
    Bkind2:
        kind = 2;
    Bkind_done:

        if (kind == 1) {
            wordOff = (i >> 3) & ~3u;
            bitPos = i & 0x1F;
            if ((one << bitPos) & *(u32*)((u8*)self + 0x15AC + wordOff)) {
                return 1;
            }
        }
    }
    goto ret0;

case133_138:
    // No kind check: only the bdat column value match gates the bit test.
    // The id - 0x139 target is hoisted before the loop (retail r29).
    {
        u32 target = id - 0x139;
        one = 1;
        for (i = 1; i < 0x12f; i++) {
            union {
                u32 w;
                u8 b;
            } u;
            u32 wordOff;
            u32 bitPos;

            u.w = getBdatStringColumnValue(
                lbl_eu_806640E0, &lbl_eu_805018A8[0xf], i);
            if ((u32)target == (u32)(u.b - 4)) {
                wordOff = (i >> 3) & ~3u;
                bitPos = i & 0x1F;
                if ((one << bitPos) &
                    *(u32*)((u8*)self + 0x15AC + wordOff)) {
                    return 1;
                }
            }
        }
    }
    goto ret0;

case139_13E:
    one = 1;
    for (i = 1; i < 0x12f; i++) {
        int kind;
        union {
            u32 w;
            u8 b;
        } u;
        u32 wordOff;
        u32 bitPos;

        if (i >= 0xd4)
            goto C5;
        if (i >= 0x3e)
            goto C4;
        if (i >= 0x2c)
            goto C3;
        if (i == 0x27)
            goto Ckind0;
        if (i >= 0x27)
            goto C2;
        if (i >= 4)
            goto C1;
        if (i >= 2)
            goto Ckind0;
        goto Ckind2;
    C1:
        if (i >= 0x14)
            goto Ckind2;
        goto Ckind1;
    C2:
        if (i >= 0x2a)
            goto Ckind1;
        goto Ckind2;
    C3:
        if (i == 0x36)
            goto Ckind0;
        if (i >= 0x36)
            goto C3b;
        if (i >= 0x35)
            goto Ckind2;
        if (i >= 0x33)
            goto Ckind0;
        goto Ckind2;
    C3b:
        if (i >= 0x3c)
            goto Ckind1;
        goto Ckind2;
    C4:
        if (i == 0x5f)
            goto Ckind0;
        if (i >= 0x5f)
            goto C4b;
        if (i >= 0x52)
            goto C4c;
        if (i >= 0x46)
            goto Ckind2;
        if (i >= 0x44)
            goto Ckind0;
        goto Ckind2;
    C4c:
        if (i >= 0x5d)
            goto Ckind2;
        if (i >= 0x58)
            goto Ckind0;
        goto Ckind1;
    C4b:
        if (i == 0x93)
            goto Ckind0;
        if (i >= 0x93)
            goto C4d;
        if (i >= 0x6a)
            goto Ckind2;
        if (i >= 0x65)
            goto Ckind1;
        goto Ckind2;
    C4d:
        if (i >= 0xce)
            goto Ckind0;
        goto Ckind2;
    C5:
        if (i >= 0x103)
            goto C5b;
        if (i == 0xeb)
            goto Ckind2;
        if (i >= 0xeb)
            goto C5c;
        if (i >= 0xdf)
            goto C5d;
        if (i == 0xdc)
            goto Ckind1;
        goto Ckind2;
    C5d:
        if (i >= 0xea)
            goto Ckind3;
        if (i >= 0xe3)
            goto Ckind2;
        goto Ckind1;
    C5c:
        if (i == 0xf7)
            goto Ckind0;
        if (i >= 0xf7)
            goto C5e;
        if (i >= 0xed)
            goto Ckind3;
        goto Ckind0;
    C5e:
        if (i == 0xff)
            goto Ckind2;
        goto Ckind3;
    C5b:
        if (i == 0x117)
            goto Ckind1;
        if (i >= 0x117)
            goto C5f;
        if (i >= 0x109)
            goto C5g;
        if (i == 0x106)
            goto Ckind3;
        goto Ckind2;
    C5g:
        if (i >= 0x111)
            goto Ckind3;
        if (i >= 0x10d)
            goto Ckind2;
        goto Ckind3;
    C5f:
        if (i == 0x12d)
            goto Ckind0;
        if (i >= 0x12d)
            goto Ckind2;
        if (i == 0x11e)
            goto Ckind0;
        goto Ckind2;

    Ckind0:
        kind = 0;
        goto Ckind_done;
    Ckind1:
        kind = 1;
        goto Ckind_done;
    Ckind3:
        kind = 3;
        goto Ckind_done;
    Ckind2:
        kind = 2;
    Ckind_done:

        if (kind == 0) {
            u.w = getBdatStringColumnValue(
                lbl_eu_806640E0, &lbl_eu_805018A8[0xf], i);
            if ((u32)(id - 0x139) == (u32)(u.b - 4)) {
                wordOff = (i >> 3) & ~3u;
                bitPos = i & 0x1F;
                if ((one << bitPos) &
                    *(u32*)((u8*)self + 0x15AC + wordOff)) {
                    return 1;
                }
            }
        }
    }
    goto ret0;

case13F_144:
    one = 1;
    for (i = 1; i < 0x12f; i++) {
        int kind;
        union {
            u32 w;
            u8 b;
        } u;
        u32 wordOff;
        u32 bitPos;

        if (i >= 0xd4)
            goto D5;
        if (i >= 0x3e)
            goto D4;
        if (i >= 0x2c)
            goto D3;
        if (i == 0x27)
            goto Dkind0;
        if (i >= 0x27)
            goto D2;
        if (i >= 4)
            goto D1;
        if (i >= 2)
            goto Dkind0;
        goto Dkind2;
    D1:
        if (i >= 0x14)
            goto Dkind2;
        goto Dkind1;
    D2:
        if (i >= 0x2a)
            goto Dkind1;
        goto Dkind2;
    D3:
        if (i == 0x36)
            goto Dkind0;
        if (i >= 0x36)
            goto D3b;
        if (i >= 0x35)
            goto Dkind2;
        if (i >= 0x33)
            goto Dkind0;
        goto Dkind2;
    D3b:
        if (i >= 0x3c)
            goto Dkind1;
        goto Dkind2;
    D4:
        if (i == 0x5f)
            goto Dkind0;
        if (i >= 0x5f)
            goto D4b;
        if (i >= 0x52)
            goto D4c;
        if (i >= 0x46)
            goto Dkind2;
        if (i >= 0x44)
            goto Dkind0;
        goto Dkind2;
    D4c:
        if (i >= 0x5d)
            goto Dkind2;
        if (i >= 0x58)
            goto Dkind0;
        goto Dkind1;
    D4b:
        if (i == 0x93)
            goto Dkind0;
        if (i >= 0x93)
            goto D4d;
        if (i >= 0x6a)
            goto Dkind2;
        if (i >= 0x65)
            goto Dkind1;
        goto Dkind2;
    D4d:
        if (i >= 0xce)
            goto Dkind0;
        goto Dkind2;
    D5:
        if (i >= 0x103)
            goto D5b;
        if (i == 0xeb)
            goto Dkind2;
        if (i >= 0xeb)
            goto D5c;
        if (i >= 0xdf)
            goto D5d;
        if (i == 0xdc)
            goto Dkind1;
        goto Dkind2;
    D5d:
        if (i >= 0xea)
            goto Dkind3;
        if (i >= 0xe3)
            goto Dkind2;
        goto Dkind1;
    D5c:
        if (i == 0xf7)
            goto Dkind0;
        if (i >= 0xf7)
            goto D5e;
        if (i >= 0xed)
            goto Dkind3;
        goto Dkind0;
    D5e:
        if (i == 0xff)
            goto Dkind2;
        goto Dkind3;
    D5b:
        if (i == 0x117)
            goto Dkind1;
        if (i >= 0x117)
            goto D5f;
        if (i >= 0x109)
            goto D5g;
        if (i == 0x106)
            goto Dkind3;
        goto Dkind2;
    D5g:
        if (i >= 0x111)
            goto Dkind3;
        if (i >= 0x10d)
            goto Dkind2;
        goto Dkind3;
    D5f:
        if (i == 0x12d)
            goto Dkind0;
        if (i >= 0x12d)
            goto Dkind2;
        if (i == 0x11e)
            goto Dkind0;
        goto Dkind2;

    Dkind0:
        kind = 0;
        goto Dkind_done;
    Dkind1:
        kind = 1;
        goto Dkind_done;
    Dkind3:
        kind = 3;
        goto Dkind_done;
    Dkind2:
        kind = 2;
    Dkind_done:

        if (kind == 1) {
            u.w = getBdatStringColumnValue(
                lbl_eu_806640E0, &lbl_eu_805018A8[0xf], i);
            if ((u32)(id - 0x13f) == (u32)(u.b - 4)) {
                wordOff = (i >> 3) & ~3u;
                bitPos = i & 0x1F;
                if ((one << bitPos) &
                    *(u32*)((u8*)self + 0x15AC + wordOff)) {
                    return 1;
                }
            }
        }
    }
    /* falls through to ret0 (retail has no branch here) */

ret0:
    return 0;

simple_test:
    {
        u32 wordOff = (id >> 3) & ~3u;
        u32 bitPos = id & 0x1F;
        return ((1u << bitPos) &
                *(u32*)((u8*)self + 0x15AC + wordOff)) != 0;
    }
}
