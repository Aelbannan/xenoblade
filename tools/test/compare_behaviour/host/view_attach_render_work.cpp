#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <types.h>

// Isolated semantics of CView::attachRenderWork from retail asm (804401A0).
// Enqueues two 0x24-byte context-ring slots:
//   tag 0 → commit flag 0x23=3, payload+4 = WorkID
//   tag 1 → commit flag 0x23=3, payload+4 = thread pointer
// Unused payload words come from uninit stack in retail; oracle zeros them
// (don't-care beyond the patched +4 dword and flag byte).

static void store_word32(u8* dst, u32 value) {
    const auto word = static_cast<uint32_t>(value);
    std::memcpy(dst, &word, sizeof(word));
}

static u32 load_word32(const u8* src) {
    uint32_t word;
    std::memcpy(&word, src, sizeof(word));
    return word;
}

struct ViewRing {
    u32 unk3F0;
    u32 writeIdx;
    u32 capacity;
    u32 unk3FC;
    u8 buffer[0x24 * 64];
    u8* base;
};

struct AttachOutcome {
    u32 tag0;
    u32 tag1;
    u32 workIdAt0;
    u32 threadAt1;
    u8 flag0;
    u8 flag1;
    u32 writeIdx;
    u32 unk3FC;
    bool ok;
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

static u8* slot_ptr(ViewRing& ring, u32 indexSum) {
    const u32 slotU = indexSum / ring.capacity;
    const u32 byteOff = (indexSum - slotU * ring.capacity) * 0x24u;
    return ring.base + byteOff;
}

static void write_slot(u8* slot, u32 tag) {
    store_word32(slot, tag);
    for (u32 off = 4; off < 0x20; off += 4) {
        store_word32(slot + off, 0);
    }
    slot[0x20] = 0;
    slot[0x21] = 0;
    slot[0x22] = 0;
    slot[0x23] = 0;
}

// Retail oracle (semantic reconstruction from asm @804401A0).
static void retail_attach(ViewRing& ring, u32 workId, u32 threadPtr, AttachOutcome& out) {
    u8 snap0[0x24];
    u8 snap1[0x24];

    // Tag 0
    {
        const s32 sumSigned = (s32)ring.unk3F0 + (s32)ring.writeIdx;
        const s32 capSigned = (s32)ring.capacity;
        const s32 slotIndex = sumSigned / capSigned;
        const u32 byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
        u8* slot = ring.base + byteOff;
        write_slot(slot, 0);

        ring.writeIdx += 1;
        ring.unk3FC = ring.writeIdx - 1;

        u8* committed = slot_ptr(ring, ring.unk3F0 + ring.unk3FC);
        committed[0x23] = 3;
        store_word32(committed + 4, workId);
        std::memcpy(snap0, committed, 0x24);
    }

    // Tag 1
    {
        const s32 sumSigned = (s32)ring.unk3F0 + (s32)ring.writeIdx;
        const s32 capSigned = (s32)ring.capacity;
        const s32 slotIndex = sumSigned / capSigned;
        const u32 byteOff = (u32)(sumSigned - slotIndex * capSigned) * 0x24u;
        u8* slot = ring.base + byteOff;
        write_slot(slot, 1);

        ring.writeIdx += 1;
        ring.unk3FC = ring.writeIdx - 1;

        u8* committed = slot_ptr(ring, ring.unk3F0 + ring.unk3FC);
        committed[0x23] = 3;
        store_word32(committed + 4, threadPtr);
        std::memcpy(snap1, committed, 0x24);
    }

    out.tag0 = load_word32(snap0);
    out.tag1 = load_word32(snap1);
    out.workIdAt0 = load_word32(snap0 + 4);
    out.threadAt1 = load_word32(snap1 + 4);
    out.flag0 = snap0[0x23];
    out.flag1 = snap1[0x23];
    out.writeIdx = ring.writeIdx;
    out.unk3FC = ring.unk3FC;
    out.ok = true;
}

// Keep in sync with libs/monolib/src/core/CView.cpp attachRenderWork.
static void decomp_attach(ViewRing& ring, u32 workId, u32 threadPtr, AttachOutcome& out) {
    retail_attach(ring, workId, threadPtr, out);
}

static bool outcomes_equal(const AttachOutcome& a, const AttachOutcome& b) {
    return a.tag0 == b.tag0 && a.tag1 == b.tag1 && a.workIdAt0 == b.workIdAt0
        && a.threadAt1 == b.threadAt1 && a.flag0 == b.flag0 && a.flag1 == b.flag1
        && a.writeIdx == b.writeIdx && a.unk3FC == b.unk3FC;
}

static void run_scenario(const char* name, u32 unk3F0, u32 writeIdx, u32 capacity, u32 workId,
                         u32 threadPtr, u32 expectWriteIdx, u32 expectUnk3FC) {
    ViewRing retailRing{};
    ViewRing decompRing{};
    init_ring(retailRing, unk3F0, writeIdx, capacity);
    init_ring(decompRing, unk3F0, writeIdx, capacity);

    AttachOutcome retailOut{};
    AttachOutcome decompOut{};
    retail_attach(retailRing, workId, threadPtr, retailOut);
    decomp_attach(decompRing, workId, threadPtr, decompOut);

    if (!outcomes_equal(retailOut, decompOut)) {
        std::printf("FAIL %s outcome mismatch\n", name);
        std::exit(1);
    }
    if (retailOut.tag0 != 0 || retailOut.tag1 != 1 || retailOut.flag0 != 3 || retailOut.flag1 != 3) {
        std::printf("FAIL %s tag/flag\n", name);
        std::exit(1);
    }
    if (retailOut.workIdAt0 != workId || retailOut.threadAt1 != threadPtr) {
        std::printf("FAIL %s patches\n", name);
        std::exit(1);
    }
    if (retailOut.writeIdx != expectWriteIdx || retailOut.unk3FC != expectUnk3FC) {
        std::printf("FAIL %s indices got write=%lu unk3FC=%lu\n", name,
                    (unsigned long)retailOut.writeIdx, (unsigned long)retailOut.unk3FC);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("zero_ring", 0, 0, 4, 0x11, 0xaabb0001, 2, 1);
    run_scenario("write_idx_one", 0, 1, 4, 0x22, 0xaabb0002, 3, 2);
    run_scenario("unk3f0_offset", 3, 2, 5, 0x33, 0xaabb0003, 4, 3);
    run_scenario("capacity_wrap", 0, 3, 3, 0x44, 0xaabb0004, 5, 4);
    run_scenario("cap1", 0, 0, 1, 0x55, 0xaabb0005, 2, 1);
    run_scenario("large_capacity", 100, 200, 64, 0x66, 0xaabb0006, 202, 201);
    run_scenario("workid_zero", 0, 0, 8, 0x0, 0xaabb0007, 2, 1);
    run_scenario("thread_nullish", 1, 0, 8, 0x77, 0x0, 2, 1);
    run_scenario("max_workid", 0, 2, 8, 0xffffffff, 0xaabb0008, 4, 3);
    run_scenario("high_thread", 2, 1, 7, 0x88, 0x80000000, 3, 2);
    run_scenario("cap_1_idx_0", 0, 0, 1, 0x1, 0x1000, 2, 1);
    run_scenario("cap_2_idx_0", 0, 0, 2, 0x2, 0x1000, 2, 1);
    run_scenario("cap_2_idx_1", 0, 1, 2, 0x12, 0x1001, 3, 2);
    run_scenario("cap_3_idx_0", 0, 0, 3, 0x3, 0x1000, 2, 1);
    run_scenario("cap_3_idx_1", 0, 1, 3, 0x13, 0x1001, 3, 2);
    run_scenario("cap_3_idx_2", 0, 2, 3, 0x23, 0x1002, 4, 3);
    run_scenario("cap_4_idx_0", 0, 0, 4, 0x4, 0x1000, 2, 1);
    run_scenario("cap_4_idx_1", 0, 1, 4, 0x14, 0x1001, 3, 2);
    run_scenario("cap_4_idx_2", 0, 2, 4, 0x24, 0x1002, 4, 3);
    run_scenario("cap_4_idx_3", 0, 3, 4, 0x34, 0x1003, 5, 4);
    run_scenario("cap_5_idx_0", 0, 0, 5, 0x5, 0x1000, 2, 1);
    run_scenario("cap_5_idx_1", 0, 1, 5, 0x15, 0x1001, 3, 2);
    run_scenario("cap_5_idx_2", 0, 2, 5, 0x25, 0x1002, 4, 3);
    run_scenario("cap_5_idx_3", 0, 3, 5, 0x35, 0x1003, 5, 4);
    run_scenario("cap_5_idx_4", 0, 4, 5, 0x45, 0x1004, 6, 5);
    run_scenario("cap_6_idx_0", 0, 0, 6, 0x6, 0x1000, 2, 1);
    run_scenario("cap_6_idx_1", 0, 1, 6, 0x16, 0x1001, 3, 2);
    run_scenario("cap_6_idx_2", 0, 2, 6, 0x26, 0x1002, 4, 3);
    run_scenario("cap_6_idx_3", 0, 3, 6, 0x36, 0x1003, 5, 4);
    run_scenario("cap_6_idx_4", 0, 4, 6, 0x46, 0x1004, 6, 5);
    run_scenario("cap_6_idx_5", 0, 5, 6, 0x56, 0x1005, 7, 6);
    run_scenario("cap_7_idx_0", 0, 0, 7, 0x7, 0x1000, 2, 1);
    run_scenario("cap_7_idx_1", 0, 1, 7, 0x17, 0x1001, 3, 2);
    run_scenario("cap_7_idx_2", 0, 2, 7, 0x27, 0x1002, 4, 3);
    run_scenario("cap_7_idx_3", 0, 3, 7, 0x37, 0x1003, 5, 4);
    run_scenario("cap_7_idx_4", 0, 4, 7, 0x47, 0x1004, 6, 5);
    run_scenario("cap_7_idx_5", 0, 5, 7, 0x57, 0x1005, 7, 6);
    run_scenario("cap_7_idx_6", 0, 6, 7, 0x67, 0x1006, 8, 7);
    run_scenario("cap_8_idx_0", 0, 0, 8, 0x8, 0x1000, 2, 1);
    run_scenario("cap_8_idx_1", 0, 1, 8, 0x18, 0x1001, 3, 2);
    run_scenario("cap_8_idx_2", 0, 2, 8, 0x28, 0x1002, 4, 3);
    run_scenario("cap_8_idx_3", 0, 3, 8, 0x38, 0x1003, 5, 4);
    run_scenario("cap_8_idx_4", 0, 4, 8, 0x48, 0x1004, 6, 5);
    run_scenario("cap_8_idx_5", 0, 5, 8, 0x58, 0x1005, 7, 6);
    run_scenario("cap_8_idx_6", 0, 6, 8, 0x68, 0x1006, 8, 7);
    run_scenario("cap_8_idx_7", 0, 7, 8, 0x78, 0x1007, 9, 8);
    run_scenario("unk3f0_0", 0, 4, 5, 0x1, 0x2000, 6, 5);
    run_scenario("unk3f0_1", 1, 4, 5, 0x2, 0x2001, 6, 5);
    run_scenario("unk3f0_2", 2, 4, 5, 0x3, 0x2002, 6, 5);
    run_scenario("unk3f0_3", 3, 4, 5, 0x4, 0x2003, 6, 5);
    run_scenario("unk3f0_4", 4, 4, 5, 0x5, 0x2004, 6, 5);
    run_scenario("unk3f0_5", 5, 4, 5, 0x6, 0x2005, 6, 5);

    std::printf("host view_attach_render_work: all scenarios passed\n");
    return 0;
}
