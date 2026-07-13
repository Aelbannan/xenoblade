#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "mtrand_common.h"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

extern MTRand* rb_getInstance__Q22ml6MTRandFv(void);
extern MTRand* dc_getInstance__Q22ml6MTRandFv(void);

static MTRand* s_retail;
static MTRand* s_decomp;

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

static void run_first_init(volatile BehaviourResult* result) {
    s_retail = rb_getInstance__Q22ml6MTRandFv();
    s_decomp = dc_getInstance__Q22ml6MTRandFv();

    if (s_retail->state[0] != MTRAND_SEED || s_decomp->state[0] != MTRAND_SEED
        || s_retail->state[1] != MTRAND_STATE1 || s_decomp->state[1] != MTRAND_STATE1 || s_retail->left != 1
        || s_decomp->left != 1 || !s_retail->initialized || !s_decomp->initialized
        || s_retail->pNext != s_retail->state || s_decomp->pNext != s_decomp->state) {
        fail_scenario(result, "first_init");
        return;
    }
    pass_scenario(result);
}

static void run_repeat_stable(volatile BehaviourResult* result) {
    MTRand* retail2 = rb_getInstance__Q22ml6MTRandFv();
    MTRand* decomp2 = dc_getInstance__Q22ml6MTRandFv();

    if (retail2 != s_retail || decomp2 != s_decomp) {
        fail_scenario(result, "repeat_stable");
        return;
    }
    pass_scenario(result);
}

static void run_no_reinit(volatile BehaviourResult* result) {
    s_retail->state[0] = 0xCAFEBABEu;
    s_decomp->state[0] = 0xCAFEBABEu;
    (void)rb_getInstance__Q22ml6MTRandFv();
    (void)dc_getInstance__Q22ml6MTRandFv();
    if (s_retail->state[0] != 0xCAFEBABEu || s_decomp->state[0] != 0xCAFEBABEu) {
        fail_scenario(result, "no_reinit");
        return;
    }
    pass_scenario(result);
}

static void run_state1_tail(volatile BehaviourResult* result) {
    if (s_retail->state[1] != MTRAND_STATE1 || s_decomp->state[1] != MTRAND_STATE1) {
        fail_scenario(result, "state1_tail");
        return;
    }
    pass_scenario(result);
}

static void run_left_one(volatile BehaviourResult* result) {
    if (s_retail->left != 1 || s_decomp->left != 1) {
        fail_scenario(result, "left_one");
        return;
    }
    pass_scenario(result);
}

static void run_pnext_state(volatile BehaviourResult* result) {
    if (s_retail->pNext != s_retail->state || s_decomp->pNext != s_decomp->state) {
        fail_scenario(result, "pnext_state");
        return;
    }
    pass_scenario(result);
}

static void run_five_calls(volatile BehaviourResult* result) {
    MTRand* last_retail = s_retail;
    MTRand* last_decomp = s_decomp;
    int i;

    for (i = 0; i < 5; ++i) {
        last_retail = rb_getInstance__Q22ml6MTRandFv();
        last_decomp = dc_getInstance__Q22ml6MTRandFv();
    }

    if (last_retail != s_retail || last_decomp != s_decomp) {
        fail_scenario(result, "five_calls");
        return;
    }
    pass_scenario(result);
}

static void run_seed_preserved(volatile BehaviourResult* result) {
    if (s_retail->state[0] != 0xCAFEBABEu || s_decomp->state[0] != 0xCAFEBABEu) {
        fail_scenario(result, "seed_preserved");
        return;
    }
    pass_scenario(result);
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;

    behaviour_result_init(result);
    mtrand_init_lbl_constants();

    run_first_init(result);
    run_repeat_stable(result);
    run_state1_tail(result);
    run_left_one(result);
    run_pnext_state(result);
    run_no_reinit(result);
    run_seed_preserved(result);
    run_five_calls(result);

    result->exit_code = result->failed == 0 ? 0 : 1;
    behaviour_emit_summary("mtrand-getinstance", result);
    return (int)result->exit_code;
}
