#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cviewroot_gfsv_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

extern void* rb_getFullScreenView__9CViewRootFv(void);
extern void* dc_getFullScreenView__9CViewRootFv(void);

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void sync_decomp(void) {
    memcpy(&g_dc_gfsv_mock, &g_rb_gfsv_mock, sizeof(g_rb_gfsv_mock));
}

static void run_scenario(volatile BehaviourResult* result, const char* name, void* expected) {
    void* retailOut;
    void* decompOut;

    sync_decomp();
    retailOut = rb_getFullScreenView__9CViewRootFv();
    decompOut = dc_getFullScreenView__9CViewRootFv();

    if (retailOut != expected || decompOut != expected || retailOut != decompOut) {
        fail(result, name);
        return;
    }
    result->passed++;
}

static void begin_desktop(void) {
    cviewroot_gfsv_mock_reset();
    g_rb_gfsv_mock.hasRoot = 1;
    g_rb_gfsv_mock.hasDesktop = 1;
}

static CViewRootGfsvChild* add_child(void) {
    return cviewroot_gfsv_add_child(0);
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    CViewRootGfsvChild* child;
    CViewRootGfsvChild* second;

    behaviour_result_init(result);

    cviewroot_gfsv_mock_reset();
    run_scenario(result, "null_root", 0);

    cviewroot_gfsv_mock_reset();
    g_rb_gfsv_mock.hasRoot = 1;
    run_scenario(result, "null_desktop", 0);

    begin_desktop();
    run_scenario(result, "empty_children_desktop", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mFlags = GFSV_THREAD_FLAG_EXCEPTION;
    child->unk278 = 1;
    run_scenario(result, "exception_flag_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->msgs[0] = GFSV_EVT_EXCEPTION;
    child->msgCount = 1;
    child->unk278 = 1;
    run_scenario(result, "msg_exception_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mState = 1;
    child->unk278 = 1;
    run_scenario(result, "non_login_run_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_LOGIN;
    child->mFlags = GFSV_THREAD_FLAG_NO_EVENT;
    child->unk278 = 1;
    run_scenario(result, "no_event_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_RUN;
    child->unk278 = 1;
    run_scenario(result, "unk278_bit0_fast", (void*)1);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_LOGIN;
    child->unk278 = 0;
    run_scenario(result, "fullscreen_size_match", (void*)1);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_RUN;
    child->unk278 = 0;
    child->posX = 10;
    run_scenario(result, "nonzero_pos_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_RUN;
    child->unk278 = 0;
    child->viewW = 320;
    run_scenario(result, "width_mismatch_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mFlags = GFSV_THREAD_FLAG_EXCEPTION;
    child->unk278 = 1;
    second = add_child();
    second->mState = GFSV_THREAD_STATE_RUN;
    second->unk278 = 1;
    run_scenario(result, "second_child_bit0", (void*)2);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_RUN;
    child->unk278 = 4;
    run_scenario(result, "unk278_bit2_skip", (void*)0xFFFFFFFEu);

    begin_desktop();
    child = add_child();
    child->mState = GFSV_THREAD_STATE_RUN;
    child->unk278 = 0;
    child->viewH = 240;
    run_scenario(result, "height_mismatch_skip", (void*)0xFFFFFFFEu);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cviewroot-get-fullscreen-view", result);
    return (int)result->exit_code;
}
