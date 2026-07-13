#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cviewroot_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

extern void* rb_getView__9CViewRootFUl(u32 id);
extern void* dc_getView__9CViewRootFUl(u32 id);

extern CViewRootMockRoot* rb_lbl_eu_806655D0;
extern CViewRootMockRoot* dc_lbl_eu_806655D0;

static u8 sRetailRoot[0x520];
static u8 sDecompRoot[0x520];

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void setup_side(int decomp, CViewRootMockRoot* root) {
    CViewRootMockState* mock = decomp ? &g_dc_cviewroot_mock : &g_rb_cviewroot_mock;
    CViewRootMockRoot* typed = root;

    mock->singleton = typed;
    typed->mStartNodePtr = &typed->sentinel;
    if (decomp) {
        dc_lbl_eu_806655D0 = typed;
    } else {
        rb_lbl_eu_806655D0 = typed;
    }
}

static void run_scenario(volatile BehaviourResult* result, const char* name, u32 id, int childCount,
                         int tableIndex, int threadType, void* expected) {
    void* retailOut;
    void* decompOut;

    cviewroot_mock_reset();
    setup_side(0, (CViewRootMockRoot*)sRetailRoot);
    setup_side(1, (CViewRootMockRoot*)sDecompRoot);
    cviewroot_mock_set_children(0, childCount);
    cviewroot_mock_set_children(1, childCount);
    if (tableIndex >= 0) {
        cviewroot_mock_bind_work(id, 0, tableIndex, threadType);
        cviewroot_mock_bind_work(id, 1, tableIndex, threadType);
    }

    retailOut = rb_getView__9CViewRootFUl(id);
    decompOut = dc_getView__9CViewRootFUl(id);

    if (retailOut != expected || decompOut != expected || retailOut != decompOut) {
        fail(result, name);
        return;
    }
    result->passed++;
}

static void run_singleton_null(volatile BehaviourResult* result) {
    void* retailOut;
    void* decompOut;

    cviewroot_mock_reset();
    rb_lbl_eu_806655D0 = 0;
    dc_lbl_eu_806655D0 = 0;

    retailOut = rb_getView__9CViewRootFUl(1u);
    decompOut = dc_getView__9CViewRootFUl(1u);
    if (retailOut != 0 || decompOut != 0) {
        fail(result, "singleton_null");
        return;
    }
    result->passed++;
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;

    behaviour_result_init(result);

    run_singleton_null(result);
    run_scenario(result, "empty_children_skips_lookup", 2u, 0, 1, CVIEWROOT_THREAD_CVIEW, 0);
    run_scenario(result, "missing_work_thread", 3u, 2, -1, 0, 0);
    run_scenario(result, "cview_min_type", 4u, 1, 4, CVIEWROOT_THREAD_CVIEW, &g_cviewroot_threads[4]);
    run_scenario(result, "cview_max_minus_one", 5u, 1, 5, CVIEWROOT_THREAD_CVIEW_MAX - 1,
                 &g_cviewroot_threads[5]);
    run_scenario(result, "below_cview_range", 6u, 1, 6, CVIEWROOT_THREAD_CVIEW - 1, 0);
    run_scenario(result, "at_cview_max", 7u, 1, 7, CVIEWROOT_THREAD_CVIEW_MAX, 0);
    run_scenario(result, "above_cview_max", 1u, 1, 1, CVIEWROOT_THREAD_CVIEW_MAX + 3, 0);
    run_scenario(result, "invalid_work_id", CVIEWROOT_INVALID_WORK_ID, 1, -1, CVIEWROOT_THREAD_CVIEW, 0);
    run_scenario(result, "walk_two_nodes_then_hit", 2u, 2, 2, CVIEWROOT_THREAD_CVIEW,
                 &g_cviewroot_threads[2]);
    run_scenario(result, "walk_three_nodes_miss", 3u, 3, -1, 0, 0);
    run_scenario(result, "proc_type_rejected", 4u, 1, 4, 24, 0);
    run_scenario(result, "desktop_type_rejected", 5u, 1, 5, 47, 0);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cviewroot-getview", result);
    return (int)result->exit_code;
}
