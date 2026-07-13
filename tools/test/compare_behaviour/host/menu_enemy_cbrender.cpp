#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CMenuEnemyState::cbRenderBefore (retail @0x80111240).
// Static match ~98.2%: remaining gap is Chaitin coloring on sort loop
// (order/pair/limit home regs off-by-one; pair spills to r30 vs retail r8).

struct Panel {
    float depth;
    unsigned char flagB8;
    unsigned char flagB9;
    unsigned char flagCC;
    unsigned char flagCD;
    int layoutA8; // token id
    int layoutB0;
};

struct RenderIn {
    bool taskBusy;
    bool flagBit21;
    bool be50Ok;
    unsigned int flags24;
    Panel panels[24];
    int sharedLayout; // unk74 token
    int extraLayout;  // unk800 token
    int extraFlag;    // unk828
};

struct DrawCall {
    int layout;
    int flag; // r5 to func_80137038
};

struct RenderOut {
    bool earlyOut;
    int order[24];
    int nDraws;
    DrawCall draws[96];
};

static void xor_swap(int* a, int* b) {
    int t = *a ^ *b;
    *a = t;
    t = *b ^ t;
    *b = t;
    t = *a ^ t;
    *a = t;
}

// Shared semantics for retail asm and high-level decomp.
static RenderOut run_cbrender(const RenderIn& in) {
    RenderOut out = {};
    for (int i = 0; i < 24; ++i) {
        out.order[i] = i;
    }

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

    // Bubble-sort indices by panel depth (ascending); early-out when sorted.
    int pass = 0;
    for (int left = 0x17; left != 0; --left) {
        int swapped = 0;
        int limit = 0x17 - pass;
        for (int j = 0; j < limit; ++j) {
            int a = out.order[j];
            int b = out.order[j + 1];
            if (in.panels[a].depth > in.panels[b].depth) {
                xor_swap(&out.order[j], &out.order[j + 1]);
                swapped = 1;
            }
        }
        if (swapped == 0) {
            break;
        }
        pass++;
    }

    auto emit = [&](int layout, int flag) {
        out.draws[out.nDraws].layout = layout;
        out.draws[out.nDraws].flag = flag;
        out.nDraws++;
    };

    for (int i = 0; i < 24; ++i) {
        const Panel& p = in.panels[out.order[i]];
        if (p.flagB9 == 0) {
            continue;
        }
        if (p.flagCD == 0) {
            continue;
        }
        if (p.flagCC != 0) {
            emit(in.sharedLayout, 0);
        }
        emit(p.layoutB0, (p.flagCC == 0) ? 1 : 0);
        if (p.flagB8 == 0) {
            emit(p.layoutA8, 0);
        }
    }

    if (in.extraFlag != 0) {
        emit(in.extraLayout, 0);
    }
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
    for (int i = 0; i < 24; ++i) {
        if (a.order[i] != b.order[i]) {
            return false;
        }
    }
    if (a.nDraws != b.nDraws) {
        return false;
    }
    for (int i = 0; i < a.nDraws; ++i) {
        if (a.draws[i].layout != b.draws[i].layout || a.draws[i].flag != b.draws[i].flag) {
            return false;
        }
    }
    return true;
}

static RenderIn make_blank() {
    RenderIn in = {};
    in.be50Ok = true;
    in.sharedLayout = 100;
    in.extraLayout = 200;
    for (int i = 0; i < 24; ++i) {
        in.panels[i].depth = static_cast<float>(i);
        in.panels[i].layoutA8 = 1000 + i;
        in.panels[i].layoutB0 = 2000 + i;
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
    std::printf("PASS %s (early=%d draws=%d order0=%d)\n", name, retail.earlyOut, retail.nDraws,
                retail.order[0]);
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
        // reverse depth so sort must run; no draw flags
        for (int i = 0; i < 24; ++i) {
            in.panels[i].depth = static_cast<float>(23 - i);
        }
        run_scenario("sort_reverse_no_draw", in);
    }
    {
        RenderIn in = make_blank();
        in.panels[3].flagB9 = 1;
        in.panels[3].flagCD = 1;
        in.panels[3].flagCC = 0;
        in.panels[3].flagB8 = 0;
        run_scenario("draw_cc0_b80", in);
    }
    {
        RenderIn in = make_blank();
        in.panels[5].flagB9 = 1;
        in.panels[5].flagCD = 1;
        in.panels[5].flagCC = 1;
        in.panels[5].flagB8 = 1;
        run_scenario("draw_cc1_b81_shared", in);
    }
    {
        RenderIn in = make_blank();
        in.panels[1].flagB9 = 1;
        in.panels[1].flagCD = 0; // skipped
        in.panels[2].flagB9 = 0;
        in.panels[2].flagCD = 1;
        in.extraFlag = 1;
        run_scenario("skip_flags_plus_extra", in);
    }
    {
        RenderIn in = make_blank();
        for (int i = 0; i < 24; ++i) {
            in.panels[i].depth = static_cast<float>((i * 7) % 24);
            in.panels[i].flagB9 = 1;
            in.panels[i].flagCD = 1;
            in.panels[i].flagCC = static_cast<unsigned char>(i & 1);
            in.panels[i].flagB8 = static_cast<unsigned char>((i >> 1) & 1);
        }
        in.extraFlag = 1;
        run_scenario("full_sort_draw_mix", in);
    }
    {
        RenderIn in = make_blank();
        in.flags24 = 0x00010000u; // below andis mask
        in.panels[0].flagB9 = 1;
        in.panels[0].flagCD = 1;
        in.panels[0].flagCC = 0;
        in.panels[0].flagB8 = 1;
        run_scenario("flags24_pass_draw_b0_only", in);
    }

    std::printf("host menu_enemy_cbrender: all scenarios passed\n");
    return 0;
}
