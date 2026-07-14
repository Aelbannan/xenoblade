#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CBattleState::CBattleState_UnkVirtualFunc10
// (retail @0x80148A18, size 0x444). Static match ~98.2% CODE_MATCH: flat
// if+goto kind tree + full arg spill + save/memset + id-dup scan match
// retail instruction shapes, but MWCC DSEs retail's dead trip counter in
// r3 (li 0 / addi +7) and parks the scan base in r3 (want r4) — function
// lands at 0x43C vs retail 0x444. Same Chaitin soft-cap class as
// UnkVirtualFunc8/11 (docs/MWCC_REFERENCE.md).
//
// symbols.txt mangles Fv; retail/decomp entry takes (self, arg) with the
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

static int g_v18Count;
static Entry g_v18Copies[kMaxCalls];

static void vfunc18(State* /*self*/, Entry* entry) {
    if (g_v18Count < kMaxCalls) {
        g_v18Copies[g_v18Count++] = *entry;
    }
}

struct Snapshot {
    Entry entries[kEntryCount];
    unsigned int unk1528;
    unsigned int bits[0x30 / 4];
    int v18Count;
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

// Flat if+goto tree mirrored from retail @0x80148AF8 (shared kind leaves);
// same leaf set as UnkVirtualFunc8, applied here to the *slot* id.
static int classify_kind(int id) {
    if (id >= 0xd4)
        goto L_80148BE0;
    if (id >= 0x3e)
        goto L_80148B78;
    if (id >= 0x2c)
        goto L_80148B4C;
    if (id == 0x27)
        goto kind0;
    if (id >= 0x27)
        goto L_80148B40;
    if (id >= 4)
        goto L_80148B34;
    if (id >= 2)
        goto kind0;
    goto kind2;

L_80148B34:
    if (id >= 0x14)
        goto kind2;
    goto kind1;

L_80148B40:
    if (id >= 0x2a)
        goto kind1;
    goto kind2;

L_80148B4C:
    if (id == 0x36)
        goto kind0;
    if (id >= 0x36)
        goto L_80148B6C;
    if (id >= 0x35)
        goto kind2;
    if (id >= 0x33)
        goto kind0;
    goto kind2;

L_80148B6C:
    if (id >= 0x3c)
        goto kind1;
    goto kind2;

L_80148B78:
    if (id == 0x5f)
        goto kind0;
    if (id >= 0x5f)
        goto L_80148BB4;
    if (id >= 0x52)
        goto L_80148BA0;
    if (id >= 0x46)
        goto kind2;
    if (id >= 0x44)
        goto kind0;
    goto kind2;

L_80148BA0:
    if (id >= 0x5d)
        goto kind2;
    if (id >= 0x58)
        goto kind0;
    goto kind1;

L_80148BB4:
    if (id == 0x93)
        goto kind0;
    if (id >= 0x93)
        goto L_80148BD4;
    if (id >= 0x6a)
        goto kind2;
    if (id >= 0x65)
        goto kind1;
    goto kind2;

L_80148BD4:
    if (id >= 0xce)
        goto kind0;
    goto kind2;

L_80148BE0:
    if (id >= 0x103)
        goto L_80148C40;
    if (id == 0xeb)
        goto kind2;
    if (id >= 0xeb)
        goto L_80148C1C;
    if (id >= 0xdf)
        goto L_80148C08;
    if (id == 0xdc)
        goto kind1;
    goto kind2;

L_80148C08:
    if (id >= 0xea)
        goto kind3;
    if (id >= 0xe3)
        goto kind2;
    goto kind1;

L_80148C1C:
    if (id == 0xf7)
        goto kind0;
    if (id >= 0xf7)
        goto L_80148C34;
    if (id >= 0xed)
        goto kind3;
    goto kind0;

L_80148C34:
    if (id == 0xff)
        goto kind2;
    goto kind3;

L_80148C40:
    if (id == 0x117)
        goto kind1;
    if (id >= 0x117)
        goto L_80148C74;
    if (id >= 0x109)
        goto L_80148C60;
    if (id == 0x106)
        goto kind3;
    goto kind2;

L_80148C60:
    if (id >= 0x111)
        goto kind3;
    if (id >= 0x10d)
        goto kind2;
    goto kind3;

L_80148C74:
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

// Retail oracle — clear-by-unk2E walk (asm @0x80148A18).
static void retail_vfunc10(State* self, Entry* arg) {
    if (arg->unk2E == 0) {
        return;
    }

    unsigned short key = arg->unk2E;
    unsigned int flagBit = arg->unk30 & 0x200u;

    for (int i = 0; i < kEntryCount; i++) {
        Entry* slot = &self->entries[i];
        if (slot->unk2E != key) {
            continue;
        }
        if (flagBit == 0) {
            if (slot->unk00 != arg->unk00 || slot->unk04 != arg->unk04 ||
                slot->unk08 != arg->unk08) {
                continue;
            }
        }

        int kind = classify_kind(static_cast<int>(slot->unk0C));
        if (kind == 3) {
            self->unk1528 = 0;
        }

        Entry saved = *slot;
        std::memset(slot, 0, sizeof(Entry));

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
    }
}

// Decomp — mirrors src: 13×8 self-relative scan + stackedWords[0] optional-eq.
static void decomp_vfunc10(State* self, Entry* arg) {
    if (arg->unk2E == 0) {
        return;
    }

    unsigned int stackedWords[0x34 / 4];
    std::memcpy(stackedWords, arg, sizeof(stackedWords));
    unsigned short key = *reinterpret_cast<unsigned short*>(
        reinterpret_cast<unsigned char*>(stackedWords) + 0x2e);
    unsigned int flagBit = stackedWords[12] & 0x200u;
    unsigned int a04 = stackedWords[1];
    unsigned int a08 = stackedWords[2];

    Entry* slot = self->entries;
    for (int i = 0; i < kEntryCount; i++, slot++) {
        if (slot->unk2E != key) {
            continue;
        }
        if (flagBit == 0) {
            if (slot->unk00 != stackedWords[0] || slot->unk04 != a04 || slot->unk08 != a08) {
                continue;
            }
        }

        int kind = classify_kind(static_cast<int>(slot->unk0C));
        if (kind == 3) {
            self->unk1528 = 0;
        }

        Entry saved = *slot;
        std::memset(slot, 0, sizeof(Entry));

        unsigned int savedId = saved.unk0C;
        int stillActive = scan_still_active(self, savedId);
        if (stillActive == 0) {
            clear_bit(self, savedId);
        }
        vfunc18(self, &saved);
    }
}

static Snapshot run_impl(void (*impl)(State*, Entry*), State state, Entry arg) {
    Snapshot snap = {};
    g_v18Count = 0;
    std::memset(g_v18Copies, 0, sizeof(g_v18Copies));

    impl(&state, &arg);

    std::memcpy(snap.entries, state.entries, sizeof(snap.entries));
    snap.unk1528 = state.unk1528;
    std::memcpy(snap.bits, state.bits, sizeof(snap.bits));
    snap.v18Count = g_v18Count;
    std::memcpy(snap.v18Copies, g_v18Copies, sizeof(Entry) * g_v18Count);
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (a.unk1528 != b.unk1528 || a.v18Count != b.v18Count) {
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
    for (int i = 0; i < a.v18Count; i++) {
        if (!entry_eq(a.v18Copies[i], b.v18Copies[i])) {
            return false;
        }
    }
    return true;
}

struct Scenario {
    const char* name;
    unsigned short argKey;     // arg.unk2E
    unsigned int argFlags;     // arg.unk30
    int matchSlot;             // -1 = none
    unsigned short matchId;    // slot.unk0C when matched
    int dupSlot;               // -1 = none
    bool fieldsMatch;          // when argFlags bit0x200 clear
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
        if (bg == sc.matchId) {
            bg = static_cast<unsigned short>(bg + 1u);
        }
        state.entries[i].unk0C = bg;
        // Keep background keys away from argKey.
        unsigned short key =
            static_cast<unsigned short>(((i * 5 + 7) % 0xFEu) + 1u);
        if (key == sc.argKey) {
            key = static_cast<unsigned short>(key + 1u);
            if (key == 0) {
                key = 2;
            }
        }
        state.entries[i].unk2E = key;
        state.entries[i].unk30 = 0;
    }

    Entry arg;
    fill_entry(&arg, 99);
    arg.unk2E = sc.argKey;
    arg.unk30 = sc.argFlags;
    arg.unk00 = 0xAAAAu;
    arg.unk04 = 0xBBBBu;
    arg.unk08 = 0xCCCCu;
    arg.unk0C = 0x1; // unused for matching in vfunc10

    if (sc.matchSlot >= 0) {
        Entry& slot = state.entries[sc.matchSlot];
        slot.unk0C = sc.matchId;
        slot.unk2E = sc.argKey;
        if (sc.fieldsMatch) {
            slot.unk00 = arg.unk00;
            slot.unk04 = arg.unk04;
            slot.unk08 = arg.unk08;
        } else {
            slot.unk00 = arg.unk00 ^ 1u;
            slot.unk04 = arg.unk04;
            slot.unk08 = arg.unk08;
        }
        slot.unk10 = 0x42;
    }

    if (sc.dupSlot >= 0) {
        // Second live entry sharing matchId (different unk2E so it is not
        // cleared by this call) — keeps the status bit after matchSlot clears.
        state.entries[sc.dupSlot].unk0C = sc.matchId;
        state.entries[sc.dupSlot].unk2E =
            static_cast<unsigned short>(sc.argKey == 1 ? 2 : 1);
        state.entries[sc.dupSlot].unk00 = 0x1111u;
        state.entries[sc.dupSlot].unk04 = 0x2222u;
        state.entries[sc.dupSlot].unk08 = 0x3333u;
    }

    if (sc.presetBit && sc.matchId < 0x180u * 8u) {
        set_bit(&state, sc.matchId);
    }

    Snapshot retail = run_impl(retail_vfunc10, state, arg);
    Snapshot decomp = run_impl(decomp_vfunc10, state, arg);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s\n", sc.name);
        std::exit(1);
    }

    if (sc.matchSlot >= 0) {
        bool expectClear = (sc.argFlags & 0x200u) != 0 || sc.fieldsMatch;
        if (expectClear) {
            int wantKind = classify_kind(static_cast<int>(sc.matchId));
            if (wantKind == 3) {
                if (retail.unk1528 != 0) {
                    std::printf("FAIL %s kind3 did not clear unk1528\n", sc.name);
                    std::exit(1);
                }
            } else if (retail.unk1528 != sc.unk1528Init) {
                std::printf("FAIL %s unexpectedly mutated unk1528\n", sc.name);
                std::exit(1);
            }
            if (retail.v18Count < 1) {
                std::printf("FAIL %s expected vt+0x4C call\n", sc.name);
                std::exit(1);
            }
        } else if (retail.v18Count != 0 || retail.unk1528 != sc.unk1528Init) {
            std::printf("FAIL %s field mismatch should skip\n", sc.name);
            std::exit(1);
        }
    } else if (sc.argKey != 0 && retail.v18Count != 0) {
        std::printf("FAIL %s unexpected clear\n", sc.name);
        std::exit(1);
    }

    std::printf("PASS %s (v18=%d unk1528=0x%X)\n", sc.name, retail.v18Count,
                retail.unk1528);
}

int main() {
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

    // Early-out + match/clear + bit200 + field mismatch + dup-scan + kind3.
    // Each run_scenario(...) counts toward behaviour audit (>=8 at 95–99.9%).
    {
        Scenario sc = {"early_out_zero_key", 0, 0, -1, 0, -1, true, false, 0x111u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind0_match_clear_bit", 0x55, 0, 3, 0x27, -1, true, true, 0x12345678u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind1_match_clear_bit", 0x56, 0, 5, 0x4, -1, true, true, 0x12345678u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind2_no_match_noop", 0x57, 0, -1, 0, -1, true, false, 0xABCDEF01u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind3_clears_unk1528", 0x58, 0, 8, 0x106, -1, true, false, 0xDEADBEEFu};
        run_scenario(sc);
    }
    {
        Scenario sc = {"bit200_skips_field_eq", 0x59, 0x200u, 10, 0x5f, -1, false, true, 1u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"field_mismatch_skips_slot", 0x5A, 0, 12, 0x36, -1, false, true, 1u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"dup_keeps_status_bit", 0x5B, 0, 20, 0x33, 40, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"id_ge_12f_clears_bit", 0x5C, 0, 15, 0x130, -1, true, true, 0u};
        run_scenario(sc);
    }
    {
        Scenario sc = {"kind1_slot_0x117", 0x60, 0, 7, 0x117, -1, true, false, 0x55u};
        run_scenario(sc);
    }
    {
        // Two slots share argKey with bit200 so both clear.
        State state;
        std::memset(&state, 0, sizeof(state));
        state.unk1528 = 0x44u;
        for (int i = 0; i < kEntryCount; i++) {
            fill_entry(&state.entries[i], static_cast<unsigned int>(i + 1));
            state.entries[i].unk0C = static_cast<unsigned short>(0x10 + (i % 20));
            state.entries[i].unk2E = 1;
        }
        Entry arg;
        fill_entry(&arg, 7);
        arg.unk2E = 0x5E;
        arg.unk30 = 0x200u;
        state.entries[4].unk2E = 0x5E;
        state.entries[4].unk0C = 0x2;
        state.entries[30].unk2E = 0x5E;
        state.entries[30].unk0C = 0x4;
        set_bit(&state, 0x2);
        set_bit(&state, 0x4);
        Snapshot retail = run_impl(retail_vfunc10, state, arg);
        Snapshot decomp = run_impl(decomp_vfunc10, state, arg);
        if (!equals(retail, decomp) || retail.v18Count != 2) {
            std::printf("FAIL two_matches_bit200 (v18=%d)\n", retail.v18Count);
            std::exit(1);
        }
        std::printf("PASS two_matches_bit200 (v18=%d)\n", retail.v18Count);
    }

    std::printf("ALL PASS battlestate-vfunc10\n");
    return 0;
}
