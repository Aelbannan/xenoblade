#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CBattleState::CBattleState_UnkVirtualFunc29
// (retail @0x80148490, size 0x15C). Static match ~95.5%: instruction shapes
// and control flow match; MWCC DSE's retail's dead trip+=7 in r3 and colors
// the scan base as r3 vs retail r4 (found already lands in r0 via goto).
// See docs/MWCC_REFERENCE.md § CBattleState_UnkVirtualFunc29.

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

enum { kEntryCount = 0x68, kSpecialBase = 0x60, kSpecialCount = 8 };

// Layout mirrors retail CBattleState: entries[0x68] at +0x8 (covers through
// +0x1528). The 8 slots cleared by vfunc29 sit at +0x1388 = entries[0x60..0x67].
struct State {
    unsigned int vtblSlot;       // 0x0
    unsigned short unk4;         // 0x4
    unsigned short unk6;         // 0x6
    Entry entries[kEntryCount];  // 0x8 .. 0x1528
    unsigned char pad1528[4];    // 0x1528 .. 0x152C
    unsigned char pad152C[0x80]; // 0x152C .. 0x15AC
    unsigned int bits[0x30 / 4]; // 0x15AC .. 0x15DC
};

struct Snapshot {
    Entry special[kSpecialCount];
    Entry kept[kSpecialBase]; // entries[0..0x5F] — must survive
    unsigned char pad152C[0x80];
    unsigned int bits[0x30 / 4];
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
    e->unk30 = 0x111u + seed;
}

static bool entry_eq(const Entry& a, const Entry& b) {
    return a.unk00 == b.unk00 && a.unk04 == b.unk04 && a.unk08 == b.unk08 && a.unk0C == b.unk0C &&
           a.unk10 == b.unk10 && a.unk14 == b.unk14 && a.unk16 == b.unk16 && a.unk18 == b.unk18 &&
           a.unk1A == b.unk1A && a.unk1C == b.unk1C && a.unk20 == b.unk20 && a.unk24 == b.unk24 &&
           a.unk28 == b.unk28 && a.unk2C == b.unk2C && a.unk2E == b.unk2E && a.unk30 == b.unk30;
}

static void set_bit(State* self, unsigned int id) {
    unsigned char* word = reinterpret_cast<unsigned char*>(self) + 0x15ACu + ((id >> 3) & ~3u);
    *reinterpret_cast<unsigned int*>(word) |= 1u << (id & 0x1Fu);
}

static bool bit_is_set(const State* self, unsigned int id) {
    const unsigned char* word =
        reinterpret_cast<const unsigned char*>(self) + 0x15ACu + ((id >> 3) & ~3u);
    return (*reinterpret_cast<const unsigned int*>(word) & (1u << (id & 0x1Fu))) != 0;
}

// Retail @0x80148490 — for each of 8 entries at this+0x1388: save unk0C, memset
// the slot, then if id < 0x12f scan 13×8 halfwords at this+0x14 for a remaining
// match; if not found (or id >= 0x12f) clear bit `id` in this+0x15AC. Finally
// memset(this+0x152C, 0, 0x80).
static void retail_vfunc29(State* self) {
    Entry* special = &self->entries[kSpecialBase];

    for (int i = 0; i < kSpecialCount; i++) {
        unsigned int id = special[i].unk0C;
        std::memset(&special[i], 0, sizeof(Entry));

        int found = 0;
        if (id < 0x12fu) {
            for (int j = 0; j < kEntryCount; j++) {
                if (self->entries[j].unk0C == id) {
                    found = 1;
                    break;
                }
            }
        }

        if (!found) {
            unsigned char* word = reinterpret_cast<unsigned char*>(self) + 0x15ACu +
                                   ((id >> 3) & ~3u);
            *reinterpret_cast<unsigned int*>(word) &= ~(1u << (id & 0x1Fu));
        }
    }

    std::memset(self->pad152C, 0, sizeof(self->pad152C));
}

// Decomp — same semantics; 13×8 raw offsets mirror src (LICM-avoidance rewrite).
static void decomp_vfunc29(State* self) {
    Entry* entry = reinterpret_cast<Entry*>(reinterpret_cast<unsigned char*>(self) + 0x1388);

    for (int i = 0; i < 8; i++, entry++) {
        unsigned int id = entry->unk0C;
        std::memset(entry, 0, 0x34);

        int found;
        if (id >= 0x12fu) {
            found = 0;
        } else {
            unsigned char* p = reinterpret_cast<unsigned char*>(self);
            int g;
            found = 0;
            for (g = 13; g != 0; g--) {
                if (id == *reinterpret_cast<unsigned short*>(p + 0x14)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x48)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x7c)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0xb0)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0xe4)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x118)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x14c)) { found = 1; goto scan_done; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x180)) { found = 1; goto scan_done; }
                p += 0x1a0;
            }
            found = 0;
        scan_done:
            ;
        }

        if (!found) {
            unsigned char* wordPtr = reinterpret_cast<unsigned char*>(self) + 0x15ACu +
                                      ((id >> 3) & ~3u);
            *reinterpret_cast<unsigned int*>(wordPtr) &= ~(1u << (id & 0x1Fu));
        }
    }

    std::memset(self->pad152C, 0, 0x80);
}

static Snapshot run_impl(void (*impl)(State*), State state) {
    Snapshot snap = {};
    impl(&state);
    std::memcpy(snap.special, &state.entries[kSpecialBase], sizeof(snap.special));
    std::memcpy(snap.kept, state.entries, sizeof(snap.kept));
    std::memcpy(snap.pad152C, state.pad152C, sizeof(snap.pad152C));
    std::memcpy(snap.bits, state.bits, sizeof(snap.bits));
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    for (int i = 0; i < kSpecialCount; i++) {
        if (!entry_eq(a.special[i], b.special[i])) {
            return false;
        }
    }
    for (int i = 0; i < kSpecialBase; i++) {
        if (!entry_eq(a.kept[i], b.kept[i])) {
            return false;
        }
    }
    if (std::memcmp(a.pad152C, b.pad152C, sizeof(a.pad152C)) != 0) {
        return false;
    }
    for (int i = 0; i < 0x30 / 4; i++) {
        if (a.bits[i] != b.bits[i]) {
            return false;
        }
    }
    return true;
}

// specialIds[8]: pre-clear unk0C for entries[0x60..0x67].
// keepId/keepSlot: place a live duplicate of keepId in entries[keepSlot]
//   (must be < 0x60) so the status bit for that id is retained when cleared
//   from the special bank. keepSlot < 0 → no duplicate.
// presetIds / presetCount: bits pre-set in this+0x15AC.
// junk152C: nonzero pattern written into pad152C before the call (must clear).
static void run_scenario(const char* name, unsigned int seedBase,
                          const unsigned short specialIds[kSpecialCount], int keepSlot,
                          unsigned short keepId, const unsigned short* presetIds,
                          int presetCount, bool junk152C) {
    State state;
    std::memset(&state, 0, sizeof(state));

    for (int i = 0; i < kEntryCount; i++) {
        fill_entry(&state.entries[i], seedBase + i + 1);
        // Distinct background ids so scans only match via explicit keepSlot /
        // specialIds collisions.
        unsigned short bg = static_cast<unsigned short>(((seedBase + i + 11) % 0x120u) + 1u);
        state.entries[i].unk0C = bg;
    }

    for (int i = 0; i < kSpecialCount; i++) {
        state.entries[kSpecialBase + i].unk0C = specialIds[i];
    }

    if (keepSlot >= 0 && keepSlot < kSpecialBase) {
        state.entries[keepSlot].unk0C = keepId;
    }

    for (int i = 0; i < presetCount; i++) {
        set_bit(&state, presetIds[i]);
    }

    if (junk152C) {
        std::memset(state.pad152C, 0xA5, sizeof(state.pad152C));
    }

    Snapshot retail = run_impl(retail_vfunc29, state);
    Snapshot decomp = run_impl(decomp_vfunc29, state);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }

    // Soft structural checks on retail result (sanity for the oracle).
    for (int i = 0; i < kSpecialCount; i++) {
        Entry z;
        std::memset(&z, 0, sizeof(z));
        if (!entry_eq(retail.special[i], z)) {
            std::printf("FAIL %s (special not cleared)\n", name);
            std::exit(1);
        }
    }
    for (int i = 0; i < 0x80; i++) {
        if (retail.pad152C[i] != 0) {
            std::printf("FAIL %s (pad152C not cleared)\n", name);
            std::exit(1);
        }
    }
    (void)bit_is_set; // keep helper for future asserts / compilers

    std::printf("PASS %s\n", name);
}

int main() {
    {
        unsigned short ids[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        run_scenario("all_zero_ids_noop_bits", 1, ids, -1, 0, 0, 0, true);
    }
    {
        unsigned short ids[8] = {0x5, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x5};
        run_scenario("single_id_clears_bit", 2, ids, -1, 0, preset, 1, true);
    }
    {
        unsigned short ids[8] = {0x5, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x5};
        run_scenario("dup_in_kept_bank_retains_bit", 3, ids, 0, 0x5, preset, 1, true);
    }
    {
        unsigned short ids[8] = {0x5, 0x5, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x5};
        // First special clears but finds second special still holding id;
        // second then clears and drops the bit.
        run_scenario("dup_inside_special_bank_order", 4, ids, -1, 0, preset, 1, false);
    }
    {
        unsigned short ids[8] = {0x12E, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x12E};
        run_scenario("id_high_boundary_0x12e_scans", 5, ids, -1, 0, preset, 1, true);
    }
    {
        unsigned short ids[8] = {0x12F, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x12F};
        run_scenario("id_boundary_0x12f_skips_scan", 6, ids, 3, 0x12F, preset, 1, true);
    }
    {
        unsigned short ids[8] = {0x7FFF, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x7FFF};
        run_scenario("id_far_above_skips_scan", 7, ids, 10, 0x7FFF, preset, 1, true);
    }
    {
        unsigned short ids[8] = {0x1, 0x1F, 0x20, 0x21, 0x40, 0x7F, 0x80, 0x100};
        unsigned short preset[] = {0x1, 0x1F, 0x20, 0x21, 0x40, 0x7F, 0x80, 0x100};
        run_scenario("eight_distinct_ids_clear_all_bits", 8, ids, -1, 0, preset, 8, true);
    }
    {
        unsigned short ids[8] = {0x9, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x9};
        run_scenario("dup_near_end_of_kept_bank", 9, ids, kSpecialBase - 1, 0x9, preset, 1, false);
    }
    {
        unsigned short ids[8] = {0xB, 0xC, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0xB, 0xC, 0xD};
        run_scenario("partial_clear_leaves_other_bits", 10, ids, -1, 0, preset, 3, true);
    }
    {
        unsigned short ids[8] = {0x15, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x15};
        run_scenario("dup_mid_group_second_check", 11, ids, 1, 0x15, preset, 1, true);
    }
    {
        unsigned short ids[8] = {0x17, 0, 0, 0, 0, 0, 0, 0};
        unsigned short preset[] = {0x17};
        run_scenario("dup_last_group_last_check", 12, ids, kSpecialBase - 1, 0x17, preset, 1, true);
    }
    return 0;
}
