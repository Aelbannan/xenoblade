#include <string.h>

#include <types.h>

#include "behaviour_result.h"
#include "scalar_fp_v2_capture_cases.inc"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

typedef void (*CapturePayloadFn)(void);

typedef struct CaptureLiveState {
    u64 fpr1;
    u64 fpr2;
    u64 fpr3;
    u64 fpr7;
    u64 fpscr_image;
    u32 cr;
    u32 pad;
} CaptureLiveState;

typedef struct CaptureObs {
    u32 magic;
    u32 pad;
    u64 result_fpr7;
    u32 post_fpscr;
    u32 post_cr;
} CaptureObs;

volatile CaptureObs g_scalar_fp_v2_capture_results[SCALAR_FP_V2_CAPTURE_COUNT];

static CaptureLiveState s_live;

static asm void capture_enter(CaptureLiveState* live, CapturePayloadFn payload) {
    nofralloc

    mflr r0
    stw r0, 4(r1)
    stwu r1, -64(r1)
    stw r2, 8(r1)
    stw r13, 12(r1)

    mr r5, r3
    mr r6, r4

    lfd f1, 0(r5)
    lfd f2, 8(r5)
    lfd f3, 16(r5)
    lfd f7, 24(r5)
    lfd f0, 32(r5)
    mtfsf 0xff, f0

    mtlr r6
    blrl

    stfd f7, 24(r5)
    mffs f0
    stfd f0, 32(r5)
    mfcr r0
    stw r0, 40(r5)

    lwz r2, 8(r1)
    lwz r13, 12(r1)
    lwz r0, 68(r1)
    mtlr r0
    addi r1, r1, 64
    blr
}

static void run_capture_case(u32 index) {
    const ScalarFpV2CaptureCase* capture = &g_scalar_fp_v2_capture_cases[index];
    volatile CaptureObs* obs = &g_scalar_fp_v2_capture_results[index];

    memset(&s_live, 0, sizeof(s_live));
    s_live.fpr1 = capture->fpr1;
    s_live.fpr2 = capture->fpr2;
    s_live.fpr3 = capture->fpr3;
    s_live.fpr7 = capture->fpr7_pre;
    s_live.fpscr_image = capture->fpscr_pre;
    capture_enter(&s_live, capture->payload);

    obs->magic = 0xCAFE0000u | index;
    obs->pad = 0;
    obs->result_fpr7 = s_live.fpr7;
    obs->post_fpscr = (u32)s_live.fpscr_image;
    obs->post_cr = s_live.cr;
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    u32 index;

    behaviour_result_init(result);
    for (index = 0; index < SCALAR_FP_V2_CAPTURE_COUNT; ++index) {
        run_capture_case(index);
        result->passed++;
    }

    result->exit_code = 0;
    behaviour_emit_summary("scalar-fp-v2-capture", result);
    return 0;
}
