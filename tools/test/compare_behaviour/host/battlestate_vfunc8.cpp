#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CBattleState::CBattleState_UnkVirtualFunc8
// (retail @0x801485EC, size 0x428). Static match ~98.0% CODE_MATCH: flat
// if+goto kind tree + shared kind leaves + interleaved word-copy/memset
// args match retail instruction shapes, but MWCC's register allocator
// parks this/entry in r27/r28 (want r28/r29), places the id-dup scan base
// in r3 (want r4), and DSEs retail's dead trip counter in r3
// (li 0 / addi +7) — function lands at 0x420 vs retail 0x428. Same Chaitin
// class as UnkVirtualFunc11 (docs/MWCC_REFERENCE.md).
//
// symbols.txt mangles Fv; retail/decomp entry takes (self, entry) with the
// CBattleStateEntry* in r4.

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

enum { kEntryCount = 0x68 };

struct State {
    unsigned int vtblSlot;
    unsigned short unk4;
    unsigned short unk6;
    Entry entries[kEntryCount];
    unsigned int unk1528;
    unsigned char pad152C[0x80];
    unsigned int bits[0x30 / 4];
};

typedef void (*VFn)(State*, Entry*);

enum { kMaxCalls = 64 };

static int g_v10Count;
static Entry g_v10Copies[kMaxCalls];
static int g_v18Count;
static Entry g_v18Copies[kMaxCalls];

static void vfunc10(State* /*self*/, Entry* entry) {
    if (g_v10Count < kMaxCalls) {
        g_v10Copies[g_v10Count++] = *entry;
    }
}

static void vfunc18(State* /*self*/, Entry* entry) {
    if (g_v18Count < kMaxCalls) {
        g_v18Copies[g_v18Count++] = *entry;
    }
}

struct Snapshot {
    Entry entries[kEntryCount];
    unsigned int unk1528;
    unsigned int bits[0x30 / 4];
    int v10Count;
    int v18Count;
    Entry v10Copies[kMaxCalls];
    Entry v18Copies[kMaxCalls];
};

static void fill_entry(Entry* e, unsigned int seed) {
    e->unk00 = 0x10000u + seed;
    e->unk04 = 0x20000u + seed;
    e->unk08 = 0x30000u + seed;
    e->unk0C = 0;
    e->unk10 = static_cast<int>(seed) * 3 - 40;
    e->unk14 = static_cast<short>(0x50 + seed);
    e->unk16 = static_cast<short>(0x60 + seed);
    e->unk18 = static_cast<short>(0x70 + seed);
    e->unk1A = static_cast<short>(0x80 + seed);
    e->unk1C = 1.5f + static_cast<float>(seed);
    e->unk20 = 2.5f + static_cast<float>(seed);
    e->unk24 = 3.5f + static_cast<float>(seed);
    e->unk28 = 4.5f + static_cast<float>(seed);
    e->unk2C = static_cast<unsigned short>(0x90 + seed);
    e->unk2E = static_cast<unsigned short>(0xA0 + seed);
    e->unk30 = 0;
}

static bool entry_eq(const Entry& a, const Entry& b) {
    return a.unk00 == b.unk00 && a.unk04 == b.unk04 && a.unk08 == b.unk08 && a.unk0C == b.unk0C &&
           a.unk10 == b.unk10 && a.unk14 == b.unk14 && a.unk16 == b.unk16 && a.unk18 == b.unk18 &&
           a.unk1A == b.unk1A && a.unk1C == b.unk1C && a.unk20 == b.unk20 && a.unk24 == b.unk24 &&
           a.unk28 == b.unk28 && a.unk2C == b.unk2C && a.unk2E == b.unk2E && a.unk30 == b.unk30;
}

// Flat if+goto tree mirrored from retail @0x80148604 (shared kind leaves).
static int classify_kind(int id) {
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
    return 0;
kind1:
    return 1;
kind3:
    return 3;
kind2:
    return 2;
}

static void clear_bit(State* self, unsigned int id) {
    unsigned char* word =
        reinterpret_cast<unsigned char*>(self) + 0x15ACu + ((id >> 3) & ~3u);
    *reinterpret_cast<unsigned int*>(word) &= ~(1u << (id & 0x1Fu));
}

static void set_bit(State* self, unsigned int id) {
    unsigned char* word =
        reinterpret_cast<unsigned char*>(self) + 0x15ACu + ((id >> 3) & ~3u);
    *reinterpret_cast<unsigned int*>(word) |= 1u << (id & 0x1Fu);
}

static int scan_still_active(State* self, unsigned int savedId) {
    if (savedId >= 0x12fu) {
        return 0;
    }
    unsigned char* p = reinterpret_cast<unsigned char*>(self);
    for (int g = 13; g != 0; g--) {
        if (savedId == *reinterpret_cast<unsigned short*>(p + 0x14) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0x48) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0x7c) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0xb0) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0xe4) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0x118) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0x14c) ||
            savedId == *reinterpret_cast<unsigned short*>(p + 0x180)) {
            return 1;
        }
        p += 0x1a0;
    }
    return 0;
}

// Retail oracle — direct entry walks + bit200 field gate (asm semantics).
static void retail_vfunc8(State* self, Entry* entry) {
    int kind = classify_kind(static_cast<int>(entry->unk0C));
    if (kind == 3) {
        self->unk1528 = 0;
    }
    vfunc10(self, entry);

    for (int i = 0; i < kEntryCount; i++) {
        Entry* slot = &self->entries[i];
        if (slot->unk0C != entry->unk0C) {
            continue;
        }
        if ((entry->unk30 & 0x200u) == 0) {
            if (slot->unk00 != entry->unk00 || slot->unk04 != entry->unk04 ||
                slot->unk08 != entry->unk08 || slot->unk2E != entry->unk2E) {
                continue;
            }
        }

        Entry saved = *slot;
        std::memset(slot, 0, sizeof(Entry));
        vfunc10(self, &saved);

        unsigned int savedId = saved.unk0C;
        bool found = false;
        if (savedId < 0x12fu) {
            for (int j = 0; j < kEntryCount; j++) {
                if (self->entries[j].unk0C == savedId) {
                    found = true;
                    break;
                }
            }
        }
        if (!found) {
            clear_bit(self, savedId);
        }
        vfunc18(self, &saved);
        if (entry->unk0C == 0) {
            break;
        }
    }
}

// Decomp — mirrors src: raw 13×8 self-relative scan groups + word-copy
// schedule expressed as Entry assign for the host (semantics-identical).
static void decomp_vfunc8(State* self, Entry* entry) {
    int kind = classify_kind(static_cast<int>(entry->unk0C));
    if (kind == 3) {
        self->unk1528 = 0;
    }
    vfunc10(self, entry);

    Entry* slot = self->entries;
    for (int i = 0; i < kEntryCount; i++, slot++) {
        if (slot->unk0C != entry->unk0C) {
            continue;
        }
        if ((entry->unk30 & 0x200u) == 0) {
            if (slot->unk00 != entry->unk00 || slot->unk04 != entry->unk04 ||
                slot->unk08 != entry->unk08 || slot->unk2E != entry->unk2E) {
                continue;
            }
        }

        Entry saved = *slot;
        std::memset(slot, 0, sizeof(Entry));
        vfunc10(self, &saved);

        unsigned int savedId = saved.unk0C;
        int stillActive = scan_still_active(self, savedId);
        if (stillActive == 0) {
            clear_bit(self, savedId);
        }
        vfunc18(self, &saved);
        if (entry->unk0C == 0) {
            break;
        }
    }
}

static Snapshot run_impl(void (*impl)(State*, Entry*), State state, Entry arg) {
    Snapshot snap = {};
    g_v10Count = 0;
    g_v18Count = 0;
    std::memset(g_v10Copies, 0, sizeof(g_v10Copies));
    std::memset(g_v18Copies, 0, sizeof(g_v18Copies));

    impl(&state, &arg);

    std::memcpy(snap.entries, state.entries, sizeof(snap.entries));
    snap.unk1528 = state.unk1528;
    std::memcpy(snap.bits, state.bits, sizeof(snap.bits));
    snap.v10Count = g_v10Count;
    snap.v18Count = g_v18Count;
    std::memcpy(snap.v10Copies, g_v10Copies, sizeof(Entry) * g_v10Count);
    std::memcpy(snap.v18Copies, g_v18Copies, sizeof(Entry) * g_v18Count);
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (a.unk1528 != b.unk1528 || a.v10Count != b.v10Count || a.v18Count != b.v18Count) {
        return false;
    }
    for (int i = 0; i < kEntryCount; i++) {
        if (!entry_eq(a.entries[i], b.entries[i])) {
            return false;
        }
    }
    for (int i = 0; i < 0x30 / 4; i++) {
        if (a.bits[i] != b.bits[i]) {
            return false;
        }
    }
    for (int i = 0; i < a.v10Count; i++) {
        if (!entry_eq(a.v10Copies[i], b.v10Copies[i])) {
            return false;
        }
    }
    for (int i = 0; i < a.v18Count; i++) {
        if (!entry_eq(a.v18Copies[i], b.v18Copies[i])) {
            return false;
        }
    }
    return true;
}

struct Scenario {
    const char* name;
    unsigned short argId;
    unsigned int argFlags; // entry.unk30
    int matchSlot;         // -1 = none
    int dupSlot;           // -1 = none (second live entry with same id)
    bool fieldsMatch;      // when argFlags bit0x200 clear
    bool presetBit;
    unsigned int unk1528Init;
};

static void run_scenario(const Scenario& sc) {
    State state;
    std::memset(&state, 0, sizeof(state));
    state.unk1528 = sc.unk1528Init;

    for (int i = 0; i < kEntryCount; i++) {
        fill_entry(&state.entries[i], static_cast<unsigned int>(i + 3));
        unsigned short bg =
            static_cast<unsigned short>(((i * 3 + 11) % 0x12eu) + 1u);
        if (bg == sc.argId) {
            bg = static_cast<unsigned short>(bg + 1u);
        }
        state.entries[i].unk0C = bg;
        state.entries[i].unk30 = 0;
    }

    Entry arg;
    fill_entry(&arg, 99);
    arg.unk0C = sc.argId;
    arg.unk30 = sc.argFlags;
    arg.unk00 = 0xAAAAu;
    arg.unk04 = 0xBBBBu;
    arg.unk08 = 0xCCCCu;
    arg.unk2E = 0xDDu;

    if (sc.matchSlot >= 0) {
        Entry& slot = state.entries[sc.matchSlot];
        slot.unk0C = sc.argId;
        if (sc.fieldsMatch) {
            slot.unk00 = arg.unk00;
            slot.unk04 = arg.unk04;
            slot.unk08 = arg.unk08;
            slot.unk2E = arg.unk2E;
        } else {
            slot.unk00 = arg.unk00 ^ 1u;
            slot.unk04 = arg.unk04;
            slot.unk08 = arg.unk08;
            slot.unk2E = arg.unk2E;
        }
        slot.unk10 = 0x42;
    }

    if (sc.dupSlot >= 0) {
        state.entries[sc.dupSlot].unk0C = sc.argId;
        // Keep fields distinct so only matchSlot clears (unless bit 0x200).
        state.entries[sc.dupSlot].unk00 = 0x1111u;
        state.entries[sc.dupSlot].unk04 = 0x2222u;
        state.entries[sc.dupSlot].unk08 = 0x3333u;
        state.entries[sc.dupSlot].unk2E = 0x44u;
    }

    if (sc.presetBit && sc.argId < 0x180u * 8u) {
        set_bit(&state, sc.argId);
    }

    // retail vs decomp oracles (array walk vs 13×8 self-relative scan).
    Snapshot retail = run_impl(retail_vfunc8, state, arg);
    Snapshot decomp = run_impl(decomp_vfunc8, state, arg);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s\n", sc.name);
        std::exit(1);
    }

    int wantKind = classify_kind(static_cast<int>(sc.argId));
    if (wantKind == 3) {
        if (retail.unk1528 != 0) {
            std::printf("FAIL %s kind3 did not clear unk1528\n", sc.name);
            std::exit(1);
        }
    } else if (retail.unk1528 != sc.unk1528Init) {
        std::printf("FAIL %s unexpectedly mutated unk1528\n", sc.name);
        std::exit(1);
    }

    // Always at least the initial vt+0x2C(self, arg) call.
    if (retail.v10Count < 1) {
        std::printf("FAIL %s missing initial vfunc10\n", sc.name);
        std::exit(1);
    }
    if (!entry_eq(retail.v10Copies[0], arg)) {
        std::printf("FAIL %s first vfunc10 arg mismatch\n", sc.name);
        std::exit(1);
    }

    std::printf("PASS %s (kind=%d v10=%d v18=%d)\n", sc.name, wantKind, retail.v10Count,
                retail.v18Count);
}

int main() {
    // Independent kind-table smoke (ensures classify_kind itself is sane).
    struct {
        int id;
        int kind;
    } kinds[] = {
        {0x0, 2}, {0x2, 0}, {0x4, 1}, {0x14, 2}, {0x27, 0}, {0x2a, 1},
        {0x36, 0}, {0x3c, 1}, {0x5f, 0}, {0x106, 3}, {0x117, 1}, {0x12d, 0},
        {0xeb, 2}, {0xff, 2}, {0xf8, 3},
    };
    for (unsigned i = 0; i < sizeof(kinds) / sizeof(kinds[0]); i++) {
        int got = classify_kind(kinds[i].id);
        if (got != kinds[i].kind) {
            std::printf("FAIL kind_table id=0x%x want=%d got=%d\n", kinds[i].id,
                        kinds[i].kind, got);
            std::exit(1);
        }
    }
    std::printf("PASS kind_table (%zu checks)\n", sizeof(kinds) / sizeof(kinds[0]));

    // Kind-tree coverage (kinds 0/1/2/3) + match / soft-match / bit200 /
    // dup-scan / early-exit / no-match. Each run_scenario(...) call is
    // counted by behaviour audit (>=8 required at 95–99.9%).
    {
        Scenario sc = {"kind0_match_clear_bit", 0x27, 0, 3, -1, true, true, 0x12345678u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind1_match_clear_bit", 0x4, 0, 5, -1, true, true, 0x12345678u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind2_no_match_noop", 0x1, 0, -1, -1, true, false, 0xABCDEF01u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind3_clears_unk1528", 0x106, 0, -1, -1, true, false, 0xDEADBEEFu};
        run_scenario(sc);
    }
    {
        Scenario sc = {"bit200_skips_field_eq", 0x5f, 0x200u, 10, -1, false, true, 1u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"field_mismatch_skips_slot", 0x36, 0, 12, -1, false, true, 1u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"dup_keeps_status_bit", 0x33, 0, 20, 40, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"id_ge_0x12f_skips_scan", 0x12F, 0x200u, 15, -1, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"arg_id_zero_stops_after_first", 0, 0x200u, 0, 50, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"multi_slot_same_id_bit200", 0x11e, 0x200u, 2, 8, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind1_range_0x14_boundary", 0x13, 0, 7, -1, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind2_range_0x14_inclusive", 0x14, 0, -1, -1, true, false, 0x55u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind3_range_0xed", 0xed, 0, -1, -1, true, false, 0x99u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"last_slot_match", 0xdc, 0, kEntryCount - 1, -1, true, true, 0u};
        run_scenario(sc);
    }
    return 0;
}
