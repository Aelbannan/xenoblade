#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cview_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void cview_init_jumptables(void);

extern void ppc_rb_setSplitLine(void* view, s16 line);
extern void ppc_dc_setSplitLine(void* view, s16 line);

static u8 sRetailView[0x470];
static u8 sDecompView[0x470];
static u8 sSplitFrame[0x20];
static u8 sView1[0x10];
static u8 sView2[0x10];

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void setup_mock(void* split, CView* view1, CView* view2) {
    g_rb_cview_mock.splitFrame = split;
    g_rb_cview_mock.view1 = view1;
    g_rb_cview_mock.view2 = view2;
    g_rb_cview_mock.splitLine = 0;
    g_dc_cview_mock = g_rb_cview_mock;
}

static void run_scenario(volatile BehaviourResult* result, const char* name, void* split, CView* view1,
                         CView* view2, s16 line, s16 expected) {
    setup_mock(split, view1, view2);
    cview_set_unk45c((CView*)sRetailView, split);
    cview_set_unk45c((CView*)sDecompView, split);

    ppc_rb_setSplitLine(sRetailView, line);
    ppc_dc_setSplitLine(sDecompView, line);

    if (g_rb_cview_mock.splitLine != expected || g_dc_cview_mock.splitLine != expected
        || g_rb_cview_mock.splitLine != g_dc_cview_mock.splitLine) {
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

    run_scenario(result, "no_split", 0, 0, 0, 100, 0);
    run_scenario(result, "split_no_views", sSplitFrame, 0, 0, 50, 0);
    run_scenario(result, "split_view1_only", sSplitFrame, (CView*)sView1, 0, 80, 0);
    run_scenario(result, "split_both_views", sSplitFrame, (CView*)sView1, (CView*)sView2, 160, 160);
    run_scenario(result, "negative_line", sSplitFrame, (CView*)sView1, (CView*)sView2, -99, -99);
    run_scenario(result, "zero_line", sSplitFrame, (CView*)sView1, (CView*)sView2, 0, 0);
    run_scenario(result, "max_s16", sSplitFrame, (CView*)sView1, (CView*)sView2, 32767, 32767);
    run_scenario(result, "min_s16", sSplitFrame, (CView*)sView1, (CView*)sView2, -32768, -32768);

    for (i = 0; i < 8; ++i) {
        static const char* names[] = {
            "grid_0", "grid_1", "grid_2", "grid_3", "grid_4", "grid_5", "grid_6", "grid_7",
        };
        run_scenario(result, names[i], sSplitFrame, (CView*)sView1, (CView*)sView2, (s16)(i * 23),
                     (s16)(i * 23));
    }

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cview-set-split-line", result);
    return (int)result->exit_code;
}
