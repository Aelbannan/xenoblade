#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for cf::CAIAction::CAIAction_UnkVirtualFunc2 (retail @0x8014B52C).
// Static match ~96.0%: inverse of vfunc1 (import trailer from inA, drain ring from
// inB into self->unk20C). Same MWCC CSE (stwux vs retail stwx+add) caps high-level
// C/C++ short of FULL_MATCH, same as vfunc1.
// symbols.txt mangles Fv; retail/decomp entry takes (this, inA, inB).

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

struct ImportBuf {
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
    Slot trailer;
    unsigned int unk210;
    unsigned int unk214;
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

// Retail @0x8014B52C — clear unk214/unk210, import inA→trailer, ring-copy inB.buffer
// into self->unk20C at (unk210+unk214)%unk218 (signed div), incrementing unk214.
static Snapshot retail_vfunc2(Action* self, Slot* inA, ImportBuf* inB) {
    Snapshot snap = {};
    Slot* trailer = reinterpret_cast<Slot*>(self->trailer);

    self->unk214 = 0;
    self->unk210 = 0;

    trailer->unk00 = inA->unk00;
    trailer->unk08 = inA->unk08;
    trailer->unk04 = inA->unk04;
    trailer->unk0C = inA->unk0C;
    trailer->unk10 = inA->unk10;
    trailer->unk12 = inA->unk12;
    trailer->unk14 = inA->unk14;
    trailer->unk18 = inA->unk18;
    trailer->unk1C = inA->unk1C;

    for (unsigned int i = 0; i < inB->unk208; ++i) {
        unsigned int srcIdx = (inB->unk204 + i) % inB->unk20C;
        int dstIdx = static_cast<int>(self->unk210 + self->unk214) % static_cast<int>(self->unk218);
        Slot* src = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(inB->buffer) + (srcIdx << 5));
        Slot* dst = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(self->unk20C) + (dstIdx << 5));
        copy_slot(dst, src);
        self->unk214 = self->unk214 + 1;
    }

    snap.trailer = *trailer;
    snap.unk210 = self->unk210;
    snap.unk214 = self->unk214;
    if (self->unk20C != 0) {
        std::memcpy(snap.slots, self->unk20C, sizeof(snap.slots));
    }
    return snap;
}

// Decomp — same semantics as src/kyoshin/cf/object/CAIAction.cpp extern "C" body.
static Snapshot decomp_vfunc2(Action* self, Slot* inA, ImportBuf* inB) {
    Snapshot snap = {};
    Slot* trailer = reinterpret_cast<Slot*>(self->trailer);
    unsigned int i;

    self->unk214 = 0;
    i = 0;
    self->unk210 = 0;

    trailer->unk00 = inA->unk00;
    {
        unsigned int a = inA->unk04;
        unsigned int b = inA->unk08;
        trailer->unk08 = b;
        trailer->unk04 = a;
    }
    trailer->unk0C = inA->unk0C;
    trailer->unk10 = inA->unk10;
    trailer->unk12 = inA->unk12;
    trailer->unk14 = inA->unk14;
    trailer->unk18 = inA->unk18;
    trailer->unk1C = inA->unk1C;

    while (i < inB->unk208) {
        unsigned int srcIdx = (inB->unk204 + i) % inB->unk20C;
        int dstIdx = static_cast<int>(self->unk210 + self->unk214) % static_cast<int>(self->unk218);
        Slot* src = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(inB->buffer) + (srcIdx << 5));
        Slot* dst = reinterpret_cast<Slot*>(reinterpret_cast<unsigned char*>(self->unk20C) + (dstIdx << 5));

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

        self->unk214 = self->unk214 + 1;
        i = i + 1;
    }

    snap.trailer = *trailer;
    snap.unk210 = self->unk210;
    snap.unk214 = self->unk214;
    if (self->unk20C != 0) {
        std::memcpy(snap.slots, self->unk20C, sizeof(snap.slots));
    }
    return snap;
}

static bool equals(const Snapshot& a, const Snapshot& b) {
    if (!slot_eq(a.trailer, b.trailer) || a.unk210 != b.unk210 || a.unk214 != b.unk214) {
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
    Slot inSlots[16];
    Action act = {};
    ImportBuf imp = {};
    Slot inASeed = {};

    std::memset(slots, 0, sizeof(slots));
    std::memset(inSlots, 0, sizeof(inSlots));
    for (unsigned int i = 0; i < cols && i < 16u; ++i) {
        fill_slot(&inSlots[i], i + 1);
    }
    fill_slot(&inASeed, 0xCDu);

    act.unk20C = slots;
    act.unk210 = ringStart;
    act.unk214 = 0xDEADBEEFu; // must be cleared by the function
    act.unk218 = capacity;

    imp.buffer = inSlots;
    imp.unk204 = colStart;
    imp.unk208 = count;
    imp.unk20C = cols;

    Action actR = act;
    Slot outR[16];
    std::memcpy(outR, slots, sizeof(outR));
    actR.unk20C = outR;

    Action actD = act;
    Slot outD[16];
    std::memcpy(outD, slots, sizeof(outD));
    actD.unk20C = outD;

    Slot inAR = inASeed;
    Slot inAD = inASeed;

    Snapshot retail = retail_vfunc2(&actR, &inAR, &imp);
    Snapshot decomp = decomp_vfunc2(&actD, &inAD, &imp);
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
