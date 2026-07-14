#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CBattleState::CBattleState_UnkVirtualFunc26
// (retail @0x80148364, size 0x12C). Static match ~75%: prologue/stmw r25,
// allZero nest, memset+fill+vt+0x1C, and compact lwzu/stwu pair-copy all
// match semantically; remaining gap is MWCC fill-schedule hoist of the
// extrwi bit-test, r5 vs r12 vtable temp, and addic./bne vs mtctr/bdnz on
// the tail copy (see docs/MWCC_REFERENCE.md).
//
// symbols.txt mangles Fv; retail leaves the source table pointer in r4.

struct Entry {
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned short unk0C;
    int unk10;
    short unk14;
    short unk16;
    short unk18;
    short unk1A;
    float unk1C;
    float unk20;
    float unk24;
    float unk28;
    unsigned short unk2C;
    unsigned short unk2E;
    unsigned int unk30;
};

struct SrcEntry {
    unsigned short unk00;
    unsigned short unk02;
    unsigned short unk04;
    unsigned short unk06;
    short unk08;
    short unk0A;
    short unk0C;
    unsigned short unk0E;
};

// Raw layout mirrors retail offsets (host void* is 8 bytes — keep a
// 4-byte stand-in so this+0x152C stays at the retail byte offset).
struct State {
    unsigned int vtblSlot;        // 0x0
    unsigned short unk4;          // 0x4
    unsigned short unk6;          // 0x6
    unsigned char pad8[0x1520];   // 0x8 .. 0x1528
    unsigned char unk1528[4];     // 0x1528 .. 0x152C
    unsigned char unk152C[0x80];  // 0x152C .. 0x15AC
};

struct Snapshot {
    Entry called[8];
    int callCount;
    int vfunc29Calls;
    unsigned char copy[0x80];
};

static Entry g_called[8];
static int g_callCount;
static int g_vfunc29Calls;

static void stub_vfunc29(State* /*self*/) {
    g_vfunc29Calls++;
}

static void stub_vfunc6(State* /*self*/, Entry* entry) {
    if (g_callCount < 8) {
        g_called[g_callCount] = *entry;
    }
    g_callCount++;
}

typedef void (*Vfunc29Fn)(State*);
typedef void (*Vfunc6Fn)(State*, Entry*);

static bool entry_eq(const Entry& a, const Entry& b) {
    return a.unk00 == b.unk00 && a.unk04 == b.unk04 && a.unk08 == b.unk08 && a.unk0C == b.unk0C &&
           a.unk10 == b.unk10 && a.unk14 == b.unk14 && a.unk16 == b.unk16 && a.unk18 == b.unk18 &&
           a.unk1A == b.unk1A && a.unk1C == b.unk1C && a.unk20 == b.unk20 && a.unk24 == b.unk24 &&
           a.unk28 == b.unk28 && a.unk2C == b.unk2C && a.unk2E == b.unk2E && a.unk30 == b.unk30;
}

// Retail @0x80148364 — call vt+0x78 (Func29); for each of 8 SrcEntry
// records skip if unk00/02/04 are all zero, else memset a 0x34 Entry,
// fill fields (unk08=0x2000, or 0x4000 when unk0E bit15), call vt+0x1C
// (Func6); then 16× u64-pair copy from src into this+0x152C.
static void retail_vfunc26(State* self, const SrcEntry* src, Vfunc29Fn v29, Vfunc6Fn v6) {
    const SrcEntry* rec;
    const SrcEntry* recFlags;
    int n;

    v29(self);

    for (n = 0, rec = src, recFlags = src; n < 8; n++, rec++, recFlags++) {
        Entry entry;
        int allZero;

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

        std::memset(&entry, 0, sizeof(entry));
        entry.unk0C = rec->unk04;
        entry.unk30 |= 1u;
        entry.unk08 = 0x2000u;
        entry.unk10 = rec->unk08;
        entry.unk14 = rec->unk0A;
        entry.unk18 = rec->unk0C;
        entry.unk1A = static_cast<short>(rec->unk06);
        if ((((unsigned int)recFlags->unk0E >> 15) & 1u) != 0) {
            entry.unk08 = 0x4000u;
        }
        v6(self, &entry);
    }

    {
        unsigned int* dst =
            reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x1528);
        unsigned int* s = reinterpret_cast<unsigned int*>(const_cast<SrcEntry*>(src)) - 1;
        unsigned int i = 0x10;
        do {
            unsigned int a = *(s + 1);
            unsigned int b = *(s += 2);
            *(dst + 1) = a;
            *(dst += 2) = b;
        } while (--i);
    }
}

// Decomp — mirrors src/kyoshin/cf/object/CBattleState.cpp
// CBattleState_UnkVirtualFunc26 body (manual callbacks instead of vtable).
static void decomp_vfunc26(State* self, const SrcEntry* src, Vfunc29Fn v29, Vfunc6Fn v6) {
    const SrcEntry* rec;
    const SrcEntry* recFlags;
    int n;
    unsigned int flag2000;
    unsigned int flag4000;

    v29(self);

    flag2000 = 0x2000;
    flag4000 = 0x4000;

    for (n = 0, rec = src, recFlags = src; n < 8; n++, rec++, recFlags++) {
        Entry entry;
        unsigned int flags;
        unsigned short id;
        int allZero;

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

        std::memset(&entry, 0, sizeof(entry));
        id = rec->unk04;
        flags = entry.unk30;
        entry.unk0C = id;
        flags |= 1;
        entry.unk08 = flag2000;
        entry.unk10 = rec->unk08;
        entry.unk14 = rec->unk0A;
        entry.unk18 = rec->unk0C;
        entry.unk1A = static_cast<short>(rec->unk06);
        entry.unk30 = flags;
        if ((((unsigned int)recFlags->unk0E >> 15) & 1u) != 0) {
            entry.unk08 = flag4000;
        }
        v6(self, &entry);
    }

    {
        unsigned int* dst =
            reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x1528);
        unsigned int* s = reinterpret_cast<unsigned int*>(const_cast<SrcEntry*>(src)) - 1;
        unsigned int i = 0x10;
        do {
            unsigned int a = *(s + 1);
            unsigned int b = *(s += 2);
            *(dst + 1) = a;
            *(dst += 2) = b;
        } while (--i);
    }
}

static Snapshot run_impl(void (*impl)(State*, const SrcEntry*, Vfunc29Fn, Vfunc6Fn), State state,
                         const SrcEntry src[8]) {
    Snapshot snap = {};
    g_callCount = 0;
    g_vfunc29Calls = 0;
    std::memset(g_called, 0, sizeof(g_called));

    impl(&state, src, stub_vfunc29, stub_vfunc6);

    snap.callCount = g_callCount;
    snap.vfunc29Calls = g_vfunc29Calls;
    for (int i = 0; i < 8 && i < g_callCount; i++) {
        snap.called[i] = g_called[i];
    }
    std::memcpy(snap.copy, state.unk152C, 0x80);
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (a.callCount != b.callCount || a.vfunc29Calls != b.vfunc29Calls) {
        return false;
    }
    for (int i = 0; i < a.callCount && i < 8; i++) {
        if (!entry_eq(a.called[i], b.called[i])) {
            return false;
        }
    }
    return std::memcmp(a.copy, b.copy, 0x80) == 0;
}

static void clear_src(SrcEntry src[8]) {
    std::memset(src, 0, sizeof(SrcEntry) * 8);
}

static void set_src(SrcEntry* e, unsigned short a, unsigned short b, unsigned short id,
                    unsigned short u06, short s08, short s0a, short s0c, unsigned short u0e) {
    e->unk00 = a;
    e->unk02 = b;
    e->unk04 = id;
    e->unk06 = u06;
    e->unk08 = s08;
    e->unk0A = s0a;
    e->unk0C = s0c;
    e->unk0E = u0e;
}

static void run_scenario(const char* name, const SrcEntry srcIn[8]) {
    State state;
    SrcEntry src[8];

    std::memset(&state, 0, sizeof(state));
    std::memcpy(src, srcIn, sizeof(src));
    // Poison destination so a missed copy fails loudly.
    std::memset(state.unk152C, 0xA5, 0x80);

    Snapshot retail = run_impl(retail_vfunc26, state, src);
    Snapshot decomp = run_impl(decomp_vfunc26, state, src);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s (calls r=%d d=%d v29 r=%d d=%d)\n", name, retail.callCount,
                    decomp.callCount, retail.vfunc29Calls, decomp.vfunc29Calls);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    SrcEntry src[8];

    // 1. All-zero table: only vfunc29 + zero copy
    clear_src(src);
    run_scenario("all_zero_skip_all", src);

    // 2. Single first slot active
    clear_src(src);
    set_src(&src[0], 1, 0, 0x10, 5, 100, -2, 50, 0);
    run_scenario("first_slot_only", src);

    // 3. Single last slot active
    clear_src(src);
    set_src(&src[7], 0, 1, 0x20, 6, -7, 3, 9, 0);
    run_scenario("last_slot_only", src);

    // 4. Middle slot with bit15 → unk08=0x4000
    clear_src(src);
    set_src(&src[3], 0, 0, 0x33, 1, 10, 20, 30, 0x8000);
    run_scenario("mid_slot_bit15_4000", src);

    // 5. Middle slot without bit15 → unk08=0x2000
    clear_src(src);
    set_src(&src[3], 0, 0, 0x33, 1, 10, 20, 30, 0x7FFF);
    run_scenario("mid_slot_no_bit_2000", src);

    // 6. All 8 slots filled
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        set_src(&src[i], static_cast<unsigned short>(i + 1), 0,
                static_cast<unsigned short>(0x100 + i), static_cast<unsigned short>(i),
                static_cast<short>(i * 10), static_cast<short>(-i), static_cast<short>(i * 2),
                static_cast<unsigned short>((i & 1) ? 0x8000 : 0));
    }
    run_scenario("all_eight_active_alt_bits", src);

    // 7. Skip zeros interleaved with actives
    clear_src(src);
    set_src(&src[0], 1, 0, 1, 0, 1, 2, 3, 0);
    set_src(&src[2], 0, 0, 2, 0, 4, 5, 6, 0x8000);
    set_src(&src[5], 9, 8, 7, 6, 5, 4, 3, 0);
    run_scenario("sparse_0_2_5", src);

    // 8. unk00 nonzero alone is enough to not skip
    clear_src(src);
    set_src(&src[1], 1, 0, 0, 0, 0, 0, 0, 0);
    run_scenario("nonzero_unk00_only", src);

    // 9. unk02 nonzero alone
    clear_src(src);
    set_src(&src[1], 0, 1, 0, 0, 0, 0, 0, 0);
    run_scenario("nonzero_unk02_only", src);

    // 10. unk04 nonzero alone
    clear_src(src);
    set_src(&src[1], 0, 0, 1, 0, 0, 0, 0, 0);
    run_scenario("nonzero_unk04_only", src);

    // 11. Negative s16 fields
    clear_src(src);
    set_src(&src[0], 1, 1, 0xAB, 0xFFF0, -32768, -1, -100, 0);
    run_scenario("negative_signed_fields", src);

    // 12. Max positive s16 fields
    clear_src(src);
    set_src(&src[0], 1, 1, 0xCD, 0x7FFF, 32767, 32767, 32767, 0x8000);
    run_scenario("max_signed_fields_bit15", src);

    // 13. Copy contents differ across records (non-field path)
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        unsigned char* raw = reinterpret_cast<unsigned char*>(&src[i]);
        for (int b = 0; b < 16; b++) {
            raw[b] = static_cast<unsigned char>(0x40 + i * 16 + b);
        }
        // Ensure not all-zero skip for first three halfwords
        src[i].unk00 = static_cast<unsigned short>(0x100 + i);
    }
    run_scenario("raw_pattern_copy_and_calls", src);

    // 14. Only copy matters — already covered by all_zero, but with nonzero pad
    clear_src(src);
    {
        unsigned char* raw = reinterpret_cast<unsigned char*>(src);
        for (int i = 0; i < 0x80; i++) {
            raw[i] = static_cast<unsigned char>(i ^ 0x5A);
        }
        // Force every record to look empty so no vfunc6 calls (zero first 3 halfwords)
        for (int i = 0; i < 8; i++) {
            src[i].unk00 = 0;
            src[i].unk02 = 0;
            src[i].unk04 = 0;
        }
    }
    run_scenario("empty_ids_nonzero_tail_bytes_copy", src);

    // 15. bit15 only on last record
    clear_src(src);
    set_src(&src[7], 0, 0, 0x50, 0, 1, 2, 3, 0x8000);
    run_scenario("last_slot_bit15", src);

    // 16. bit15 on every record
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        set_src(&src[i], 1, 0, static_cast<unsigned short>(i + 1), 0, 0, 0, 0, 0x8000);
    }
    run_scenario("all_bit15", src);

    // 17. Mixed skip prefix
    clear_src(src);
    set_src(&src[4], 1, 2, 3, 4, 5, 6, 7, 0);
    set_src(&src[6], 8, 9, 10, 11, 12, 13, 14, 0x8000);
    run_scenario("prefix_skip_then_4_and_6", src);

    // 18. id=0 but unk00 set (not skipped)
    clear_src(src);
    set_src(&src[0], 1, 0, 0, 9, 8, 7, 6, 0);
    run_scenario("id_zero_but_unk00_set", src);

    // 19. Alternating skip
    clear_src(src);
    for (int i = 0; i < 8; i += 2) {
        set_src(&src[i], 1, 0, static_cast<unsigned short>(0x40 + i), 0, static_cast<short>(i), 0,
                0, 0);
    }
    run_scenario("alternating_even_slots", src);

    // 20. Alternating odd
    clear_src(src);
    for (int i = 1; i < 8; i += 2) {
        set_src(&src[i], 0, 1, static_cast<unsigned short>(0x50 + i), 0, 0, static_cast<short>(i),
                0, 0x8000);
    }
    run_scenario("alternating_odd_slots_bit15", src);

    // 21. unk06 high bit (not bit15 of unk0E)
    clear_src(src);
    set_src(&src[0], 1, 0, 0x60, 0x8000, 0, 0, 0, 0);
    run_scenario("unk06_high_does_not_set_4000", src);

    // 22. Multiple same ids
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        set_src(&src[i], 1, 0, 0x77, static_cast<unsigned short>(i), static_cast<short>(i), 0, 0,
                0);
    }
    run_scenario("repeated_same_id", src);

    // 23. Zero lower half of records, active upper — copy still full 0x80
    clear_src(src);
    for (int i = 4; i < 8; i++) {
        set_src(&src[i], 2, 3, static_cast<unsigned short>(0x80 + i), 1, -1, -2, -3, 0);
    }
    run_scenario("upper_half_active", src);

    // 24. Single byte pattern in copy via structured fields
    clear_src(src);
    set_src(&src[0], 0xFFFF, 0xEEEE, 0xDDDD, 0xCCCC, -1, -2, -3, 0xFFFF);
    run_scenario("ffff_first_record", src);

    // 25. Call count = 1 mid
    clear_src(src);
    set_src(&src[4], 0, 0, 0x90, 0, 0, 0, 0, 0);
    run_scenario("single_mid_call", src);

    // 26. Call count = 7 (skip one)
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        if (i == 3) {
            continue;
        }
        set_src(&src[i], 1, 0, static_cast<unsigned short>(0xA0 + i), 0, 0, 0, 0, 0);
    }
    run_scenario("seven_calls_skip_index_3", src);

    // 27. bit15 with otherwise empty signed fields
    clear_src(src);
    set_src(&src[2], 0, 0, 0xB0, 0, 0, 0, 0, 0x8000);
    run_scenario("bit15_zero_payload", src);

    // 28. Clamp-like large id
    clear_src(src);
    set_src(&src[0], 1, 1, 0xFFFF, 0, 0, 0, 0, 0);
    run_scenario("max_u16_id", src);

    // 29. Distinct copy with no calls (records empty but low bytes later)
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        src[i].unk06 = static_cast<unsigned short>(0x1111 * (i + 1));
        src[i].unk08 = static_cast<short>(i);
        src[i].unk0A = static_cast<short>(i * 2);
        src[i].unk0C = static_cast<short>(i * 3);
        src[i].unk0E = static_cast<unsigned short>(0x4000 + i);
    }
    run_scenario("no_calls_nonzero_trailing_half_copied", src);

    // 30. Stress: every field unique + alternating bits
    clear_src(src);
    for (int i = 0; i < 8; i++) {
        set_src(&src[i], static_cast<unsigned short>(0x10 + i), static_cast<unsigned short>(0x20 + i),
                static_cast<unsigned short>(0x30 + i), static_cast<unsigned short>(0x40 + i),
                static_cast<short>(100 + i), static_cast<short>(-100 - i),
                static_cast<short>(50 + i),
                static_cast<unsigned short>((i % 3 == 0) ? 0x8000 : 0x0001));
    }
    run_scenario("stress_unique_fields_mod3_bits", src);

    // 31. Only unk02+unk04 for skip boundary
    clear_src(src);
    set_src(&src[0], 0, 0, 0, 1, 2, 3, 4, 5); // fully skippable (00/02/04 zero)
    set_src(&src[1], 0, 9, 8, 7, 6, 5, 4, 0);
    run_scenario("skip_then_unk02_active", src);

    // 32. Two consecutive actives with opposite bits
    clear_src(src);
    set_src(&src[0], 1, 0, 1, 0, 1, 1, 1, 0);
    set_src(&src[1], 1, 0, 2, 0, 2, 2, 2, 0x8000);
    run_scenario("pair_opposite_bits", src);

    return 0;
}
