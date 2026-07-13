#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CAIAction::CAIAction_UnkVirtualFunc1 (retail @0x8014B41C).
// Static match ~93.2%: frameless leaf; MWCC CSE of early zeros + stwux vs retail stwx+add.
// symbols.txt mangles Fv; retail/decomp entry takes (this, outA, outB).

struct Slot {
    unsigned int unk00;
    unsigned int unk04;
    unsigned int unk08;
    unsigned int unk0C;
    unsigned short unk10;
    short unk12;
    float unk14;
    unsigned int unk18;
    unsigned int unk1C;
};

struct ExportBuf {
    unsigned char pad[0x200];
    Slot* buffer;
    unsigned int unk204;
    unsigned int unk208;
    unsigned int unk20C;
};

struct Action {
    unsigned char pad0[0x20C];
    void* unk20C;
    unsigned int unk210;
    unsigned int unk214;
    unsigned int unk218;
    unsigned char pad21C[0x8C0];
    unsigned char trailer[0x20];
};

struct Snapshot {
    Slot outA;
    unsigned int out204;
    unsigned int out208;
    Slot slots[16];
};

static void fill_slot(Slot* s, unsigned int seed) {
    s->unk00 = 0x1000u + seed;
    s->unk04 = 0x2000u + seed;
    s->unk08 = 0x3000u + seed;
    s->unk0C = 0x4000u + seed;
    s->unk10 = static_cast<unsigned short>(0x50u + seed);
    s->unk12 = static_cast<short>(0x60 + static_cast<int>(seed));
    s->unk14 = 1.5f + static_cast<float>(seed);
    s->unk18 = 0x7000u + seed;
    s->unk1C = 0x8000u + seed;
}

static void copy_slot(Slot* dst, const Slot* src) {
    dst->unk00 = src->unk00;
    dst->unk08 = src->unk08;
    dst->unk04 = src->unk04;
    dst->unk0C = src->unk0C;
    dst->unk10 = src->unk10;
    dst->unk12 = src->unk12;
    dst->unk14 = src->unk14;
    dst->unk18 = src->unk18;
    dst->unk1C = src->unk1C;
}

static bool slot_eq(const Slot& a, const Slot& b) {
    return a.unk00 == b.unk00 && a.unk04 == b.unk04 && a.unk08 == b.unk08 && a.unk0C == b.unk0C &&
           a.unk10 == b.unk10 && a.unk12 == b.unk12 && a.unk14 == b.unk14 && a.unk18 == b.unk18 &&
           a.unk1C == b.unk1C;
}

// Retail @0x8014B41C — trailer→outA, clear outB counters, ring-copy into outB.buffer.
static Snapshot retail_vfunc1(Action* self, ExportBuf* outB) {
    Snapshot snap = {};
    Slot* trailer = reinterpret_cast<Slot*>(self->trailer);
    Slot outA = {};
    unsigned int i;

    outA.unk00 = trailer->unk00;
    i = 0;
    outA.unk08 = trailer->unk08;
    outA.unk04 = trailer->unk04;
    outA.unk0C = trailer->unk0C;
    outA.unk10 = trailer->unk10;
    outA.unk12 = trailer->unk12;
    outA.unk14 = trailer->unk14;
    outA.unk18 = trailer->unk18;
    outA.unk1C = trailer->unk1C;

    outB->unk208 = 0;
    outB->unk204 = 0;

    while (i < self->unk214) {
        unsigned int ringIdx = (self->unk210 + i) % self->unk218;
        int outIdx = static_cast<int>(outB->unk204 + outB->unk208) % static_cast<int>(outB->unk20C);
        Slot* src = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(self->unk20C) + (ringIdx << 5));
        Slot* dst = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(outB->buffer) + (outIdx << 5));
        copy_slot(dst, src);
        outB->unk208 = outB->unk208 + 1;
        i = i + 1;
    }

    snap.outA = outA;
    snap.out204 = outB->unk204;
    snap.out208 = outB->unk208;
    if (outB->buffer != 0) {
        std::memcpy(snap.slots, outB->buffer, sizeof(snap.slots));
    }
    return snap;
}

// Decomp — same semantics as src/kyoshin/cf/object/CAIAction.cpp extern "C" body.
static Snapshot decomp_vfunc1(Action* self, ExportBuf* outB) {
    Snapshot snap = {};
    Slot* trailer = reinterpret_cast<Slot*>(self->trailer);
    Slot outA = {};
    unsigned int i;

    i = trailer->unk00;
    outA.unk00 = i;
    i = 0;
    {
        unsigned int a = trailer->unk04;
        unsigned int b = trailer->unk08;
        outA.unk08 = b;
        outA.unk04 = a;
    }
    outA.unk0C = trailer->unk0C;
    outA.unk10 = trailer->unk10;
    outA.unk12 = trailer->unk12;
    outA.unk14 = trailer->unk14;
    outA.unk18 = trailer->unk18;
    outA.unk1C = trailer->unk1C;

    outB->unk208 = 0;
    outB->unk204 = 0;

    while (i < self->unk214) {
        unsigned int ringIdx = (self->unk210 + i) % self->unk218;
        int outIdx = static_cast<int>(outB->unk204 + outB->unk208) % static_cast<int>(outB->unk20C);
        Slot* src = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(self->unk20C) + (ringIdx << 5));
        Slot* dst = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(outB->buffer) + (outIdx << 5));
        dst->unk00 = src->unk00;
        {
            unsigned int t8 = src->unk08;
            unsigned int t4 = src->unk04;
            dst->unk04 = t4;
            dst->unk08 = t8;
        }
        dst->unk0C = src->unk0C;
        dst->unk10 = src->unk10;
        dst->unk12 = src->unk12;
        dst->unk14 = src->unk14;
        dst->unk18 = src->unk18;
        dst->unk1C = src->unk1C;
        outB->unk208 = outB->unk208 + 1;
        i = i + 1;
    }

    snap.outA = outA;
    snap.out204 = outB->unk204;
    snap.out208 = outB->unk208;
    if (outB->buffer != 0) {
        std::memcpy(snap.slots, outB->buffer, sizeof(snap.slots));
    }
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (!slot_eq(a.outA, b.outA) || a.out204 != b.out204 || a.out208 != b.out208) {
        return false;
    }
    for (int i = 0; i < 16; ++i) {
        if (!slot_eq(a.slots[i], b.slots[i])) {
            return false;
        }
    }
    return true;
}

static void run_scenario(const char* name, unsigned int ringStart, unsigned int count,
                         unsigned int capacity, unsigned int colStart, unsigned int cols) {
    Slot slots[16];
    Slot outSlots[16];
    Action act = {};
    ExportBuf exp = {};
    Slot trailerSeed = {};

    std::memset(slots, 0, sizeof(slots));
    std::memset(outSlots, 0, sizeof(outSlots));
    for (unsigned int i = 0; i < capacity && i < 16u; ++i) {
        fill_slot(&slots[i], i + 1);
    }
    fill_slot(&trailerSeed, 0xABu);
    std::memcpy(act.trailer, &trailerSeed, sizeof(trailerSeed));
    act.unk20C = slots;
    act.unk210 = ringStart;
    act.unk214 = count;
    act.unk218 = capacity;

    exp.buffer = outSlots;
    exp.unk204 = colStart;
    exp.unk208 = 0xDEADBEEFu;
    exp.unk20C = cols;

    Action actR = act;
    ExportBuf expR = exp;
    Slot outR[16];
    std::memcpy(outR, outSlots, sizeof(outR));
    expR.buffer = outR;

    Action actD = act;
    ExportBuf expD = exp;
    Slot outD[16];
    std::memcpy(outD, outSlots, sizeof(outD));
    expD.buffer = outD;

    Snapshot retail = retail_vfunc1(&actR, &expR);
    Snapshot decomp = decomp_vfunc1(&actD, &expD);
    if (!equals(retail, decomp)) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("empty_count", 0, 0, 16, 0, 8);
    run_scenario("one_from_head", 0, 1, 16, 0, 8);
    run_scenario("one_from_mid", 5, 1, 16, 0, 8);
    run_scenario("wrap_ring", 14, 4, 16, 0, 8);
    run_scenario("full_capacity_copy", 0, 16, 16, 0, 16);
    run_scenario("col_offset", 0, 3, 16, 2, 8);
    run_scenario("col_wrap", 0, 5, 16, 6, 8);
    run_scenario("col_and_ring_wrap", 12, 8, 16, 5, 8);
    run_scenario("small_capacity", 1, 3, 4, 0, 4);
    run_scenario("cols_one", 0, 3, 16, 0, 1);
    run_scenario("count_two_cols_three", 3, 2, 16, 1, 3);
    run_scenario("zeros_trailer_overlap", 0, 0, 16, 0, 4);
    run_scenario("max_iters_half", 8, 8, 16, 0, 16);
    run_scenario("ring_mod_nonzero_start", 7, 6, 10, 3, 5);
    return 0;
}
