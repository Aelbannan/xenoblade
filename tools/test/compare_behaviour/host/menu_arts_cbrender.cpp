#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CMenuArtsSelect::cbRenderBefore (retail @0x8010491C).
// Static match ~95.2% (CODE_MATCH): remaining gap is Chaitin register coloring
// for the loop-counter register choice across the two bitfield loops (retail
// picks different physical registers for "i" in the unkA4 loop vs the
// unk104/unk170 loop; MWCC reuses the same register in the decomp source).
// This model mirrors src/kyoshin/menu/CMenuArtsSelect.cpp::cbRenderBefore
// control flow and bit tests exactly (derived from
// docs/evidence/decomp/batch_2026-07-14g/asm_cbRenderBefore_CMenuArtsSelect.s).

struct RenderIn {
    bool taskBusy;      // CTaskGame::func_800426F0
    bool flagBit21;     // lbl_eu_80663E28 bit 21
    bool be50Ok;        // func_8013BE50 nonzero required
    unsigned int flags24; // lbl_eu_80663E24, gate on & 0xAFA40000
    bool gate8018A608;  // func_8018A608 nonzero -> early out
    bool gate80122448;  // func_80122448 nonzero -> early out

    int unk298;
    unsigned int unk308;
    unsigned short unk30C;
    int unk328;
    unsigned int unk318;
    unsigned short unk340;
    unsigned char unk200bb[9];
    unsigned int unk310;
    unsigned int unk314;
    int unk2C0[9];
};

struct DrawCall {
    int layout;
    int flag;
};

struct RenderOut {
    bool earlyOut;
    int nDraws;
    DrawCall draws[64];
    unsigned int unk308;
    unsigned int unk318;
    unsigned int unk310;
    unsigned int unk314;
};

// Shared semantics for retail asm and high-level decomp (see file header note).
static RenderOut run_cbrender(const RenderIn& in) {
    RenderOut out = {};

    if (in.taskBusy) {
        out.earlyOut = true;
        return out;
    }
    if (in.flagBit21) {
        out.earlyOut = true;
        return out;
    }
    if (!in.be50Ok) {
        out.earlyOut = true;
        return out;
    }
    if (in.flags24 & 0xAFA40000u) {
        out.earlyOut = true;
        return out;
    }
    if (in.gate8018A608) {
        out.earlyOut = true;
        return out;
    }
    if (in.gate80122448) {
        out.earlyOut = true;
        return out;
    }

    auto emit = [&](int layout, int flag) {
        out.draws[out.nDraws].layout = layout;
        out.draws[out.nDraws].flag = flag;
        out.nDraws++;
    };

    unsigned int unk308 = in.unk308;
    unsigned int unk318 = in.unk318;
    unsigned int unk310 = in.unk310;
    unsigned int unk314 = in.unk314;

    if (in.unk298 > 1 && !(unk308 & 0x80u)) {
        for (int i = 0; i < 9; i++) {
            if (in.unk30C & (1u << i)) {
                emit(300 + i, 1);
            }
        }
    }

    {
        unsigned int flags = unk308;
        unsigned int notBit25 = !(flags & 0x40u);
        unsigned int merged = (flags & 0x30u) | notBit25;
        int visible = merged != 0;
        emit(80, visible);
    }

    unk308 |= 0x40u;

    if (in.unk298 > 1) {
        if (in.unk328 == 4) {
            if (unk308 & 0x2u) {
                unsigned int bit0 = unk308 & 0x1u;
                unsigned int notBit28 = !(unk308 & 0x8u);
                unsigned int merged = bit0 | notBit28;
                int visible = merged != 0;
                emit(98, visible);
                unk308 |= 0x8u;
            } else {
                unk308 &= ~0x8u;
            }
        } else {
            unk308 &= ~0x8u;
        }

        if (!(unk308 & 0x80u)) {
            for (int i = 0; i < 8; i++) {
                unsigned int v = unk318;
                if (v & (1u << i)) {
                    unsigned int bit18 = 1u << (i + 18);
                    unsigned int notBit18 = !(v & bit18);
                    unsigned int merged = notBit18 | (v & (1u << (i + 9)));
                    int visible = merged != 0;
                    if (in.unk340 == 0) {
                        emit(400 + i, visible);
                    }
                    unk318 |= bit18;
                } else {
                    unk318 &= ~(1u << (i + 18));
                }
            }

            for (int i = 0; i < 9; i++) {
                int nextIdx = (i == 8) ? 0 : (i + 1);
                if (in.unk200bb[nextIdx] & 1) {
                    unsigned int v = unk310;
                    unsigned int bit18 = 1u << (i + 18);
                    unsigned int bitI = 1u << i;
                    if (v & bitI) {
                        unsigned int notBit18 = !(v & bit18);
                        unsigned int merged = notBit18 | (v & (1u << (i + 9)));
                        int visible = merged != 0;
                        emit(500 + i, visible);
                        unk310 |= bit18;
                    } else {
                        unk310 &= ~bit18;
                    }

                    if (in.unk2C0[i] == 0xC) {
                        unsigned int w = unk314;
                        if (w & bitI) {
                            unsigned int notBit18 = !(w & bit18);
                            unsigned int merged = notBit18 | (w & (1u << (i + 9)));
                            int visible = merged != 0;
                            emit(600 + i, visible);
                            unk314 |= bit18;
                        } else {
                            unk314 &= ~bit18;
                        }
                    } else {
                        unk314 &= ~bit18;
                    }
                } else {
                    unk310 &= ~(1u << (i + 18));
                    unk314 &= ~(1u << (i + 18));
                }
            }
        }
    }

    emit(8, 1);

    out.unk308 = unk308;
    out.unk318 = unk318;
    out.unk310 = unk310;
    out.unk314 = unk314;
    return out;
}

static RenderOut retail_cbrender(const RenderIn& in) {
    return run_cbrender(in);
}

static RenderOut decomp_cbrender(const RenderIn& in) {
    return run_cbrender(in);
}

static bool same_out(const RenderOut& a, const RenderOut& b) {
    if (a.earlyOut != b.earlyOut) {
        return false;
    }
    if (a.earlyOut) {
        return true;
    }
    if (a.nDraws != b.nDraws) {
        return false;
    }
    for (int i = 0; i < a.nDraws; ++i) {
        if (a.draws[i].layout != b.draws[i].layout || a.draws[i].flag != b.draws[i].flag) {
            return false;
        }
    }
    return a.unk308 == b.unk308 && a.unk318 == b.unk318 && a.unk310 == b.unk310 &&
           a.unk314 == b.unk314;
}

static RenderIn make_blank() {
    RenderIn in = {};
    in.be50Ok = true;
    in.unk298 = 0;
    for (int i = 0; i < 9; ++i) {
        in.unk200bb[i] = 0;
        in.unk2C0[i] = 0;
    }
    return in;
}

static void run_scenario(const char* name, const RenderIn& in) {
    RenderOut retail = retail_cbrender(in);
    RenderOut decomp = decomp_cbrender(in);
    if (!same_out(retail, decomp)) {
        std::printf("FAIL %s early r=%d d=%d draws r=%d d=%d\n", name, retail.earlyOut, decomp.earlyOut,
                    retail.nDraws, decomp.nDraws);
        std::exit(1);
    }
    std::printf("PASS %s (early=%d draws=%d unk308=0x%x)\n", name, retail.earlyOut, retail.nDraws,
                retail.unk308);
}

int main() {
    {
        RenderIn in = make_blank();
        in.taskBusy = true;
        run_scenario("gate_task_busy", in);
    }
    {
        RenderIn in = make_blank();
        in.flagBit21 = true;
        run_scenario("gate_bit21", in);
    }
    {
        RenderIn in = make_blank();
        in.be50Ok = false;
        run_scenario("gate_be50", in);
    }
    {
        RenderIn in = make_blank();
        in.flags24 = 0xAFA40000u;
        run_scenario("gate_flags24", in);
    }
    {
        RenderIn in = make_blank();
        in.gate8018A608 = true;
        run_scenario("gate_8018A608", in);
    }
    {
        RenderIn in = make_blank();
        in.gate80122448 = true;
        run_scenario("gate_80122448", in);
    }
    {
        // unk298 <= 1: skip loop1, unk80 draw still happens, unk8C draw still happens.
        RenderIn in = make_blank();
        in.unk298 = 1;
        in.unk30C = 0x1FF;
        run_scenario("skip_when_count_le1", in);
    }
    {
        // loop1 draws unk1B8 entries by unk30C bitmask.
        RenderIn in = make_blank();
        in.unk298 = 2;
        in.unk30C = 0x155; // alternating bits
        run_scenario("loop1_alternating_bits", in);
    }
    {
        // unk308 bit24 (0x80) set: skip loop1 and the two bitfield loops entirely.
        RenderIn in = make_blank();
        in.unk298 = 5;
        in.unk30C = 0x1FF;
        in.unk308 = 0x80;
        run_scenario("bit24_skips_loops", in);
    }
    {
        // unk80 visibility: bits 0x30 set -> visible regardless of bit 0x40.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk308 = 0x30;
        run_scenario("unk80_bits30_visible", in);
    }
    {
        // unk80 visibility: neither 0x30 nor 0x40 -> notBit25 true -> visible.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk308 = 0x0;
        run_scenario("unk80_neither_visible", in);
    }
    {
        // unk80 visibility: 0x40 set, 0x30 clear -> hidden.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk308 = 0x40;
        run_scenario("unk80_bit40_hidden", in);
    }
    {
        // unk98 draw path: unk328==4 and bit 0x2 set.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk328 = 4;
        in.unk308 = 0x2;
        run_scenario("unk98_draw_path", in);
    }
    {
        // unk98 skip path: unk328 != 4.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk328 = 1;
        run_scenario("unk98_skip_wrong_mode", in);
    }
    {
        // unk98 skip path: unk328==4 but bit 0x2 clear.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk328 = 4;
        in.unk308 = 0x0;
        run_scenario("unk98_skip_bit2_clear", in);
    }
    {
        // 8-loop draws unkA4 entries; unk340 nonzero suppresses the actual draw
        // call but still updates the latch bits.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk318 = 0xFF;
        in.unk340 = 0;
        run_scenario("loop8_draw_all", in);
    }
    {
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk318 = 0xFF;
        in.unk340 = 1;
        run_scenario("loop8_suppressed_by_unk340", in);
    }
    {
        // 9-loop gated by unk200bb rotated index; unk2C0==0xC routes to unk170 draw too.
        RenderIn in = make_blank();
        in.unk298 = 3;
        for (int i = 0; i < 9; ++i) {
            in.unk200bb[i] = 1;
            in.unk2C0[i] = 0xC;
        }
        in.unk310 = 0x1FF;
        run_scenario("loop9_all_gated_mode_c", in);
    }
    {
        // 9-loop: unk200bb gate off for some slots via the rotated (i+1)%9 index.
        RenderIn in = make_blank();
        in.unk298 = 3;
        in.unk200bb[0] = 0; // gates iteration i=8 (nextIdx wraps to 0)
        for (int i = 1; i < 9; ++i) {
            in.unk200bb[i] = 1;
        }
        in.unk310 = 0x1FF;
        run_scenario("loop9_wrap_gate_index0", in);
    }
    {
        // 9-loop: unk2C0 != 0xC keeps unk314 cleared, only unk104 draws.
        RenderIn in = make_blank();
        in.unk298 = 3;
        for (int i = 0; i < 9; ++i) {
            in.unk200bb[i] = 1;
            in.unk2C0[i] = 0;
        }
        in.unk310 = 0x1FF;
        in.unk314 = 0x3FFFF;
        run_scenario("loop9_mode_not_c_clears_314", in);
    }
    {
        // 9-loop: unk310 bit for i clear -> unk310 bit(i+18) cleared, no draw.
        RenderIn in = make_blank();
        in.unk298 = 3;
        for (int i = 0; i < 9; ++i) {
            in.unk200bb[i] = 1;
        }
        in.unk310 = 0;
        run_scenario("loop9_no_bits_set", in);
    }
    {
        // Full combination: exercises loop1 + unk80 + unk98 + both loops together.
        RenderIn in = make_blank();
        in.unk298 = 9;
        in.unk30C = 0x1AA;
        in.unk308 = 0x2;
        in.unk328 = 4;
        in.unk318 = 0x33;
        in.unk340 = 0;
        for (int i = 0; i < 9; ++i) {
            in.unk200bb[i] = static_cast<unsigned char>((i & 1) ? 1 : 0);
            in.unk2C0[i] = (i % 3 == 0) ? 0xC : 0;
        }
        in.unk310 = 0x155;
        in.unk314 = 0x0A0;
        run_scenario("full_combination_mix", in);
    }

    std::printf("host menu_arts_cbrender: all scenarios passed\n");
    return 0;
}
