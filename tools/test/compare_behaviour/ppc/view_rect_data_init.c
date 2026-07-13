#include <string.h>

#include <types.h>

#include "behaviour_result.h"

typedef struct CViewRectDataCore {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
} CViewRectDataCore;

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

extern void rb_func_80459270__17CViewRectDataCoreFv(CViewRectDataCore* self);
extern void dc_func_80459270__17CViewRectDataCoreFv(CViewRectDataCore* self);

static void run_scenario(volatile BehaviourResult* result, const char* name) {
    CViewRectDataCore retail;
    CViewRectDataCore decomp;
    int i;

    memset(&retail, 0, sizeof(retail));
    memset(&decomp, 0, sizeof(decomp));
    rb_func_80459270__17CViewRectDataCoreFv(&retail);
    dc_func_80459270__17CViewRectDataCoreFv(&decomp);

    if (memcmp(&retail, &decomp, sizeof(retail)) != 0) {
        result->failed++;
        behaviour_log_fail(name);
        for (i = 0; i < 63 && name[i] != '\0'; ++i) {
            result->last_fail[i] = name[i];
        }
        result->last_fail[63] = '\0';
        return;
    }
    result->passed++;
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;

    behaviour_result_init(result);

    run_scenario(result, "init_default");
    run_scenario(result, "init_repeat");
    run_scenario(result, "init_third");
    run_scenario(result, "init_fourth");
    run_scenario(result, "init_fifth");
    run_scenario(result, "init_sixth");
    run_scenario(result, "init_seventh");
    run_scenario(result, "init_eighth");

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("view-rect-data-init", result);
    return (int)result->exit_code;
}
