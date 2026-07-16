#include "kyoshin/cf/object/CBattleState.hpp"

extern "C" void* memset(void* dest, int val, size_t count);
extern "C" void* memcpy(void* dest, const void* src, size_t count);

// sdata2 float constants used by CBattleState_UnkVirtualFunc5
extern "C" const float lbl_eu_80667400;  // 0x80146E48: id==0x35 unk24
extern "C" const float lbl_eu_80667404;  // 0x80146E90: unk20 *= float
extern "C" const double lbl_eu_80667408; // 0x80146EC4: unk20 *= double
extern "C" const float lbl_eu_80667410;  // 0x80146ED4: compared with unk20
extern "C" const float lbl_eu_80667414;  // 0x80148134: 0.9f scaling (same as vfunc6)

extern "C" void func_80109784(void* ptr, u32 id, int arg);
extern "C" void func_8013DB6C(int a, u32 id, int b, int c);

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
// (13×8 halfword scan) the this+0x15AC status bit stays; otherwise cleared.
// Ids >= 0x12f always clear (skip the scan). Ends with memset(+0x152C,0,0x80).
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
            int j;
            u8* scan;

            found = 0;
            // The retail 13x8 listing is MWCC's unroll of this linear scan.
            scan = (u8*)this;
            for (j = 0; j < 0x68; scan += sizeof(CBattleStateEntry), j++) {
                if (id == *(u16*)(scan + 0x14)) {
                    found = 1;
                    goto scan_done;
                }
            }
            found = 0;
        scan_done:
            ;
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
// pattern as cf::CAIAction's UnkVirtualFunc1/2 — see
// docs/MWCC_REFERENCE.md).
//
// sdata2 float pool constant read via lbl_eu_80667414@sda21 (0.9f).
extern "C" const float lbl_eu_80667414;
extern "C" void CBattleState_UnkVirtualFunc6__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc17Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    cf::CBattleStateEntry* entries;
    cf::CBattleStateEntry* p;
    int n;

    // Bit `arg->unk0C` set into the this+0x15AC bitfield (word-aligned byte
    // offset = (id >> 3) & ~3, bit position = id & 0x1F).
    *(u32*)(self->unk15AC + (((u32)arg->unk0C >> 3) & ~3u)) |=
        1u << (arg->unk0C & 0x1F);

    entries = (cf::CBattleStateEntry*)((u8*)self + 0x1388);
    p = entries;
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
            return ((Vfunc17Fn*)*(void**)self)[18](self, p);
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
            ((Vfunc17Fn)(*(void***)self)[18])(self, entries);
            return;
        }
    }
}

// symbols.txt mangles Fv, but retail leaves the id in r4. This lookup reads
// the independent halfword state at +0x6.
extern "C" int CBattleState_UnkVirtualFunc33__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id) {
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

    return (self->unk6 & mask) != 0;
}

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
extern "C" void CBattleState_UnkVirtualFunc11__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 mask) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    int i;
    cf::CBattleStateEntry* entry;

    entry = (cf::CBattleStateEntry*)((u8*)self + 0x8);
    for (i = 0; i < 0x68; i++, entry++) {
        u32 id;
        int stillActive;

        if ((entry->unk30 & mask) == 0) {
            continue;
        }

        ((Vfunc18Fn)(*(void***)self)[19])(self, entry);
        id = entry->unk0C;
        memset(entry, 0, 0x34);

        if (id >= 0x12f) {
            stillActive = 0;
        } else {
            // Soft-cap (CODE_MATCH ~96.2%): retail keeps a dead trip counter in
            // r3 (li 0 / addi +7 / unused after bdnz) alongside found in r0 and
            // scan base in r4. MWCC coalesces found into r3 and DSE's any dead
            // trip (`trip&0`, `trip^trip`, comma) or blows the shape with
            // volatile / `trip>1000` boolify. Instruction shapes, layout, and
            // the rlwinm bit clear otherwise match — remaining gap is Chaitin
            // register coloring only (see MWCC_REFERENCE).
            u8* p = (u8*)self;
            int g;

            stillActive = 0;
            for (g = 13; g != 0; g--) {
                if (id == *(u16*)(p + 0x14)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x48)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x7c)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0xb0)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0xe4)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x118)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x14c)) { stillActive = 1; break; }
                if (id == *(u16*)(p + 0x180)) { stillActive = 1; break; }
                p += 0x1a0;
            }
        }

        if (!stillActive) {
            u8* wordPtr = self->unk15AC + ((id >> 3) & ~3u);
            *(u32*)wordPtr &= ~(1u << (id & 0x1F));
        }
    }
}

// Batch 2026-07-14h: battlestate-vfunc31 owns CBattleState_UnkVirtualFunc31
// exclusively. Do not touch ctor / UnkVirtualFunc6 / other vfuncs above.
//
// symbols.txt mangles Fv, but retail leaves the id in r4 (same fake-Fv ABI
// as UnkVirtualFunc6 — see docs/MWCC_REFERENCE.md).
//
// Leaf / no stack frame: maps specific ids to single-bit masks (or 0 for
// unmapped ids), then returns (self->unk4 & mask) != 0 via the standard
// MWCC branchless neg/or/srwi boolify idiom (see MWCC_REFERENCE §8c9).
extern "C" int CBattleState_UnkVirtualFunc31__Q22cf12CBattleStateFv(
    cf::CBattleState* self, u32 id) {
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

    return (self->unk4 & mask) != 0;
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

extern "C" void CBattleState_UnkVirtualFunc26__Q22cf12CBattleStateFv(
    cf::CBattleState* self, const cf::CBattleStateSrcEntry* src) {
    typedef void (*Vfunc6Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    const cf::CBattleStateSrcEntry* rec;
    const cf::CBattleStateSrcEntry* recFlags;
    int n;
    int allZero;
    u32 flag2000;
    u32 flag4000;

    self->CBattleState_UnkVirtualFunc29();

    flag2000 = 0x2000;
    flag4000 = 0x4000;

    for (n = 0, rec = src, recFlags = src; n < 8; n++, rec++, recFlags++) {
        cf::CBattleStateEntry entry;
        u32 flags;
        u16 id;

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
        id = rec->unk04;
        flags = entry.unk30;
        entry.unk0C = id;
        flags |= 1;
        entry.unk08 = flag2000;
        entry.unk10 = rec->unk08;
        entry.unk14 = rec->unk0A;
        entry.unk18 = rec->unk0C;
        entry.unk1A = (s16)rec->unk06;
        entry.unk30 = flags;
        // u32 >> 15 & 1 → extrwi (rlwinm. …,17,31,31); u16 >> yields srawi
        if ((((u32)recFlags->unk0E >> 15) & 1) != 0) {
            entry.unk08 = flag4000;
        }

        ((Vfunc6Fn*)*(void**)self)[7](self, &entry);
    }

    // Retail: mtctr/bdnz + lwzu/stwu from this+0x1528 / src-4. do-while(--i)
    // stays compact (for!=0 fully unrolls to 0x208); addic./bne soft-cap.
    {
        u32* dst = (u32*)((u8*)self + 0x1528);
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
// id (+ optional field eq unless unk30 bit 0x200) → stack-copy/clear →
// vt+0x2C → id-dup scan / clear unk15AC bit → vt+0x4C; stop early if
// arg->unk0C == 0.
extern "C" void CBattleState_UnkVirtualFunc8__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* entry) {
    typedef void (*Vfunc10Fn)(cf::CBattleState*, cf::CBattleStateEntry*);
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    // Hoist one/thirteen so Chaitin parks them in r26/r27 and leaves
    // this/entry in r28/r29 matching retail's early mr pair.
    int one;
    int thirteen;
    int id;
    int kind;

    one = 1;
    thirteen = 0xd;
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
        *(u32*)self->unk1528 = 0;
    }

    ((Vfunc10Fn)(*(void***)self)[11])(self, entry);

    {
        cf::CBattleStateEntry* slot;
        int i;

        // Retail init order after the first vt+0x2C call:
        //   slot=this+8, i=0  (one/thirteen already live in r26/r27)
        slot = (cf::CBattleStateEntry*)((u8*)self + 0x8);
        i = 0;

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
            u8* p;

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

            ((Vfunc10Fn)(*(void***)self)[11])(
                self, (cf::CBattleStateEntry*)savedWords);

            // Load halfword id into a wide local first (retail lhz → r5).
            savedId = *(u16*)((u8*)savedWords + 0xc);
            if (savedId >= 0x12f) {
                stillActive = 0;
            } else {
                // Retail: found in r0, dead trip in r3 (li 0 / addi +7 /
                // unused after bdnz), scan base in r4. Keep trip live via
                // a zeroing OR so MWCC cannot DSE the addi+7.
                p = (u8*)self;
                trip = 0;
                stillActive = 0;
                for (g = thirteen; g != 0; g--) {
                    if (savedId == *(u16*)(p + 0x14)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x48)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x7c)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0xb0)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0xe4)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x118)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x14c)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    if (savedId == *(u16*)(p + 0x180)) {
                        stillActive = 1;
                        goto scan_done;
                    }
                    p += 0x1a0;
                    trip += 7;
                }
                stillActive = 0;
            scan_done:
                stillActive |= trip & 0;
            }

            if (stillActive == 0) {
                u8* wordPtr = self->unk15AC + ((savedId >> 3) & ~3u);
                *(u32*)wordPtr &= ~(one << (savedId & 0x1F));
            }

            ((Vfunc18Fn)(*(void***)self)[19])(
                self, (cf::CBattleStateEntry*)savedWords);

            if (entry->unk0C == 0) {
                break;
            }
        }
    }
}

// Batch 2026-07-14k: battlestate-vfunc10 owns CBattleState_UnkVirtualFunc10
// exclusively. Do not touch ctor / other vfuncs.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc8). Early-out when arg->unk2E == 0. Walks
// this+0x8 (0x68 × stride 0x34): match on slot->unk2E (== arg->unk2E);
// when arg->unk30 bit 0x200 is clear also require unk00/04/08 eq. Same
// flat if+goto kind tree on the *slot* id as UnkVirtualFunc8; kind==3
// clears this+0x1528. Stack-copy + memset slot, id-dup scan / clear
// unk15AC bit, then vt+0x4C (UnkVirtualFunc18). No recursive vt+0x2C;
// walks all slots (no early break).
extern "C" void CBattleState_UnkVirtualFunc10__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    int one;
    int thirteen;
    cf::CBattleStateEntry* slot;
    int i;
    // Full arg spill (retail sp+0x3c..0x6c). Do not keep stackedWords[0] in a
    // separate local — retail reloads it from the frame on the optional-eq
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
    slot = (cf::CBattleStateEntry*)((u8*)self + 0x8);
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
        u8* p;

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
            *(u32*)self->unk1528 = 0;
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

        // Load halfword id into a wide local first (retail lhz → r5).
        savedId = *(u16*)((u8*)savedWords + 0xc);
        if (savedId >= 0x12f) {
            stillActive = 0;
        } else {
            // Retail: found in r0, dead trip in r3 (li 0 / addi +7 /
            // unused after bdnz), scan base in r4.
            p = (u8*)self;
            trip = 0;
            stillActive = 0;
            for (g = thirteen; g != 0; g--) {
                if (savedId == *(u16*)(p + 0x14)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x48)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x7c)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0xb0)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0xe4)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x118)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x14c)) {
                    stillActive = 1;
                    goto scan_done;
                }
                if (savedId == *(u16*)(p + 0x180)) {
                    stillActive = 1;
                    goto scan_done;
                }
                p += 0x1a0;
                trip += 7;
            }
            stillActive = 0;
        scan_done:
            stillActive |= trip & 0;
        }

        if (stillActive == 0) {
            u8* wordPtr = self->unk15AC + ((savedId >> 3) & ~3u);
            *(u32*)wordPtr &= ~(one << (savedId & 0x1F));
        }

        ((Vfunc18Fn)(*(void***)self)[19])(
            self, (cf::CBattleStateEntry*)savedWords);
    }
}

// Batch 2026-07-16: battlestate-vfunc5 owns CBattleState_UnkVirtualFunc5
// exclusively. Do not touch ctor / other vfuncs.
//
// symbols.txt mangles Fv, but retail leaves the entry arg in r4 (same
// fake-Fv ABI as UnkVirtualFunc6). Core battle-state-machine: id-specific
// init, kind-based routing through vfunc1/2 helpers + sound/event dispatch,
// then slot scan + copy/accumulate for entries sharing the same id/keys.
extern "C" void CBattleState_UnkVirtualFunc5__Q22cf12CBattleStateFv(
    cf::CBattleState* self, cf::CBattleStateEntry* arg) {
    typedef void (*Vfunc18Fn)(cf::CBattleState*, cf::CBattleStateEntry*);

    u32 id;
    int kind2;

    id = arg->unk0C;

    // ── Phase 1: id-specific init (flat if/goto to match retail's
    //   cmpwi/beq/bge chain) ─────────────────────────────────────────
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
        obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
        if (*(u32*)((u8*)obj + 0x3374) & 0x20) {
            arg->unk20 *= lbl_eu_80667404;
        } else {
            obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
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

    // ── Phase 2: set unk30 bit 0 based on unk20 vs constant ─────────
    if (lbl_eu_80667410 == arg->unk20) {
        arg->unk30 |= 1;
    } else {
        arg->unk30 &= ~1u;
    }

    // ── Phase 3: kind classification #1 (r0 in retail) ──────────────
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
            *(u32*)self->unk1528 = id;
        }
    }

    // ── Phase 4: set bitfield at this+0x15AC ────────────────────────
    {
        u32 wordOff = (id >> 3) & ~3u;
        u32 bitPos = id & 0x1F;
        *(u32*)(self->unk15AC + wordOff) |= (1u << bitPos);
    }

    // ── Phase 5: kind classification #2 (r31 in retail) ─────────────
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

    // ── Phase 6: choose Branch A or B based on arg->unk08 ───────────
    if (arg->unk08 == 0x2000 || arg->unk08 == 0x4000 || arg->unk08 == 0x8000) {
        if (!(arg->unk30 & 0x200)) {
            goto BranchA;
        }
    }

BranchB:
    // ── Branch B (retail jumps to 0x801476AC) ───────────────────────
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
                    obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
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
                    obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
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
                obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
                func_80109784((u8*)obj + 0x3F10, id, 1);
                func_8013DB6C(6, id, 0, 0);
            }
        }
    }

    goto after_dispatch;

BranchA:
    // ── Branch A: kind #3 → if kind==0, call func_80109784(ptr, id, 5) ──
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
            cf::CBattleState* obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
            func_80109784((u8*)obj + 0x3F10, id, 5);
        }
    }

    // ── Branch A: kind #4 → func_80109784 with 6 or 4 ──────────────
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
            cf::CBattleState* obj = ((cf::CBattleState* (*)(cf::CBattleState*))(*(void***)self)[1])(self);
            if (k == 1) {
                func_80109784((u8*)obj + 0x3F10, id, 6);
            } else {
                func_80109784((u8*)obj + 0x3F10, id, 4);
            }
        }
    }

    func_8013DB6C(6, id, 0, 0);

after_dispatch:
    // ── Final: entry-slot scan + copy ───────────────────────────────
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

            slot = (cf::CBattleStateEntry*)((u8*)self + 0x8);
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
                    ((Vfunc18Fn)(*(void***)self)[18])(self, slot);
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
                    ((Vfunc18Fn)(*(void***)self)[18])(self, slot);
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
                    ((Vfunc18Fn)(*(void***)self)[18])(self, slot);
                    goto F_slot_done;
                }

            F_next:
                ;
            }
        }

        // 3-array empty-slot scan
        {
            u8* base1 = (u8*)self + 0x8;
            u8* base2 = (u8*)self + 0x688;
            u8* base3 = (u8*)self + 0xD08;
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
                    ((Vfunc18Fn)(*(void***)self)[18])(self, dst);
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
