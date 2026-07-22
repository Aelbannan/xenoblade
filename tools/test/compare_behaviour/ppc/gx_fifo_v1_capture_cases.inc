/* Generated fixture data for tools/test/compare_behaviour/ppc/gx_fifo_v1_capture.c.
 * Mirrors tools/ppc_equivalence/corpora/gx_fifo_v1/write_trace.jsonl's
 * width/alignment/repeat themes. Legal generated fixture only — never a
 * retail binary or disc asset (see AGENTS.md "Do not"). */
#ifndef GX_FIFO_V1_CAPTURE_CASES_INC
#define GX_FIFO_V1_CAPTURE_CASES_INC

typedef struct GxFifoV1CaptureCase {
    const char* id;
    u32 offset; /* byte offset from WGPIPE base (0xCC008000) */
    u32 width;  /* 1, 2, or 4 */
    u32 value;  /* masked to width at store time */
} GxFifoV1CaptureCase;

#define GX_FIFO_V1_CAPTURE_COUNT 6

static const GxFifoV1CaptureCase g_gx_fifo_v1_capture_cases[GX_FIFO_V1_CAPTURE_COUNT] = {
    {"write-width1-single", 0x00, 1, 0xAB},
    {"write-width2-single", 0x04, 2, 0xBEEF},
    {"write-width4-single", 0x08, 4, 0xDEADBEEF},
    {"write-width4-offset", 0x0C, 4, 0x12345678},
    {"write-width1-repeat-a", 0x00, 1, 0x01},
    {"write-width1-repeat-b", 0x00, 1, 0x02},
};

#endif
