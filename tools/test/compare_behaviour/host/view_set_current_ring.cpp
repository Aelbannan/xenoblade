#include <cstdint>
#include <cstdio>
#include <cstring>

#include <types.h>

// Isolated semantics of CView::setCurrent() from retail asm (8043F3D8).
// Caller passes a 0x24-byte snapshot via the stack; the view writes a tag into
// the ring slot at (base + byteOffset) but stores the snapshot at fixed base+4.

static void store_word32(u8* dst, u32 value) {
    const auto word = static_cast<uint32_t>(value);
    std::memcpy(dst, &word, sizeof(word));
}

static u32 load_word32(const u8* src) {
    uint32_t word;
    std::memcpy(&word, src, sizeof(word));
    return word;
}

struct CallerCtx {
    u32 w0;
    u32 w1;
    u32 w2;
    u32 w3;
    u32 w4;
    u32 w5;
    u32 w6;
    s16 half;
    u8 byte;
};

struct ViewRing {
    u32 unk3F0;
    u32 writeIdx;
    u32 capacity;
    u32 unk3FC;
    u8 buffer[0x24 * 64];
    u8* base;
};

struct SetCurrentOutcome {
    u32 slotTag;
    u8 snapshot[0x20];
    u32 writeIdx;
    u32 unk3FC;
};

static void init_ring(ViewRing& ring, u32 unk3F0, u32 writeIdx, u32 capacity) {
    std::memset(&ring, 0, sizeof(ring));
    ring.base = ring.buffer;
    ring.unk3F0 = unk3F0;
    ring.writeIdx = writeIdx;
    ring.capacity = capacity;
    for (u32 i = 0; i < sizeof(ring.buffer); ++i) {
        ring.buffer[i] = 0xA5;
    }
}

static void retail_set_current(ViewRing& ring, const CallerCtx& ctx, SetCurrentOutcome& out) {
    const u32 sum = ring.unk3F0 + ring.writeIdx;
    const s32 slotIndex = (s32)(sum / ring.capacity);
    const u32 byteOff = (sum - (u32)(slotIndex * ring.capacity)) * 0x24u;

    store_word32(ring.base + byteOff, 6);
    store_word32(ring.base + 0x4, ctx.w0);
    store_word32(ring.base + 0x8, ctx.w1);
    store_word32(ring.base + 0xC, ctx.w2);
    store_word32(ring.base + 0x10, ctx.w3);
    store_word32(ring.base + 0x14, ctx.w4);
    store_word32(ring.base + 0x18, ctx.w5);
    store_word32(ring.base + 0x1C, ctx.w6);
    std::memcpy(ring.base + 0x20, &ctx.half, sizeof(ctx.half));
    ring.base[0x22] = ctx.byte;
    ring.base[0x23] = 0;

    ring.writeIdx += 1;
    ring.unk3FC = ring.writeIdx - 1;

    out.slotTag = load_word32(ring.base + byteOff);
    std::memcpy(out.snapshot, ring.base + 0x4, sizeof(out.snapshot));
    out.writeIdx = ring.writeIdx;
    out.unk3FC = ring.unk3FC;
}

// Keep in sync with libs/monolib/src/core/CView.cpp when asm is replaced.
static void decomp_set_current(ViewRing& ring, const CallerCtx& ctx, SetCurrentOutcome& out) {
    retail_set_current(ring, ctx, out);
}

static bool outcomes_equal(const SetCurrentOutcome& a, const SetCurrentOutcome& b) {
    return a.slotTag == b.slotTag && a.writeIdx == b.writeIdx && a.unk3FC == b.unk3FC
        && std::memcmp(a.snapshot, b.snapshot, sizeof(a.snapshot)) == 0;
}

static void run_scenario(const char* name, u32 unk3F0, u32 writeIdx, u32 capacity, const CallerCtx& ctx,
                         u32 expectWriteIdx, u32 expectUnk3FC) {
    ViewRing retailRing{};
    ViewRing decompRing{};
    init_ring(retailRing, unk3F0, writeIdx, capacity);
    init_ring(decompRing, unk3F0, writeIdx, capacity);

    SetCurrentOutcome retailOut{};
    SetCurrentOutcome decompOut{};
    retail_set_current(retailRing, ctx, retailOut);
    decomp_set_current(decompRing, ctx, decompOut);

    if (!outcomes_equal(retailOut, decompOut)) {
        std::printf("FAIL %s outcome mismatch\n", name);
        std::exit(1);
    }
    if (retailOut.writeIdx != expectWriteIdx || retailOut.unk3FC != expectUnk3FC || retailOut.slotTag != 6u) {
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static CallerCtx make_ctx(u32 w0, u32 w1, u32 w2, u32 w3, u32 w4, u32 w5, u32 w6, s16 half, u8 byte) {
    return CallerCtx{w0, w1, w2, w3, w4, w5, w6, half, byte};
}

int main() {
    run_scenario("zero_ring", 0, 0, 4, make_ctx(1, 2, 3, 4, 5, 6, 7, 8, 9), 1, 0);
    run_scenario("write_idx_one", 0, 1, 4, make_ctx(0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, -1, 0xFF), 2,
                 1);
    run_scenario("unk3f0_offset", 3, 2, 5, make_ctx(0x20, 0, 0, 0, 0, 0, 0, 0, 0), 3, 2);
    run_scenario("capacity_wrap", 0, 3, 3, make_ctx(0x30, 0x31, 0, 0, 0, 0, 0, 1, 2), 4, 3);
    run_scenario("signed_div_negative_sum", 0, 7, 8, make_ctx(0x40, 0x41, 0x42, 0, 0, 0, 0, -5, 7), 8, 7);
    run_scenario("max_half", 1, 0, 2, make_ctx(0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x7FFF, 0), 1, 0);
    run_scenario("min_half", 0, 5, 6, make_ctx(0x60, 0, 0, 0, 0, 0, 0, (s16)0x8000, 0x80), 6, 5);
    run_scenario("byte_only", 2, 1, 3, make_ctx(0, 0, 0, 0, 0, 0, 0, 0, 0xAB), 2, 1);
    run_scenario("all_ones", 0, 0, 1, make_ctx(0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu, 0xFFFFFFFFu,
                                               0xFFFFFFFFu, 0xFFFFFFFFu, (s16)0xFFFF, 0xFF),
                 1, 0);
    run_scenario("large_capacity", 100, 200, 64, make_ctx(0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0, 0, 42, 3), 201, 200);
    run_scenario("cap1_idx0", 0, 0, 1, make_ctx(1, 1, 0, 0, 0, 0, 0, 0, 1), 1, 0);
    run_scenario("cap2_idx0", 0, 0, 2, make_ctx(2, 0, 2, 0, 0, 0, 0, 0, 2), 1, 0);
    run_scenario("cap2_idx1", 0, 1, 2, make_ctx(2, 1, 3, 0, 0, 0, 0, 1, 2), 2, 1);
    run_scenario("cap3_idx0", 0, 0, 3, make_ctx(3, 0, 3, 0, 0, 0, 0, 0, 3), 1, 0);
    run_scenario("cap3_idx1", 0, 1, 3, make_ctx(3, 1, 2, 0, 0, 0, 0, 1, 3), 2, 1);
    run_scenario("cap3_idx2", 0, 2, 3, make_ctx(3, 2, 1, 0, 0, 0, 0, 2, 3), 3, 2);
    run_scenario("cap4_idx0", 0, 0, 4, make_ctx(4, 0, 4, 0, 0, 0, 0, 0, 4), 1, 0);
    run_scenario("cap4_idx3", 0, 3, 4, make_ctx(4, 3, 7, 0, 0, 0, 0, 3, 4), 4, 3);
    run_scenario("cap5_idx2", 0, 2, 5, make_ctx(5, 2, 7, 0, 0, 0, 0, 2, 5), 3, 2);
    run_scenario("cap5_idx4", 0, 4, 5, make_ctx(5, 4, 1, 0, 0, 0, 0, 4, 5), 5, 4);
    run_scenario("cap8_idx0", 0, 0, 8, make_ctx(8, 0, 8, 0, 0, 0, 0, 0, 8), 1, 0);
    run_scenario("cap8_idx7", 0, 7, 8, make_ctx(8, 7, 15, 0, 0, 0, 0, 7, 8), 8, 7);
    run_scenario("unk3f0_0", 0, 4, 5, make_ctx(0, 0, 0, 0, 0, 0, 0, 0, 0), 5, 4);
    run_scenario("unk3f0_1", 1, 4, 5, make_ctx(3, 5, 0, 0, 0, 0, 0, 1, 1), 5, 4);
    run_scenario("unk3f0_2", 2, 4, 5, make_ctx(6, 10, 0, 0, 0, 0, 0, 2, 2), 5, 4);
    run_scenario("unk3f0_3", 3, 4, 5, make_ctx(9, 15, 0, 0, 0, 0, 0, 3, 3), 5, 4);
    run_scenario("unk3f0_4", 4, 4, 5, make_ctx(12, 20, 0, 0, 0, 0, 0, 4, 4), 5, 4);
    run_scenario("unk3f0_5", 5, 4, 5, make_ctx(15, 25, 0, 0, 0, 0, 0, 5, 5), 5, 4);
    run_scenario("wrap_sum_exact", 2, 2, 4, make_ctx(0x70, 0x71, 0x72, 0, 0, 0, 0, 9, 10), 3, 2);
    run_scenario("wrap_sum_over", 3, 3, 4, make_ctx(0x80, 0x81, 0x82, 0, 0, 0, 0, 11, 12), 4, 3);
    run_scenario("half_neg1", 0, 0, 4, make_ctx(1, 2, 3, 4, 5, 6, 7, -1, 0), 1, 0);
    run_scenario("byte_ff", 0, 2, 4, make_ctx(0, 0, 0, 0, 0, 0, 0, 0, 0xFF), 3, 2);

    std::printf("host view_set_current_ring: all scenarios passed\n");
    return 0;
}
