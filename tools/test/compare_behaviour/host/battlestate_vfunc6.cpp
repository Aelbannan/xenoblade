#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CBattleState::CBattleState_UnkVirtualFunc6
// (retail @0x80148210, size 0x154). Static match ~92.2%: instruction shapes
// match retail 1:1 but MWCC's register allocator colors the leaf-tail-call
// preamble/epilogue differently (see docs/MWCC_REFERENCE.md
// "CBattleState_UnkVirtualFunc6").
//
// symbols.txt mangles Fv; retail/decomp entry takes (self, arg) with arg in
// r4 (same fake-Fv ABI as cf::CAIAction's UnkVirtualFunc1/2).

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

struct State {
    unsigned char pad8[0x1380];
    Entry entries[8]; // 0x1388 .. 0x1528
    unsigned char pad1528[0x84];
    unsigned int bits[12]; // 0x15AC .. 0x15DC
};

const float kScale = 0.9f; // lbl_eu_80667414

struct Vtable {
    void* slots[19]; // slot 18 == offset 0x48 (UnkVirtualFunc17)
};

struct Snapshot {
    Entry entries[8];
    unsigned int bits[12];
    Entry calledWith;
    bool called;
};

static void fill_entry(Entry* e, unsigned int seed) {
    e->unk00 = 0x10000u + seed;
    e->unk04 = 0x20000u + seed;
    e->unk08 = 0x30000u + seed;
    e->unk0C = static_cast<unsigned short>(seed & 0xFFFFu);
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
    e->unk30 = 0x40000u + seed;
}

static bool entry_eq(const Entry& a, const Entry& b) {
    return a.unk00 == b.unk00 && a.unk04 == b.unk04 && a.unk08 == b.unk08 && a.unk0C == b.unk0C &&
           a.unk10 == b.unk10 && a.unk14 == b.unk14 && a.unk16 == b.unk16 && a.unk18 == b.unk18 &&
           a.unk1A == b.unk1A && a.unk1C == b.unk1C && a.unk20 == b.unk20 && a.unk24 == b.unk24 &&
           a.unk28 == b.unk28 && a.unk2C == b.unk2C && a.unk2E == b.unk2E && a.unk30 == b.unk30;
}

static Entry* g_calledWithSlot;
static Entry g_calledWithCopy;
static bool g_called;

static void tailFn(State* /*self*/, Entry* entry) {
    g_called = true;
    g_calledWithSlot = entry;
    g_calledWithCopy = *entry;
}

typedef void (*TailFnPtr)(State*, Entry*);

// Retail @0x80148210 — set bit arg->unk0C in this+0x15AC; scan 8 entries at
// this+0x1388 (stride 0x34) for a matching unk0C, clamping unk10 up to
// arg->unk10 then down to arg->unk18 (if nonzero); else fill the first free
// slot (unk0C==0) from arg with unk1C := arg->unk20 and
// unk28 := 0.9f * arg->unk24; tail-call vtable slot 0x48 with the
// matched/filled entry.
static void retail_vfunc6(State* self, Entry* arg, TailFnPtr vfunc17) {
    unsigned int byteOffset = ((static_cast<unsigned int>(arg->unk0C) >> 3) & ~3u);
    unsigned char* word = reinterpret_cast<unsigned char*>(self) + 0x15ACu + byteOffset;
    *reinterpret_cast<unsigned int*>(word) |= 1u << (arg->unk0C & 0x1Fu);

    Entry* entries = self->entries;

    for (int i = 0; i < 8; i++) {
        Entry* e = &entries[i];
        if (e->unk0C == arg->unk0C) {
            if (e->unk10 < arg->unk10) {
                e->unk10 = arg->unk10;
            }
            if (arg->unk18 != 0) {
                if (e->unk10 > arg->unk18) {
                    e->unk10 = arg->unk18;
                }
            }
            vfunc17(self, e);
            return;
        }
    }

    for (int i = 0; i < 8; i++) {
        Entry* e = &entries[i];
        if (e->unk0C == 0) {
            e->unk00 = arg->unk00;
            e->unk04 = arg->unk04;
            e->unk08 = arg->unk08;
            e->unk0C = arg->unk0C;
            e->unk10 = arg->unk10;
            e->unk14 = arg->unk14;
            e->unk16 = arg->unk16;
            e->unk18 = arg->unk18;
            e->unk1A = arg->unk1A;
            e->unk1C = arg->unk1C;
            e->unk20 = arg->unk20;
            e->unk24 = arg->unk24;
            float scaled = kScale * e->unk24;
            e->unk28 = arg->unk28;
            e->unk2C = arg->unk2C;
            e->unk2E = arg->unk2E;
            e->unk30 = arg->unk30;
            e->unk1C = e->unk20;
            e->unk28 = scaled;
            vfunc17(self, e);
            return;
        }
    }
}

// Decomp — same semantics as src/kyoshin/cf/object/CBattleState.cpp extern
// "C" body (manual vtable-slot-0x48 dispatch instead of a named vfunc17).
static void decomp_vfunc6(State* self, Entry* arg, TailFnPtr vfunc17) {
    Entry* entries;
    Entry* p;
    int n;

    *reinterpret_cast<unsigned int*>(reinterpret_cast<unsigned char*>(self) + 0x15ACu +
                                      ((static_cast<unsigned int>(arg->unk0C) >> 3) & ~3u)) |=
        1u << (arg->unk0C & 0x1Fu);

    entries = self->entries;
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
            vfunc17(self, p);
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
            {
                float scaled = kScale * entries->unk24;
                entries->unk28 = arg->unk28;
                entries->unk2C = arg->unk2C;
                entries->unk2E = arg->unk2E;
                entries->unk30 = arg->unk30;
                entries->unk1C = entries->unk20;
                entries->unk28 = scaled;
            }
            vfunc17(self, entries);
            return;
        }
    }
}

static Snapshot run_impl(void (*impl)(State*, Entry*, TailFnPtr), State state, Entry arg) {
    Snapshot snap = {};
    g_called = false;
    g_calledWithSlot = 0;
    std::memset(&g_calledWithCopy, 0, sizeof(g_calledWithCopy));

    impl(&state, &arg, tailFn);

    std::memcpy(snap.entries, state.entries, sizeof(snap.entries));
    std::memcpy(snap.bits, state.bits, sizeof(snap.bits));
    snap.called = g_called;
    snap.calledWith = g_calledWithCopy;
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (a.called != b.called) {
        return false;
    }
    for (int i = 0; i < 8; i++) {
        if (!entry_eq(a.entries[i], b.entries[i])) {
            return false;
        }
    }
    for (int i = 0; i < 12; i++) {
        if (a.bits[i] != b.bits[i]) {
            return false;
        }
    }
    if (a.called && !entry_eq(a.calledWith, b.calledWith)) {
        return false;
    }
    return true;
}

// matchSlot: index (0..7) of the slot whose unk0C is set to argId (match
// path exercised), or -1 for no match (fill/no-op path).
// freeMask: bit i set => slot i starts free (unk0C == 0); ignored for slot
// index == matchSlot.
static void run_scenario(const char* name, unsigned int seedBase, int matchSlot,
                          unsigned int freeMask, unsigned short argId, int argVal10,
                          short argVal18) {
    State state;
    Entry arg;

    std::memset(&state, 0, sizeof(state));
    for (int i = 0; i < 8; i++) {
        if (i == matchSlot) {
            fill_entry(&state.entries[i], seedBase + i + 1);
            state.entries[i].unk0C = argId;
        } else if (freeMask & (1u << i)) {
            std::memset(&state.entries[i], 0, sizeof(Entry));
        } else {
            fill_entry(&state.entries[i], seedBase + i + 1);
            unsigned short id = static_cast<unsigned short>(((seedBase + i + 1) & 0x7FFFu) | 1u);
            if (id == argId) {
                id = static_cast<unsigned short>(id + 1);
            }
            state.entries[i].unk0C = id;
        }
    }

    fill_entry(&arg, seedBase + 0x100);
    arg.unk0C = argId;
    arg.unk10 = argVal10;
    arg.unk18 = argVal18;

    Snapshot retail = run_impl(retail_vfunc6, state, arg);
    Snapshot decomp = run_impl(decomp_vfunc6, state, arg);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("free_slot_first", 1, -1, 0xFFu, 0x1234, 10, 5);
    run_scenario("free_slot_middle", 2, -1, 0x04u, 0x2345, -3, 0);
    run_scenario("free_slot_last", 3, -1, 0x80u, 0x3456, 7, -2);
    run_scenario("all_full_no_match_no_op", 4, -1, 0x00u, 0x9999, 1, 1);
    run_scenario("match_first_slot_clamp_up", 5, 0, 0x00u, 0x0002, 500, 0);
    run_scenario("match_last_slot_clamp_down", 6, 7, 0x00u, 0x0009, -50, 5);
    run_scenario("match_mid_slot_no_lower_clamp_zero", 7, 3, 0x00u, 0x000A, 100, 0);
    run_scenario("match_lower_clamp_negative_bound", 8, 2, 0x00u, 0x000B, 100, -10);
    run_scenario("match_entry_already_higher_no_clamp_up", 9, 4, 0x00u, 0x000C, -100, 0);
    run_scenario("bit_index_low", 10, -1, 0xFFu, 0x0001, 0, 0);
    run_scenario("bit_index_high", 11, -1, 0xFFu, 0x017F, 0, 0);
    run_scenario("bit_index_word_boundary", 12, -1, 0xFFu, 0x001F, 0, 0);
    run_scenario("bit_index_next_word", 13, -1, 0xFFu, 0x0020, 0, 0);
    run_scenario("scaled_float_positive", 14, -1, 0x01u, 0x0777, 3, 1);
    run_scenario("scaled_float_negative_duration", 15, -1, 0x02u, 0x0778, 3, 1);
    run_scenario("match_slot_zero_with_free_others", 16, 0, 0xFEu, 0x000D, 20, 0);
    return 0;
}
