#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "cview_mock.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);
extern void cview_init_jumptables(void);

extern void* ppc_rb_getCurrentView(void);
extern void* ppc_dc_getCurrentView(void);

extern void* rb_lbl_eu_806655C8;
extern void* dc_lbl_eu_806655C8;

static u8 sRetailView[0x470];
static u8 sDecompView[0x470];
static u8 sRetailPtr[0x8];
static u8 sDecompPtr[0x8];

static void fail(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void run_scenario(volatile BehaviourResult* result, const char* name, void* retailPtr, void* decompPtr) {
    void* retailOut;
    void* decompOut;

    rb_lbl_eu_806655C8 = retailPtr;
    dc_lbl_eu_806655C8 = decompPtr;
    retailOut = ppc_rb_getCurrentView();
    decompOut = ppc_dc_getCurrentView();

    if (retailOut != retailPtr || decompOut != decompPtr) {
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

    run_scenario(result, "null_ptr", 0, 0);
    run_scenario(result, "retail_view", sRetailView, sDecompView);
    run_scenario(result, "scratch_ptr", sRetailPtr, sDecompPtr);

    for (i = 0; i < 8; ++i) {
        static const char* names[] = {
            "slot_0", "slot_1", "slot_2", "slot_3", "slot_4", "slot_5", "slot_6", "slot_7",
        };
        rb_lbl_eu_806655C8 = sRetailView + i * 0x10;
        dc_lbl_eu_806655C8 = sDecompView + i * 0x10;
        if (ppc_rb_getCurrentView() != rb_lbl_eu_806655C8 || ppc_dc_getCurrentView() != dc_lbl_eu_806655C8) {
            fail(result, names[i]);
        } else {
            result->passed++;
        }
    }

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("cview-get-current-view", result);
    return (int)result->exit_code;
}
