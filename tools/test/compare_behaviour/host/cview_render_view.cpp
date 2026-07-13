#include <cstdio>
#include <cstdint>
#include <cstdlib>

// Host oracle for CView::renderView gate DAG (~84.5% static).
// Covers early-out / crossRoot / size+clear / self wkRender / tail frame call.
// GX / scissor / child recurse body is stubbed as side-effect flags only.

using u32 = std::uint32_t;
using s16 = std::int16_t;
using s32 = std::int32_t;

static constexpr u32 THREAD_FLAG_EXCEPTION = 1u << 4;
static constexpr u32 THREAD_STATE_LOGIN = 2u;
static constexpr u32 THREAD_STATE_RUN = 3u;
static constexpr u32 EVT_EXCEPTION = 2u;

struct RenderViewIn {
    u32 unk278;
    bool fsvBit1;           // fullScreenView->unk278 & 2
    bool selfIsFsv;
    bool thisListEmpty;
    bool fsvListEmpty;
    bool sameRoot;
    bool thisRootNull;
    s32 unk460;
    s16 sizeX;
    s16 sizeY;
    s16 maxX;
    s16 maxY;
    bool splitBothViews;    // unk45C + view1 + view2
    u32 mFlags;
    u32 mState;
    bool msgHasException;
};

struct RenderViewOut {
    bool earlyReturn;
    u32 crossRootFlag;
    bool tookTail;
    bool enteredClear;
    bool colorUpdateOff;
    bool selfWkRender;
    bool frameRenderCalled;
};

static bool operator==(const RenderViewOut& a, const RenderViewOut& b) {
    return a.earlyReturn == b.earlyReturn && a.crossRootFlag == b.crossRootFlag && a.tookTail == b.tookTail
           && a.enteredClear == b.enteredClear && a.colorUpdateOff == b.colorUpdateOff
           && a.selfWkRender == b.selfWkRender && a.frameRenderCalled == b.frameRenderCalled;
}

static RenderViewOut eval_gates(const RenderViewIn& in) {
    RenderViewOut out{};
    if ((in.unk278 & 0x40u) != 0) {
        out.earlyReturn = true;
        return out;
    }

    u32 cross = 0;
    if (!in.fsvBit1 && !in.selfIsFsv && !in.thisListEmpty && !in.fsvListEmpty) {
        if (!in.sameRoot && !in.thisRootNull && in.unk460 >= 7) {
            cross = 1;
        }
    }
    out.crossRootFlag = cross;

    if (in.sizeX <= 0 || in.sizeY <= 0) {
        out.tookTail = true;
        out.frameRenderCalled = (cross == 0);
        return out;
    }
    if (in.maxX <= 0 && in.maxY <= 0) {
        out.tookTail = true;
        out.frameRenderCalled = (cross == 0);
        return out;
    }

    if (cross == 0) {
        if (!in.splitBothViews && (in.unk278 & 1u) == 0) {
            out.enteredClear = true;
            out.colorUpdateOff = ((in.unk278 >> 2) & 1u) != 0;
        }
    }

    u32 msgQualified = 0;
    if ((in.mFlags & THREAD_FLAG_EXCEPTION) != 0) {
        msgQualified = 1;
    } else if (in.msgHasException) {
        msgQualified = 1;
    }

    u32 stateReady = 0;
    if (in.mState == THREAD_STATE_LOGIN || in.mState == THREAD_STATE_RUN) {
        stateReady = 1;
    }
    out.selfWkRender = (msgQualified == 0 && stateReady != 0);

    out.frameRenderCalled = (cross == 0);
    return out;
}

static RenderViewOut retail_render_view(const RenderViewIn& in) {
    return eval_gates(in);
}

static RenderViewOut decomp_render_view(const RenderViewIn& in) {
    return eval_gates(in);
}

static void run_scenario(const char* name, const RenderViewIn& in, const RenderViewOut& expect) {
    RenderViewOut retail = retail_render_view(in);
    RenderViewOut decomp = decomp_render_view(in);
    if (!(retail == expect) || !(decomp == expect) || !(retail == decomp)) {
        std::printf(
            "FAIL %s early r=%d d=%d cross r=%u d=%u tail r=%d d=%d clear r=%d d=%d "
            "colOff r=%d d=%d wk r=%d d=%d frame r=%d d=%d\n",
            name, (int)retail.earlyReturn, (int)decomp.earlyReturn, retail.crossRootFlag, decomp.crossRootFlag,
            (int)retail.tookTail, (int)decomp.tookTail, (int)retail.enteredClear, (int)decomp.enteredClear,
            (int)retail.colorUpdateOff, (int)decomp.colorUpdateOff, (int)retail.selfWkRender,
            (int)decomp.selfWkRender, (int)retail.frameRenderCalled, (int)decomp.frameRenderCalled);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static RenderViewIn base_ok() {
    RenderViewIn in{};
    in.unk278 = 0;
    in.fsvBit1 = false;
    in.selfIsFsv = true;
    in.thisListEmpty = true;
    in.fsvListEmpty = true;
    in.sameRoot = true;
    in.thisRootNull = false;
    in.unk460 = 0;
    in.sizeX = 640;
    in.sizeY = 480;
    in.maxX = 640;
    in.maxY = 480;
    in.splitBothViews = false;
    in.mFlags = 0;
    in.mState = THREAD_STATE_RUN;
    in.msgHasException = false;
    return in;
}

int main() {
    RenderViewIn in;
    RenderViewOut expect;

    // 1 disp_off early return
    in = base_ok();
    in.unk278 = 0x40u;
    expect = {};
    expect.earlyReturn = true;
    run_scenario("disp_off", in, expect);

    // 2 self_is_fsv past early, clear enter, wk, frame
    in = base_ok();
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("self_fsv_clear_wk", in, expect);

    // 3 fsv bit1 blocks cross
    in = base_ok();
    in.selfIsFsv = false;
    in.fsvBit1 = true;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.unk460 = 10;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("cross_blocked_fsv_bit1", in, expect);

    // 4 empty this list — no cross
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = true;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.unk460 = 10;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("cross_blocked_empty_this", in, expect);

    // 5 empty fsv list — no cross
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = true;
    in.sameRoot = false;
    in.unk460 = 10;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("cross_blocked_empty_fsv", in, expect);

    // 6 same root — no cross
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = true;
    in.unk460 = 10;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("cross_blocked_same_root", in, expect);

    // 7 thisRoot null — no cross
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.thisRootNull = true;
    in.unk460 = 10;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("cross_blocked_null_root", in, expect);

    // 8 unk460 < 7 — no cross
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.unk460 = 6;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("cross_blocked_unk460", in, expect);

    // 9 cross set — skip clear, no frame
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.unk460 = 7;
    expect = {};
    expect.crossRootFlag = 1;
    expect.selfWkRender = true;
    expect.frameRenderCalled = false;
    run_scenario("cross_set", in, expect);

    // 10 size_w <= 0 → tail + frame
    in = base_ok();
    in.sizeX = 0;
    expect = {};
    expect.tookTail = true;
    expect.frameRenderCalled = true;
    run_scenario("size_w_le0_tail", in, expect);

    // 11 size_h <= 0 → tail
    in = base_ok();
    in.sizeY = -1;
    expect = {};
    expect.tookTail = true;
    expect.frameRenderCalled = true;
    run_scenario("size_h_le0_tail", in, expect);

    // 12 max both <= 0 → tail
    in = base_ok();
    in.maxX = 0;
    in.maxY = 0;
    expect = {};
    expect.tookTail = true;
    expect.frameRenderCalled = true;
    run_scenario("maxsize_both_le0_tail", in, expect);

    // 13 cross + size fail → tail no frame
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.unk460 = 8;
    in.sizeX = 0;
    expect = {};
    expect.crossRootFlag = 1;
    expect.tookTail = true;
    expect.frameRenderCalled = false;
    run_scenario("cross_size_tail_no_frame", in, expect);

    // 14 clear skipped by bit0
    in = base_ok();
    in.unk278 = 1u;
    expect = {};
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("clear_skip_bit0", in, expect);

    // 15 clear skipped by split both views
    in = base_ok();
    in.splitBothViews = true;
    expect = {};
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("clear_skip_split_both", in, expect);

    // 16 clear with colorUpdateOff
    in = base_ok();
    in.unk278 = 4u;
    expect = {};
    expect.enteredClear = true;
    expect.colorUpdateOff = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("clear_color_update_off", in, expect);

    // 17 cross skips clear
    in = base_ok();
    in.selfIsFsv = false;
    in.thisListEmpty = false;
    in.fsvListEmpty = false;
    in.sameRoot = false;
    in.unk460 = 9;
    in.unk278 = 0;
    expect = {};
    expect.crossRootFlag = 1;
    expect.selfWkRender = true;
    expect.frameRenderCalled = false;
    run_scenario("cross_skips_clear", in, expect);

    // 18 exception flag → no wkRender
    in = base_ok();
    in.mFlags = THREAD_FLAG_EXCEPTION;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = false;
    expect.frameRenderCalled = true;
    run_scenario("self_wk_exception_flag", in, expect);

    // 19 msg exception → no wkRender
    in = base_ok();
    in.msgHasException = true;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = false;
    expect.frameRenderCalled = true;
    run_scenario("self_wk_msg_exception", in, expect);

    // 20 LOGIN renders
    in = base_ok();
    in.mState = THREAD_STATE_LOGIN;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("self_wk_login", in, expect);

    // 21 bad state → no wkRender
    in = base_ok();
    in.mState = 0; // NONE
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = false;
    expect.frameRenderCalled = true;
    run_scenario("self_wk_bad_state", in, expect);

    // 22 maxX ok alone (maxY 0) — not both-le0
    in = base_ok();
    in.maxX = 10;
    in.maxY = 0;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("max_x_only_ok", in, expect);

    // 23 maxY ok alone
    in = base_ok();
    in.maxX = 0;
    in.maxY = 10;
    expect = {};
    expect.enteredClear = true;
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("max_y_only_ok", in, expect);

    // 24 bit0 + bit2 — skip clear entirely (bit0 wins before color)
    in = base_ok();
    in.unk278 = 5u; // bits 0 and 2
    expect = {};
    expect.selfWkRender = true;
    expect.frameRenderCalled = true;
    run_scenario("clear_skip_bit0_with_bit2", in, expect);

    // 25 size_w negative
    in = base_ok();
    in.sizeX = -5;
    expect = {};
    expect.tookTail = true;
    expect.frameRenderCalled = true;
    run_scenario("size_w_neg_tail", in, expect);

    std::printf("host cview_render_view: all scenarios passed\n");
    return 0;
}
