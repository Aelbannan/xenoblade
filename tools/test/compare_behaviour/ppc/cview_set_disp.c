#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cview_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void cview_init_jumptables(void);

extern void ppc_rb_setDisp(void* view, int disp, int invalidate);
extern void ppc_dc_setDisp(void* view, int disp, int invalidate);

static u8 sRetailView[0x470];
static u8 sDecompView[0x470];
static u8 sOtherView[0x470];

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void run_scenario(volatile BehaviourResult* result, const char* name, u32 seed, int disp, int invalidate,
                         CView* retailCurrent, CView* decompCurrent, u32 expectFlags, int expectInvalid,
                         CView* expectRetailInvalid, CView* expectDecompInvalid) {
    u32 retailFlags;
    u32 decompFlags;

    g_rb_cview_mock.rootCurrent = retailCurrent;
    g_dc_cview_mock.rootCurrent = decompCurrent;
    g_rb_cview_mock.invalidCount = 0;
    g_dc_cview_mock.invalidCount = 0;
    g_rb_cview_mock.invalidArg = 0;
    g_dc_cview_mock.invalidArg = 0;

    cview_set_unk278((CView*)sRetailView, seed);
    cview_set_unk278((CView*)sDecompView, seed);

    ppc_rb_setDisp(sRetailView, disp, invalidate);
    ppc_dc_setDisp(sDecompView, disp, invalidate);

    retailFlags = cview_get_unk278((CView*)sRetailView);
    decompFlags = cview_get_unk278((CView*)sDecompView);

    if (retailFlags != expectFlags || decompFlags != expectFlags || retailFlags != decompFlags) {
        fail(result, name);
        return;
    }
    if (g_rb_cview_mock.invalidCount != expectInvalid || g_dc_cview_mock.invalidCount != expectInvalid
        || g_rb_cview_mock.invalidArg != expectRetailInvalid
        || g_dc_cview_mock.invalidArg != expectDecompInvalid) {
        fail(result, name);
        return;
    }
    result->passed++;
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    int i;

    behaviour_result_init(result);
    cview_init_jumptables();
    cview_mock_reset();

    run_scenario(result, "hide_clear", 0, 0, 0, (CView*)sOtherView, (CView*)sOtherView, 0x40u, 0, 0, 0);
    run_scenario(result, "show_clear", 0x40u, 1, 0, (CView*)sOtherView, (CView*)sOtherView, 0u, 0, 0, 0);
    run_scenario(result, "hide_keep_bit", 0x41u, 0, 0, (CView*)sOtherView, (CView*)sOtherView, 0x41u, 0, 0, 0);
    run_scenario(result, "show_keep_bits", 0x12345678u, 1, 0, (CView*)sOtherView, (CView*)sOtherView, 0x12345638u, 0, 0,
                 0);
    run_scenario(result, "invalidate_other", 0, 0, 1, (CView*)sOtherView, (CView*)sOtherView, 0x40u, 0, 0, 0);
    run_scenario(result, "invalidate_self", 0, 0, 1, (CView*)sRetailView, (CView*)sDecompView, 0x40u, 1,
                 (CView*)sRetailView, (CView*)sDecompView);
    run_scenario(result, "invalidate_while_show", 0x10u, 1, 1, (CView*)sRetailView, (CView*)sDecompView, 0x10u, 0, 0,
                 0);
    run_scenario(result, "hide_no_invalidate", 0x55u, 0, 1, (CView*)sOtherView, (CView*)sOtherView, 0x55u, 0, 0, 0);

    for (i = 0; i < 8; ++i) {
        static const char* names[] = {
            "bit_0", "bit_1", "bit_2", "bit_3", "bit_4", "bit_5", "bit_6", "bit_7",
        };
        u32 seed = (u32)(0x11111111u * (u32)(i + 1));
        run_scenario(result, names[i], seed, 0, 0, (CView*)sOtherView, (CView*)sOtherView, seed | 0x40u, 0, 0, 0);
    }

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cview-set-disp", result);
    return (int)result->exit_code;
}
