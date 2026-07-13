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

typedef struct CPnt16 {
    s16 x;
    s16 y;
} CPnt16;

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

extern void rb_func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* self, const CPnt16* size);
extern void dc_func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(CViewRectDataCore* self, const CPnt16* size);

static void run_scenario(volatile BehaviourResult* result, const char* name, s16 x0, s16 y0, s16 x4, s16 y4,
                         s16 mx, s16 my) {
    CViewRectDataCore retail;
    CViewRectDataCore decomp;
    CPnt16 maxSize;
    int i;

    memset(&retail, 0, sizeof(retail));
    memset(&decomp, 0, sizeof(decomp));
    retail.unk0 = x0;
    retail.unk2 = y0;
    retail.unk4 = x4;
    retail.unk6 = y4;
    decomp = retail;
    maxSize.x = mx;
    maxSize.y = my;

    rb_func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(&retail, &maxSize);
    dc_func_80459384__17CViewRectDataCoreFRCQ22ml6CPnt16(&decomp, &maxSize);

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
    int i;

    behaviour_result_init(result);

    run_scenario(result, "store_basic", 10, 20, 100, 200, 640, 480);
    run_scenario(result, "store_zero", 0, 0, 0, 0, 0, 0);
    run_scenario(result, "store_negative", -1, -2, 50, 60, -10, -20);
    run_scenario(result, "store_max", 100, 100, 200, 200, 32767, 32767);
    run_scenario(result, "store_min", -100, -100, 0, 0, -32768, -32768);
    for (i = 0; i < 6; ++i) {
        static const char* names[] = {
            "store_iter_0", "store_iter_1", "store_iter_2",
            "store_iter_3", "store_iter_4", "store_iter_5",
        };
        run_scenario(result, names[i], (s16)i, (s16)(i * 2), (s16)(100 + i), (s16)(200 + i), (s16)(300 + i * 3),
                     (s16)(400 + i * 5));
    }

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("view-rect-data-store", result);
    return (int)result->exit_code;
}
