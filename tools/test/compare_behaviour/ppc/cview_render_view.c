#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cview_render_view_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void rb_renderView__5CViewFv(void* view);
extern void dc_renderView__5CViewFv(void* view);

static u8 sDummyView[16];

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static int outs_equal(void) {
    return g_rb_rv_mock.earlyReturn == g_dc_rv_mock.earlyReturn
           && g_rb_rv_mock.crossRootFlag == g_dc_rv_mock.crossRootFlag
           && g_rb_rv_mock.tookTail == g_dc_rv_mock.tookTail
           && g_rb_rv_mock.clearEntered == g_dc_rv_mock.clearEntered
           && g_rb_rv_mock.colorUpdateOff == g_dc_rv_mock.colorUpdateOff
           && g_rb_rv_mock.selfWkRender == g_dc_rv_mock.selfWkRender
           && g_rb_rv_mock.frameRenderCalled == g_dc_rv_mock.frameRenderCalled;
}

static void run_scenario(volatile BehaviourResult* result, const char* name, CViewRVScenario* in,
                         int expectEarly, u32 expectCross, int expectTail, int expectClear,
                         int expectColOff, int expectWk, int expectFrame) {
    CViewRVScenario dcIn;

    cview_rv_mock_reset();
    cview_rv_set_scenario(0, in);
    dcIn = *in;
    cview_rv_set_scenario(1, &dcIn);

    rb_renderView__5CViewFv(sDummyView);
    dc_renderView__5CViewFv(sDummyView);

    if (!outs_equal() || g_rb_rv_mock.earlyReturn != expectEarly
        || g_rb_rv_mock.crossRootFlag != expectCross || g_rb_rv_mock.tookTail != expectTail
        || g_rb_rv_mock.clearEntered != expectClear || g_rb_rv_mock.colorUpdateOff != expectColOff
        || g_rb_rv_mock.selfWkRender != expectWk || g_rb_rv_mock.frameRenderCalled != expectFrame) {
        fail(result, name);
        return;
    }
    result->passed++;
}

static void base_ok(CViewRVScenario* in) {
    memset(in, 0, sizeof(*in));
    in->selfIsFsv = 1;
    in->thisListEmpty = 1;
    in->fsvListEmpty = 1;
    in->sameRoot = 1;
    in->sizeX = 640;
    in->sizeY = 480;
    in->maxX = 640;
    in->maxY = 480;
    in->mState = 3; /* RUN */
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    CViewRVScenario in;

    behaviour_result_init(result);

    base_ok(&in);
    in.unk278 = 0x40u;
    run_scenario(result, "disp_off", &in, 1, 0, 0, 0, 0, 0, 0);

    base_ok(&in);
    run_scenario(result, "self_fsv_clear_wk", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.fsvBit1 = 1;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.unk460 = 10;
    run_scenario(result, "cross_blocked_fsv_bit1", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 1;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.unk460 = 10;
    run_scenario(result, "cross_blocked_empty_this", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 1;
    in.sameRoot = 0;
    in.unk460 = 10;
    run_scenario(result, "cross_blocked_empty_fsv", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 1;
    in.unk460 = 10;
    run_scenario(result, "cross_blocked_same_root", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.thisRootNull = 1;
    in.unk460 = 10;
    run_scenario(result, "cross_blocked_null_root", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.unk460 = 6;
    run_scenario(result, "cross_blocked_unk460", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.unk460 = 7;
    run_scenario(result, "cross_set", &in, 0, 1, 0, 0, 0, 1, 0);

    base_ok(&in);
    in.sizeX = 0;
    run_scenario(result, "size_w_le0_tail", &in, 0, 0, 1, 0, 0, 0, 1);

    base_ok(&in);
    in.sizeY = -1;
    run_scenario(result, "size_h_le0_tail", &in, 0, 0, 1, 0, 0, 0, 1);

    base_ok(&in);
    in.maxX = 0;
    in.maxY = 0;
    run_scenario(result, "maxsize_both_le0_tail", &in, 0, 0, 1, 0, 0, 0, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.unk460 = 8;
    in.sizeX = 0;
    run_scenario(result, "cross_size_tail_no_frame", &in, 0, 1, 1, 0, 0, 0, 0);

    base_ok(&in);
    in.unk278 = 1u;
    run_scenario(result, "clear_skip_bit0", &in, 0, 0, 0, 0, 0, 1, 1);

    base_ok(&in);
    in.splitBothViews = 1;
    run_scenario(result, "clear_skip_split_both", &in, 0, 0, 0, 0, 0, 1, 1);

    base_ok(&in);
    in.unk278 = 4u;
    run_scenario(result, "clear_color_update_off", &in, 0, 0, 0, 1, 1, 1, 1);

    base_ok(&in);
    in.selfIsFsv = 0;
    in.thisListEmpty = 0;
    in.fsvListEmpty = 0;
    in.sameRoot = 0;
    in.unk460 = 9;
    run_scenario(result, "cross_skips_clear", &in, 0, 1, 0, 0, 0, 1, 0);

    base_ok(&in);
    in.mFlags = 1u << 4;
    run_scenario(result, "self_wk_exception_flag", &in, 0, 0, 0, 1, 0, 0, 1);

    base_ok(&in);
    in.msgHasException = 1;
    run_scenario(result, "self_wk_msg_exception", &in, 0, 0, 0, 1, 0, 0, 1);

    base_ok(&in);
    in.mState = 2; /* LOGIN */
    run_scenario(result, "self_wk_login", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.mState = 0;
    run_scenario(result, "self_wk_bad_state", &in, 0, 0, 0, 1, 0, 0, 1);

    base_ok(&in);
    in.maxX = 10;
    in.maxY = 0;
    run_scenario(result, "max_x_only_ok", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.maxX = 0;
    in.maxY = 10;
    run_scenario(result, "max_y_only_ok", &in, 0, 0, 0, 1, 0, 1, 1);

    base_ok(&in);
    in.unk278 = 5u;
    run_scenario(result, "clear_skip_bit0_with_bit2", &in, 0, 0, 0, 0, 0, 1, 1);

    base_ok(&in);
    in.sizeX = -5;
    run_scenario(result, "size_w_neg_tail", &in, 0, 0, 1, 0, 0, 0, 1);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cview-render-view", result);
    return (int)result->exit_code;
}
