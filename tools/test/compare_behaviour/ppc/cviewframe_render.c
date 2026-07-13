#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cviewframe_render_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern int rb_render__10CViewFrameFv(void* frame);
extern int dc_render__10CViewFrameFv(void* frame);

static u8 sDummy[8];

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void run_scenario(volatile BehaviourResult* result, const char* name, CViewFrameScenario* in,
                         int expectFalse, s16 expectW, s16 expectH, int expectExp) {
    CViewFrameScenario dcIn;
    int rbRet;
    int dcRet;

    cviewframe_render_mock_reset();
    cviewframe_render_set(0, in);
    dcIn = *in;
    cviewframe_render_set(1, &dcIn);

    rbRet = rb_render__10CViewFrameFv(sDummy);
    dcRet = dc_render__10CViewFrameFv(sDummy);

    if (rbRet == dcRet && g_rb_cfr_out.returnedFalse == expectFalse
        && g_dc_cfr_out.returnedFalse == expectFalse && g_rb_cfr_out.outW == expectW
        && g_dc_cfr_out.outW == expectW && g_rb_cfr_out.outH == expectH && g_dc_cfr_out.outH == expectH
        && g_rb_cfr_out.expandApplied == expectExp && g_dc_cfr_out.expandApplied == expectExp) {
        result->passed++;
        return;
    }
    fail(result, name);
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    CViewFrameScenario in;

    behaviour_result_init(result);

    memset(&in, 0, sizeof(in));
    in.ownerNull = 1;
    run_scenario(result, "owner_null", &in, 1, 0, 0, 0);

    memset(&in, 0, sizeof(in));
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 4;
    run_scenario(result, "no_expand_ok", &in, 0, 100, 50, 0);

    memset(&in, 0, sizeof(in));
    in.sizeX = 0;
    in.sizeY = 50;
    run_scenario(result, "bad_w", &in, 1, 0, 50, 0);

    memset(&in, 0, sizeof(in));
    in.sizeX = 100;
    in.sizeY = 0;
    run_scenario(result, "bad_h", &in, 1, 100, 0, 0);

    memset(&in, 0, sizeof(in));
    in.sizeX = -1;
    in.sizeY = 10;
    run_scenario(result, "neg_w", &in, 1, -1, 10, 0);

    memset(&in, 0, sizeof(in));
    in.unk27C = 1u;
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 3;
    run_scenario(result, "expand_width_height2", &in, 0, 106, 56, 1);

    memset(&in, 0, sizeof(in));
    in.unk27C = 3u;
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 2;
    run_scenario(result, "expand_height3", &in, 0, 104, 78, 2); /* 50+(2*3+0x16)=78 */

    memset(&in, 0, sizeof(in));
    in.unk27C = 1u;
    in.unk278 = 1u;
    in.sizeX = 100;
    in.sizeY = 50;
    in.border = 4;
    run_scenario(result, "expand_blocked_mode1", &in, 0, 100, 50, 0);

    memset(&in, 0, sizeof(in));
    in.unk27C = 1u;
    in.unk278 = 2u;
    in.sizeX = 80;
    in.sizeY = 40;
    in.border = 5;
    run_scenario(result, "expand_blocked_mode2", &in, 0, 80, 40, 0);

    memset(&in, 0, sizeof(in));
    in.unk27C = 1u;
    in.sizeX = -10;
    in.sizeY = 20;
    in.border = 1;
    run_scenario(result, "expand_then_bad", &in, 1, -8, 22, 1);

    memset(&in, 0, sizeof(in));
    in.unk27C = 1u;
    in.sizeX = 64;
    in.sizeY = 32;
    in.border = 0;
    run_scenario(result, "expand_border0", &in, 0, 64, 32, 1);

    memset(&in, 0, sizeof(in));
    in.unk27C = 2u;
    in.sizeX = 10;
    in.sizeY = 10;
    in.border = 9;
    run_scenario(result, "bit2_alone_no_expand", &in, 0, 10, 10, 0);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cviewframe-render", result);
    return (int)result->exit_code;
}
