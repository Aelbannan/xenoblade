#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CBattleState::CBattleState_UnkVirtualFunc11
// (retail @0x80148FC8, size 0x174). Static match ~96.2%: instruction shapes,
// branch layout and the rlwinm bitfield mask match retail 1:1, but MWCC's
// register allocator colors the vtable-temp/found-flag registers
// differently (r5 vs retail r12; r3 vs retail r0) and omits retail's dead
// induction-variable update (see docs/MWCC_REFERENCE.md
// "CBattleState_UnkVirtualFunc11").
//
// symbols.txt mangles Fv; retail/decomp entry takes (self, mask) with mask
// in r4 (same fake-Fv ABI as cf::CBattleState's UnkVirtualFunc6/26/31).

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

// vtblSlot mirrors retail's 4-byte PPC vtable pointer (NOT host `void*`,
// which is 8 bytes and would misalign every raw self+offset computation
// below — retail/decomp both use hardcoded byte offsets, not field access).
struct State {
    unsigned int vtblSlot;      // 0x0
    unsigned short unk4;        // 0x4
    unsigned short unk6;        // 0x6
    Entry entries[kEntryCount]; // 0x8 .. 0x1528
    unsigned char pad1528[4];   // 0x1528 .. 0x152C
    unsigned char pad152C[0x80]; // 0x152C .. 0x15AC
    unsigned int bits[0x30 / 4]; // 0x15AC .. 0x15DC
};

struct Vtable {
    void* slots[20]; // slot 19 == offset 0x4C (UnkVirtualFunc18)
};

static Entry* g_calledWithSlot;
static Entry g_calledWithCopy;
static int g_callCount;

static void tailFn(State* /*self*/, Entry* entry) {
    g_callCount++;
    g_calledWithSlot = entry;
    g_calledWithCopy = *entry;
}

typedef void (*TailFnPtr)(State*, Entry*);

struct Snapshot {
    Entry entries[kEntryCount];
    unsigned int bits[0x30 / 4];
    int callCount;
    Entry lastCalledWith;
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

// Retail @0x80148FC8 — walk this+0x8 entries[0x68] stride 0x34; for any slot
// whose unk30 & mask != 0: call vt+0x4C(this, slot), remember slot's id,
// memset(slot, 0, 0x34); if id < 0x12f, linear-scan the same entries[] array
// (13 groups of 8, matching MWCC's fixed-trip-count unroll) for another
// live entry sharing that id — if found, leave the this+0x15AC status bit
// alone; otherwise (not found, or id >= 0x12f) clear bit `id` in the
// this+0x15AC bitfield.
static void retail_vfunc11(State* self, unsigned int mask, TailFnPtr vfunc18) {
    Entry* entries = self->entries;

    for (int i = 0; i < kEntryCount; i++) {
        Entry* slot = &entries[i];
        if ((slot->unk30 & mask) == 0) {
            continue;
        }

        vfunc18(self, slot);
        unsigned int id = slot->unk0C;
        std::memset(slot, 0, sizeof(Entry));

        bool found = false;
        if (id < 0x12fu) {
            for (int j = 0; j < kEntryCount && !found; j++) {
                if (entries[j].unk0C == id) {
                    found = true;
                }
            }
        }

        if (!found) {
            unsigned char* word = reinterpret_cast<unsigned char*>(self) + 0x15ACu +
                                   ((id >> 3) & ~3u);
            *reinterpret_cast<unsigned int*>(word) &= ~(1u << (id & 0x1Fu));
        }
    }
}

// Decomp — same semantics as src/kyoshin/cf/object/CBattleState.cpp extern
// "C" body (raw self-relative offsets 0x14..0x180 per scan group instead of
// entries[]-array indexing, matching the source's LICM-avoidance rewrite).
static void decomp_vfunc11(State* self, unsigned int mask, TailFnPtr vfunc18) {
    Entry* entry = self->entries;

    for (int i = 0; i < kEntryCount; i++, entry++) {
        if ((entry->unk30 & mask) == 0) {
            continue;
        }

        vfunc18(self, entry);
        unsigned int id = entry->unk0C;
        std::memset(entry, 0, sizeof(Entry));

        int stillActive;
        if (id >= 0x12fu) {
            stillActive = 0;
        } else {
            unsigned char* p = reinterpret_cast<unsigned char*>(self);
            stillActive = 0;
            for (int g = 13; g != 0; g--) {
                if (id == *reinterpret_cast<unsigned short*>(p + 0x14)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x48)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x7c)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0xb0)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0xe4)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x118)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x14c)) { stillActive = 1; break; }
                if (id == *reinterpret_cast<unsigned short*>(p + 0x180)) { stillActive = 1; break; }
                p += 0x1a0;
            }
        }

        if (!stillActive) {
            unsigned char* wordPtr = reinterpret_cast<unsigned char*>(self) + 0x15ACu +
                                      ((id >> 3) & ~3u);
            *reinterpret_cast<unsigned int*>(wordPtr) &= ~(1u << (id & 0x1Fu));
        }
    }
}

static Snapshot run_impl(void (*impl)(State*, unsigned int, TailFnPtr), State state,
                          unsigned int mask) {
    Snapshot snap = {};
    g_callCount = 0;
    g_calledWithSlot = 0;
    std::memset(&g_calledWithCopy, 0, sizeof(g_calledWithCopy));

    impl(&state, mask, tailFn);

    std::memcpy(snap.entries, state.entries, sizeof(snap.entries));
    std::memcpy(snap.bits, state.bits, sizeof(snap.bits));
    snap.callCount = g_callCount;
    snap.lastCalledWith = g_calledWithCopy;
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (a.callCount != b.callCount) {
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
    if (a.callCount > 0 && !entry_eq(a.lastCalledWith, b.lastCalledWith)) {
        return false;
    }
    return true;
}

// matchSlots: indices in entries[] whose unk30 intersects mask (triggers
// vt-call + clear for that slot). dupSlot: index of another entry sharing
// matchId (still-active path) or -1 for none. matchId: the id stored in
// each matched slot before it's cleared. presetBit: whether bit `matchId`
// starts set in this+0x15AC (only observable when dupSlot == -1, i.e. the
// clear path executes).
static void run_scenario(const char* name, unsigned int seedBase, const int* matchSlots,
                          int matchCount, int dupSlot, unsigned short matchId,
                          unsigned int mask, bool presetBit) {
    State state;
    std::memset(&state, 0, sizeof(state));

    for (int i = 0; i < kEntryCount; i++) {
        fill_entry(&state.entries[i], seedBase + i + 1);
        // Background id, distinct from matchId and from other backgrounds,
        // so the scan only reports "still active" via an explicit dupSlot.
        unsigned short bg = static_cast<unsigned short>(((seedBase + i + 7) % 0x12eu) + 1u);
        if (bg == matchId) {
            bg = static_cast<unsigned short>(bg + 1u);
        }
        state.entries[i].unk0C = bg;
        state.entries[i].unk30 = 0; // not mask-selected unless in matchSlots
    }

    for (int k = 0; k < matchCount; k++) {
        int idx = matchSlots[k];
        state.entries[idx].unk0C = matchId;
        state.entries[idx].unk30 = mask | 1u; // ensure `& mask` is nonzero
    }

    if (dupSlot >= 0) {
        state.entries[dupSlot].unk0C = matchId;
        // dupSlot itself must not be mask-selected (else it'd also clear).
        state.entries[dupSlot].unk30 = 0;
    }

    if (presetBit && matchId < 0x180u * 8u) {
        unsigned char* word = reinterpret_cast<unsigned char*>(&state) + 0x15ACu +
                               ((static_cast<unsigned int>(matchId) >> 3) & ~3u);
        *reinterpret_cast<unsigned int*>(word) |= 1u << (matchId & 0x1Fu);
    }

    Snapshot retail = run_impl(retail_vfunc11, state, mask);
    Snapshot decomp = run_impl(decomp_vfunc11, state, mask);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    {
        int slots[] = {0};
        run_scenario("no_mask_match_noop", 1, slots, 0, -1, 0x10, 0x2u, false);
    }
    {
        int slots[] = {0};
        run_scenario("single_match_first_slot_id_cleared", 2, slots, 1, -1, 0x05, 0x1u, true);
    }
    {
        int slots[] = {kEntryCount - 1};
        run_scenario("single_match_last_slot_id_cleared", 3, slots, 1, -1, 0x33, 0x4u, true);
    }
    {
        int slots[] = {10};
        run_scenario("dup_in_first_group_stays_active", 4, slots, 1, 0, 0x07, 0x8u, true);
    }
    {
        int slots[] = {20};
        run_scenario("dup_in_seventh_check_of_group", 5, slots, 1, 6, 0x09, 0x10u, true);
    }
    {
        int slots[] = {30};
        run_scenario("dup_in_last_group_boundary", 6, slots, 1, kEntryCount - 1, 0x0B, 0x2u, true);
    }
    {
        int slots[] = {40};
        run_scenario("id_at_high_boundary_0x12e_scans", 7, slots, 1, -1, 0x12E, 0x1u, true);
    }
    {
        int slots[] = {50};
        run_scenario("id_at_boundary_0x12f_skips_scan", 8, slots, 1, -1, 0x12F, 0x1u, true);
    }
    {
        int slots[] = {60};
        run_scenario("id_far_above_boundary_skips_scan", 9, slots, 1, -1, 0x7FFF, 0x1u, true);
    }
    {
        int slots[] = {0, 8, 16, 55, kEntryCount - 1};
        run_scenario("multiple_matches_one_pass", 10, slots, 5, -1, 0x15, 0x1u, true);
    }
    {
        int slots[] = {5};
        run_scenario("bit_index_low_id", 11, slots, 1, -1, 0x1, 0x1u, true);
    }
    {
        int slots[] = {5};
        run_scenario("bit_index_word_boundary_id", 12, slots, 1, -1, 0x1F, 0x1u, true);
    }
    {
        int slots[] = {5};
        run_scenario("bit_index_next_word_id", 13, slots, 1, -1, 0x20, 0x1u, true);
    }
    {
        int slots[] = {5};
        run_scenario("mask_selects_only_lsb", 14, slots, 1, -1, 0x22, 0x80000000u, true);
    }
    return 0;
}
