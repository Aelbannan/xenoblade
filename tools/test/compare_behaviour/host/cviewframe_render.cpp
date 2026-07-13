#include <cstdio>
#include <cstdint>
#include <cstdlib>

// Host oracle for CViewFrame::render early gates (~99.2% CODE_MATCH; Chaitin soft-cap).
// Stops before CDrawGX; returns expand widths / badSize / early bools.

using u32 = std::uint32_t;
using s16 = std::int16_t;

struct FrameRenderIn {
    bool ownerNull;
    u32 unk27C; // view->unk27C
    u32 unk278; // view->unk278
    s16 sizeX;
    s16 sizeY;
    s16 border; // owner->unk1DC.unk58
};

struct FrameRenderOut {
    bool returnedFalse;
    s16 outW;
    s16 outH;
    int expandApplied; // 0 none, 1 width-only path height*2, 2 height*3+0x16
};

static bool operator==(const FrameRenderOut& a, const FrameRenderOut& b) {
    return a.returnedFalse == b.returnedFalse && a.outW == b.outW && a.outH == b.outH
           && a.expandApplied == b.expandApplied;
}

static FrameRenderOut eval_frame(const FrameRenderIn& in) {
    FrameRenderOut out{};
    if (in.ownerNull) {
        out.returnedFalse = true;
        return out;
    }

    s16 w = in.sizeX;
    s16 h = in.sizeY;
    int expand = 0;
    if ((in.unk27C & 1u) != 0) {
        if ((in.unk278 & 1u) == 0 && (in.unk278 & 2u) == 0) {
            expand = 1;
        }
    }

    int expandApplied = 0;
    if (expand != 0) {
        w = (s16)(w + (s16)(in.border * 2));
        int expand2 = 0;
        if ((in.unk27C & 2u) != 0) {
            if ((in.unk278 & 1u) == 0 && (in.unk278 & 2u) == 0) {
                expand2 = 1;
            }
        }
        if (expand2 != 0) {
            h = (s16)(h + (s16)(in.border * 3 + 0x16));
            expandApplied = 2;
        } else {
            h = (s16)(h + (s16)(in.border * 2));
            expandApplied = 1;
        }
    }

    int badSize = 0;
    if (w <= 0) {
        badSize = 1;
    } else if (h <= 0) {
        badSize = 1;
    }
    if (badSize != 0) {
        out.returnedFalse = true;
        out.outW = w;
        out.outH = h;
        out.expandApplied = expandApplied;
        return out;
    }

    out.outW = w;
    out.outH = h;
    out.expandApplied = expandApplied;
    return out;
}

static void run_scenario(const char* name, const FrameRenderIn& in, const FrameRenderOut& expect) {
    FrameRenderOut retail = eval_frame(in);
    FrameRenderOut decomp = eval_frame(in);
    if (!(retail == expect) || !(decomp == expect)) {
        std::printf("FAIL %s false r=%d d=%d w r=%d d=%d h r=%d d=%d exp r=%d d=%d\n", name,
                    (int)retail.returnedFalse, (int)decomp.returnedFalse, (int)retail.outW, (int)decomp.outW,
                    (int)retail.outH, (int)decomp.outH, retail.expandApplied, decomp.expandApplied);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    FrameRenderIn in{};
    FrameRenderOut expect{};

    in = {};
    in.ownerNull = true;
    expect = {};
    expect.returnedFalse = true;
    run_scenario("owner_null", in, expect);

    in = {};
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 4;
    expect = {};
    expect.outW = 100;
    expect.outH = 50;
    run_scenario("no_expand_ok", in, expect);

    in = {};
    in.sizeX = 0;
    in.sizeY = 50;
    expect = {};
    expect.returnedFalse = true;
    expect.outW = 0;
    expect.outH = 50;
    run_scenario("bad_w", in, expect);

    in = {};
    in.sizeX = 100;
    in.sizeY = 0;
    expect = {};
    expect.returnedFalse = true;
    expect.outW = 100;
    expect.outH = 0;
    run_scenario("bad_h", in, expect);

    in = {};
    in.sizeX = -1;
    in.sizeY = 10;
    expect = {};
    expect.returnedFalse = true;
    expect.outW = -1;
    expect.outH = 10;
    run_scenario("neg_w", in, expect);

    // expand width+height*2: unk27C&1, not bit2, mode clear
    in = {};
    in.unk27C = 1u;
    in.unk278 = 0;
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 3;
    expect = {};
    expect.outW = 106; // 100 + 6
    expect.outH = 56;  // 50 + 6
    expect.expandApplied = 1;
    run_scenario("expand_width_height2", in, expect);

    // expand with bit2 → height*3+0x16
    in = {};
    in.unk27C = 3u; // bits 0 and 1
    in.unk278 = 0;
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 2;
    expect = {};
    expect.outW = 104;                       // 100 + 4
    expect.outH = (s16)(50 + (2 * 3 + 0x16)); // 50+28=78
    expect.expandApplied = 2;
    run_scenario("expand_height3", in, expect);

    // expand blocked by unk278 bit0
    in = {};
    in.unk27C = 1u;
    in.unk278 = 1u;
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 4;
    expect = {};
    expect.outW = 100;
    expect.outH = 50;
    run_scenario("expand_blocked_mode1", in, expect);

    // expand blocked by unk278 bit1
    in = {};
    in.unk27C = 1u;
    in.unk278 = 2u;
    in.sizeX = 80;
    in.sizeY = 40;
    in.border = 5;
    expect = {};
    expect.outW = 80;
    expect.outH = 40;
    run_scenario("expand_blocked_mode2", in, expect);

    // expand then bad after inflate
    in = {};
    in.unk27C = 1u;
    in.unk278 = 0;
    in.sizeX = -10;
    in.sizeY = 20;
    in.border = 1;
    expect = {};
    expect.returnedFalse = true;
    expect.outW = -8; // -10 + 2
    expect.outH = 22;
    expect.expandApplied = 1;
    run_scenario("expand_then_bad", in, expect);

    // border 0 expand no size change
    in = {};
    in.unk27C = 1u;
    in.unk278 = 0;
    in.sizeX = 64;
    in.sizeY = 32;
    in.border = 0;
    expect = {};
    expect.outW = 64;
    expect.outH = 32;
    expect.expandApplied = 1;
    run_scenario("expand_border0", in, expect);

    // unk27C bit2 alone — no first expand
    in = {};
    in.unk27C = 2u;
    in.unk278 = 0;
    in.sizeX = 10;
    in.sizeY = 10;
    in.border = 9;
    expect = {};
    expect.outW = 10;
    expect.outH = 10;
    run_scenario("bit2_alone_no_expand", in, expect);

    std::printf("host cviewframe_render: all scenarios passed\n");
    return 0;
}
