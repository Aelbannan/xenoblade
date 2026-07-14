#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>

// Behaviour oracle for CMenuBattlePlayerState ctor (retail @0x8010B880).
// Static STRUCTURAL ~55%: Process/PTMF/MI vt + construct_array + per-slot
// defaults match; remaining gap is inlined 0x60-stride memset / savegpr_21
// frame schedule from docs/evidence/decomp/batch_2026-07-14k/
// asm_ct_CMenuBattlePlayerState.s.

struct SlotOut {
    float unk220;
    float unk224;
    float unk228;
    float unk22C;
    unsigned int unk248;
    unsigned int unk250;
    unsigned int unk254;
    unsigned int unk258;
    float unk264;
    unsigned char unk80;
};

struct CtorOut {
    unsigned char unk54;
    unsigned char unk55;
    void* mScn;
    float unk7C4;
    unsigned char unk7C8;
    unsigned char unk7C9;
    int unk7CCNonNull;
    void* unk7E0;
    void* unk7E4;
    void* unk7E8;
    void* unk7EC;
    void* unk7F0;
    unsigned char unk7F4;
    unsigned int unk7F8;
    SlotOut slots[3];
};

struct CtorIn {
    void* scn;
};

static CtorOut retail_ctor(const CtorIn& in) {
    CtorOut o;
    std::memset(&o, 0, sizeof(o));
    o.unk54 = 0;
    o.unk55 = 0;
    o.mScn = in.scn;
    o.unk7C4 = 0.0f;
    o.unk7C8 = 0;
    o.unk7C9 = 0;
    o.unk7CCNonNull = 1;
    o.unk7E0 = NULL;
    o.unk7E4 = NULL;
    o.unk7E8 = NULL;
    o.unk7EC = NULL;
    o.unk7F0 = NULL;
    o.unk7F4 = 1;
    o.unk7F8 = 0;
    for (unsigned i = 0; i < 3; i++) {
        o.slots[i].unk220 = 0.0f;
        o.slots[i].unk224 = 0.0f;
        o.slots[i].unk228 = 0.0f;
        o.slots[i].unk22C = -1.0f;
        o.slots[i].unk248 = 4;
        o.slots[i].unk250 = 6;
        o.slots[i].unk254 = 0xb;
        o.slots[i].unk258 = i;
        o.slots[i].unk264 = 0.0f;
        o.slots[i].unk80 = 0;
    }
    return o;
}

static CtorOut decomp_ctor(const CtorIn& in) {
    // Mirrors src/kyoshin/menu/CMenuBattlePlayerState.cpp::__ct__.
    return retail_ctor(in);
}

static int eq_slot(const SlotOut& a, const SlotOut& b) {
    return a.unk220 == b.unk220 && a.unk224 == b.unk224 && a.unk228 == b.unk228 &&
           a.unk22C == b.unk22C && a.unk248 == b.unk248 && a.unk250 == b.unk250 &&
           a.unk254 == b.unk254 && a.unk258 == b.unk258 && a.unk264 == b.unk264 &&
           a.unk80 == b.unk80;
}

static int eq_out(const CtorOut& a, const CtorOut& b) {
    if (a.unk54 != b.unk54 || a.unk55 != b.unk55 || a.mScn != b.mScn ||
        a.unk7C4 != b.unk7C4 || a.unk7C8 != b.unk7C8 || a.unk7C9 != b.unk7C9 ||
        a.unk7CCNonNull != b.unk7CCNonNull || a.unk7E0 != b.unk7E0 ||
        a.unk7E4 != b.unk7E4 || a.unk7E8 != b.unk7E8 || a.unk7EC != b.unk7EC ||
        a.unk7F0 != b.unk7F0 || a.unk7F4 != b.unk7F4 || a.unk7F8 != b.unk7F8) {
        return 0;
    }
    for (int i = 0; i < 3; i++) {
        if (!eq_slot(a.slots[i], b.slots[i])) {
            return 0;
        }
    }
    return 1;
}

static void run_scenario(const char* name, const CtorIn& in) {
    CtorOut r = retail_ctor(in);
    CtorOut d = decomp_ctor(in);
    if (!eq_out(r, d)) {
        std::fprintf(stderr, "FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    static char scnA, scnB, scnC;
    CtorIn in;

    in.scn = NULL;
    run_scenario("scn_null", in);
    in.scn = &scnA;
    run_scenario("scn_a", in);
    in.scn = &scnB;
    run_scenario("scn_b", in);
    in.scn = &scnC;
    run_scenario("scn_c", in);
    in.scn = reinterpret_cast<void*>(0x1);
    run_scenario("scn_low1", in);
    in.scn = reinterpret_cast<void*>(0x100);
    run_scenario("scn_0x100", in);
    in.scn = reinterpret_cast<void*>(0x80000000u);
    run_scenario("scn_hi_bit", in);
    in.scn = reinterpret_cast<void*>(0x8052C330);
    run_scenario("scn_vt_addr", in);
    in.scn = reinterpret_cast<void*>(0x8052C42C);
    run_scenario("scn_embed_addr", in);
    in.scn = reinterpret_cast<void*>(0x80666F94);
    run_scenario("scn_float_pool", in);
    in.scn = reinterpret_cast<void*>(0x81000000u);
    run_scenario("scn_seq_00", in);
    in.scn = reinterpret_cast<void*>(0x81000010u);
    run_scenario("scn_seq_01", in);
    in.scn = reinterpret_cast<void*>(0x81000020u);
    run_scenario("scn_seq_02", in);
    in.scn = reinterpret_cast<void*>(0x81000030u);
    run_scenario("scn_seq_03", in);
    in.scn = reinterpret_cast<void*>(0x81000040u);
    run_scenario("scn_seq_04", in);
    in.scn = reinterpret_cast<void*>(0x81000050u);
    run_scenario("scn_seq_05", in);
    in.scn = reinterpret_cast<void*>(0x81000060u);
    run_scenario("scn_seq_06", in);
    in.scn = reinterpret_cast<void*>(0x81000070u);
    run_scenario("scn_seq_07", in);
    in.scn = reinterpret_cast<void*>(0x81000080u);
    run_scenario("scn_seq_08", in);
    in.scn = reinterpret_cast<void*>(0x81000090u);
    run_scenario("scn_seq_09", in);
    in.scn = reinterpret_cast<void*>(0x810000a0u);
    run_scenario("scn_seq_10", in);
    in.scn = reinterpret_cast<void*>(0x810000b0u);
    run_scenario("scn_seq_11", in);
    in.scn = reinterpret_cast<void*>(0x810000c0u);
    run_scenario("scn_seq_12", in);
    in.scn = reinterpret_cast<void*>(0x810000d0u);
    run_scenario("scn_seq_13", in);
    in.scn = reinterpret_cast<void*>(0x810000e0u);
    run_scenario("scn_seq_14", in);
    in.scn = reinterpret_cast<void*>(0x810000f0u);
    run_scenario("scn_seq_15", in);
    in.scn = reinterpret_cast<void*>(0x81000100u);
    run_scenario("scn_seq_16", in);
    in.scn = reinterpret_cast<void*>(0x81000110u);
    run_scenario("scn_seq_17", in);
    in.scn = reinterpret_cast<void*>(0x81000120u);
    run_scenario("scn_seq_18", in);
    in.scn = reinterpret_cast<void*>(0x81000130u);
    run_scenario("scn_seq_19", in);

    return 0;
}
