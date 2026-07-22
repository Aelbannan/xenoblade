/* GX FIFO v1 Dolphin capture scaffold harness.
 *
 * See tools/ppc_equivalence/corpora/gx_fifo_v1/dolphin_capture_plan.md.
 * Not yet wired into manifest.json / not yet run under Dolphin in this
 * environment (Dolphin must be launched outside the restricted process —
 * see AGENTS.md). This source exists so the capture plan is concrete and
 * reviewable even before a live run.
 *
 * For each table-driven case (gx_fifo_v1_capture_cases.inc) this performs
 * exactly one aligned store of the declared width to
 * WGPIPE_BASE + offset and records a small observation so a future GDB-stub
 * read (mirroring scalar_fp_v2_capture.py's run_dolphin_capture) can confirm
 * every case completed without a CPU exception. It intentionally makes no
 * claim about FIFO-consumer-side readback (see the capture plan's "What
 * independent evidence is (and is not) obtainable here").
 */

#include <types.h>

#include "behaviour_result.h"
#include "gx_fifo_v1_capture_cases.inc"

extern void behaviour_emit_summary(const char* test_id, volatile BehaviourResult* result);

#define GX_FIFO_V1_WGPIPE_BASE 0xCC008000u

typedef struct GxFifoV1CaptureObs {
    u32 magic;
    u32 completed;
} GxFifoV1CaptureObs;

volatile GxFifoV1CaptureObs g_gx_fifo_v1_capture_results[GX_FIFO_V1_CAPTURE_COUNT];

static void run_capture_case(u32 index) {
    const GxFifoV1CaptureCase* capture = &g_gx_fifo_v1_capture_cases[index];
    volatile GxFifoV1CaptureObs* obs = &g_gx_fifo_v1_capture_results[index];
    u32 addr = GX_FIFO_V1_WGPIPE_BASE + capture->offset;

    switch (capture->width) {
        case 1:
            *(volatile u8*)addr = (u8)capture->value;
            break;
        case 2:
            *(volatile u16*)addr = (u16)capture->value;
            break;
        case 4:
        default:
            *(volatile u32*)addr = (u32)capture->value;
            break;
    }

    obs->magic = 0xFEED0000u | index;
    obs->completed = 1;
}

int behaviour_main(void) {
    volatile BehaviourResult* result = &g_behaviour_result;
    u32 index;

    behaviour_result_init(result);
    for (index = 0; index < GX_FIFO_V1_CAPTURE_COUNT; ++index) {
        run_capture_case(index);
        result->passed++;
    }

    result->exit_code = 0;
    behaviour_emit_summary("gx-fifo-v1-capture", result);
    return 0;
}
