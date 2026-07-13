#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "mtrand_common.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

extern void rb_srand__Q22ml6MTRandFUl(MTRand* self, u32 seed);
extern void dc_srand__Q22ml6MTRandFUl(MTRand* self, u32 seed);
extern u32 rb_rand31__Q22ml6MTRandFv(MTRand* self);
extern u32 dc_rand31__Q22ml6MTRandFv(MTRand* self);

static void fail_scenario(volatile BehaviourResult* result, const char* name) {
    int i;

    result->failed++;
    behaviour_log_fail(name);
    for (i = 0; i < 63 && name[i] != '\0'; ++i) {
        result->last_fail[i] = name[i];
    }
    result->last_fail[63] = '\0';
}

static void pass_scenario(volatile BehaviourResult* result) {
    result->passed++;
}

static void run_scenario(volatile BehaviourResult* result, const char* name, u32 seed, int draws) {
    MTRand retail;
    MTRand decomp;
    int i;

    memset(&retail, 0, sizeof(retail));
    memset(&decomp, 0, sizeof(decomp));
    rb_srand__Q22ml6MTRandFUl(&retail, seed);
    dc_srand__Q22ml6MTRandFUl(&decomp, seed);

    for (i = 0; i < draws; ++i) {
        u32 r = rb_rand31__Q22ml6MTRandFv(&retail);
        u32 d = dc_rand31__Q22ml6MTRandFv(&decomp);
        if (r != d) {
            fail_scenario(result, name);
            return;
        }
    }
    pass_scenario(result);
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;

    behaviour_result_init(result);
    mtrand_init_lbl_constants();

    run_scenario(result, "seed_default", 0x012BD6AAu, 32);
    run_scenario(result, "seed_zero_maps", 0u, 16);
    run_scenario(result, "seed_one", 1u, 64);
    run_scenario(result, "seed_1571", 0x1571u, 64);
    run_scenario(result, "seed_deadbeef", 0xDEADBEEFu, 128);
    run_scenario(result, "long_run", 0x12345678u, 700);
    run_scenario(result, "seed_2", 2u, 40);
    run_scenario(result, "seed_3", 3u, 40);
    run_scenario(result, "seed_4", 4u, 40);
    run_scenario(result, "seed_5", 5u, 40);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("mtrand-integer-rng", result);
    return (int)result->exit_code;
}
