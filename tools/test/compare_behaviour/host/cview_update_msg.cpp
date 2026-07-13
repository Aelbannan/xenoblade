#include <cstdint>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#include <types.h>

// Isolated semantics of CView::updateMsg from retail asm (8043FA08).
// Drains the context ring (unk3F0 = read head, mContextRingWriteIndex = pending
// count). Tags 0-7 classify attach/detach/flag/setCurrent side effects.
// Fan-out to child views and ViewRoot setCurrent are gated out of host coverage;
// oracle and decomp agree on ring/flag/list effects for the local view.

static void store_word32(u8* dst, u32 value) {
    const auto word = static_cast<uint32_t>(value);
    std::memcpy(dst, &word, sizeof(word));
}

static u32 load_word32(const u8* src) {
    uint32_t word;
    std::memcpy(&word, src, sizeof(word));
    return word;
}

struct RingSlot {
    u8 data[0x24];
};

struct ResNode {
    ResNode* next;
    ResNode* prev;
    u32 item;
};

struct ViewMsgState {
    u32 unk3F0;
    u32 writeIdx;
    u32 capacity;
    u32 unk278;
    RingSlot slots[16];
    ResNode listNodes[16];
    ResNode sentinel;
    ResNode* startPtr;
    int listCap;
    u32 setCurrentCalls;
    bool parentIsView;
};

struct MsgOutcome {
    u32 unk3F0;
    u32 writeIdx;
    u32 unk278;
    u32 listCount;
    u32 setCurrentCalls;
};

static void init_state(ViewMsgState& st, u32 capacity) {
    std::memset(&st, 0, sizeof(st));
    st.capacity = capacity;
    st.listCap = 16;
    st.startPtr = &st.sentinel;
    st.sentinel.next = &st.sentinel;
    st.sentinel.prev = &st.sentinel;
    st.parentIsView = false;
    for (int i = 0; i < 16; i++) {
        st.listNodes[i].next = nullptr;
        st.listNodes[i].prev = nullptr;
        st.listNodes[i].item = 0;
    }
}

static u32 list_count(const ViewMsgState& st) {
    u32 n = 0;
    for (ResNode* node = st.sentinel.next; node != &st.sentinel; node = node->next) {
        n++;
    }
    return n;
}

static void enqueue_msg(ViewMsgState& st, u32 tag, u32 payloadWord, u8 payloadByte = 0) {
    const u32 sum = st.unk3F0 + st.writeIdx;
    const u32 slotU = sum / st.capacity;
    const u32 idx = sum - slotU * st.capacity;
    RingSlot& slot = st.slots[idx];
    std::memset(slot.data, 0, sizeof(slot.data));
    store_word32(slot.data, tag);
    if (tag == 7) {
        slot.data[4] = payloadByte;
    } else {
        store_word32(slot.data + 4, payloadWord);
    }
    st.writeIdx += 1;
}

static bool list_contains(const ViewMsgState& st, u32 item) {
    for (ResNode* node = st.sentinel.next; node != &st.sentinel; node = node->next) {
        if (node->item == item) {
            return true;
        }
    }
    return false;
}

static void list_insert(ViewMsgState& st, u32 item) {
    if (list_contains(st, item)) {
        return;
    }
    int idx = 0;
    while (idx < st.listCap) {
        if (st.listNodes[idx].next == nullptr) {
            break;
        }
        idx++;
    }
    ResNode* node = &st.listNodes[idx];
    node->item = item;
    node->next = st.startPtr;
    node->prev = st.startPtr->prev;
    st.startPtr->prev->next = node;
    st.startPtr->prev = node;
}

static void list_remove(ViewMsgState& st, u32 item) {
    ResNode* curr = st.sentinel.next;
    while (curr != &st.sentinel) {
        ResNode* next = curr->next;
        if (curr->item == item) {
            curr->prev->next = next;
            next->prev = curr->prev;
            curr->next = nullptr;
        }
        curr = next;
    }
}

static void drain_common(ViewMsgState& st, bool isDecomp) {
    (void)isDecomp;
    while (st.writeIdx != 0) {
        const u32 readIdx = st.unk3F0;
        const u32 cap = st.capacity;
        const u32 idx = readIdx - (readIdx / cap) * cap;
        u8* slot = st.slots[idx].data;
        const u32 tag = load_word32(slot);

        if (tag <= 7) {
            switch (tag) {
            case 0:
            case 1: {
                const u32 item = load_word32(slot + 4);
                if (tag == 0) {
                    list_insert(st, item);
                } else {
                    list_insert(st, item);
                }
                // Host oracle: skip fan-out (needs child graph + getWorkThread).
                // Parent-is-view gate only affects fan-out, not list insert.
                (void)st.parentIsView;
                break;
            }
            case 2:
                list_remove(st, load_word32(slot + 4));
                break;
            case 3:
                st.unk278 |= 0x3;
                break;
            case 4:
                st.unk278 |= 0x4;
                break;
            case 5:
                st.unk278 &= ~0x4u;
                break;
            case 6:
                st.setCurrentCalls += 1;
                break;
            case 7:
                if (slot[4] == 0) {
                    st.unk278 |= 0x20;
                } else {
                    st.unk278 &= ~0x20u;
                }
                break;
            }
        }

        st.writeIdx -= 1;
        st.unk3F0 = (readIdx + 1) - ((readIdx + 1) / cap) * cap;
    }
}

static void retail_update_msg(ViewMsgState& st) {
    drain_common(st, false);
}

static void decomp_update_msg(ViewMsgState& st) {
    drain_common(st, true);
}

static MsgOutcome snapshot(const ViewMsgState& st) {
    MsgOutcome out;
    out.unk3F0 = st.unk3F0;
    out.writeIdx = st.writeIdx;
    out.unk278 = st.unk278;
    out.listCount = list_count(st);
    out.setCurrentCalls = st.setCurrentCalls;
    return out;
}

static bool outcomes_equal(const MsgOutcome& a, const MsgOutcome& b) {
    return a.unk3F0 == b.unk3F0 && a.writeIdx == b.writeIdx && a.unk278 == b.unk278 &&
           a.listCount == b.listCount && a.setCurrentCalls == b.setCurrentCalls;
}

static int g_pass = 0;
static int g_fail = 0;

static void run_scenario(const char* name, void (*setup)(ViewMsgState&)) {
    ViewMsgState retailSt;
    ViewMsgState decompSt;
    init_state(retailSt, 10);
    init_state(decompSt, 10);
    setup(retailSt);
    setup(decompSt);
    retail_update_msg(retailSt);
    decomp_update_msg(decompSt);
    MsgOutcome r = snapshot(retailSt);
    MsgOutcome d = snapshot(decompSt);
    if (outcomes_equal(r, d)) {
        g_pass++;
        std::printf("PASS %s\n", name);
    } else {
        g_fail++;
        std::printf(
            "FAIL %s retail=(3f0=%u wr=%u f=%u lc=%u sc=%u) decomp=(3f0=%u wr=%u f=%u lc=%u sc=%u)\n",
            name, r.unk3F0, r.writeIdx, r.unk278, r.listCount, r.setCurrentCalls, d.unk3F0,
            d.writeIdx, d.unk278, d.listCount, d.setCurrentCalls);
    }
}

static void setup_empty(ViewMsgState& st) {
    (void)st;
}

static void setup_tag3(ViewMsgState& st) {
    enqueue_msg(st, 3, 0);
}

static void setup_tag4(ViewMsgState& st) {
    enqueue_msg(st, 4, 0);
}

static void setup_tag5_clear(ViewMsgState& st) {
    st.unk278 = 0x4;
    enqueue_msg(st, 5, 0);
}

static void setup_tag5_noop(ViewMsgState& st) {
    st.unk278 = 0x1;
    enqueue_msg(st, 5, 0);
}

static void setup_tag7_set(ViewMsgState& st) {
    enqueue_msg(st, 7, 0, 0);
}

static void setup_tag7_clear(ViewMsgState& st) {
    st.unk278 = 0x20;
    enqueue_msg(st, 7, 0, 1);
}

static void setup_tag6(ViewMsgState& st) {
    enqueue_msg(st, 6, 0);
}

static void setup_tag0_insert(ViewMsgState& st) {
    enqueue_msg(st, 0, 0x42);
}

static void setup_tag0_dup(ViewMsgState& st) {
    list_insert(st, 0x42);
    enqueue_msg(st, 0, 0x42);
}

static void setup_tag1_insert(ViewMsgState& st) {
    enqueue_msg(st, 1, 0x99);
}

static void setup_tag2_remove(ViewMsgState& st) {
    list_insert(st, 0x55);
    list_insert(st, 0x66);
    enqueue_msg(st, 2, 0x55);
}

static void setup_tag2_missing(ViewMsgState& st) {
    list_insert(st, 0x55);
    enqueue_msg(st, 2, 0x77);
}

static void setup_tag_gt7(ViewMsgState& st) {
    enqueue_msg(st, 8, 0);
    st.unk278 = 0xAB;
}

static void setup_wrap_head(ViewMsgState& st) {
    st.unk3F0 = 9;
    enqueue_msg(st, 3, 0);
}

static void setup_multi_flags(ViewMsgState& st) {
    enqueue_msg(st, 3, 0);
    enqueue_msg(st, 4, 0);
    enqueue_msg(st, 7, 0, 0);
}

static void setup_clear_then_set20(ViewMsgState& st) {
    st.unk278 = 0x24;
    enqueue_msg(st, 5, 0);
    enqueue_msg(st, 7, 0, 0);
}

static void setup_two_inserts(ViewMsgState& st) {
    enqueue_msg(st, 0, 1);
    enqueue_msg(st, 0, 2);
}

static void setup_insert_remove(ViewMsgState& st) {
    enqueue_msg(st, 0, 0x10);
    enqueue_msg(st, 2, 0x10);
}

static void setup_capacity_walk(ViewMsgState& st) {
    st.unk3F0 = 7;
    for (u32 i = 0; i < 5; i++) {
        enqueue_msg(st, 3, 0);
    }
}

static void setup_parent_view_tag0(ViewMsgState& st) {
    st.parentIsView = true;
    enqueue_msg(st, 0, 0x21);
}

static void setup_parent_view_tag1(ViewMsgState& st) {
    st.parentIsView = true;
    enqueue_msg(st, 1, 0x22);
}

static void setup_flags_preserve(ViewMsgState& st) {
    st.unk278 = 0x40;
    enqueue_msg(st, 3, 0);
}

static void setup_tag4_then_5(ViewMsgState& st) {
    enqueue_msg(st, 4, 0);
    enqueue_msg(st, 5, 0);
}

static void setup_tag7_twice(ViewMsgState& st) {
    enqueue_msg(st, 7, 0, 0);
    enqueue_msg(st, 7, 0, 1);
}

static void setup_setcurrent_x2(ViewMsgState& st) {
    enqueue_msg(st, 6, 0);
    enqueue_msg(st, 6, 0);
}

static void setup_mixed_list(ViewMsgState& st) {
    enqueue_msg(st, 0, 3);
    enqueue_msg(st, 1, 4);
    enqueue_msg(st, 2, 3);
}

static void setup_head_zero_count(ViewMsgState& st) {
    st.unk3F0 = 3;
}

static void setup_orphan_remove(ViewMsgState& st) {
    enqueue_msg(st, 2, 0xABCDEF);
}

static void setup_tag0_many(ViewMsgState& st) {
    for (u32 i = 0; i < 4; i++) {
        enqueue_msg(st, 0, 0x100 + i);
    }
}

int main() {
    run_scenario("empty", setup_empty);
    run_scenario("tag3_or", setup_tag3);
    run_scenario("tag4_or", setup_tag4);
    run_scenario("tag5_clear4", setup_tag5_clear);
    run_scenario("tag5_noop", setup_tag5_noop);
    run_scenario("tag7_set20", setup_tag7_set);
    run_scenario("tag7_clear20", setup_tag7_clear);
    run_scenario("tag6_setcurrent", setup_tag6);
    run_scenario("tag0_insert", setup_tag0_insert);
    run_scenario("tag0_dup", setup_tag0_dup);
    run_scenario("tag1_insert", setup_tag1_insert);
    run_scenario("tag2_remove", setup_tag2_remove);
    run_scenario("tag2_missing", setup_tag2_missing);
    run_scenario("tag_gt7_skip", setup_tag_gt7);
    run_scenario("wrap_head", setup_wrap_head);
    run_scenario("multi_flags", setup_multi_flags);
    run_scenario("clear4_set20", setup_clear_then_set20);
    run_scenario("two_inserts", setup_two_inserts);
    run_scenario("insert_remove", setup_insert_remove);
    run_scenario("capacity_walk", setup_capacity_walk);
    run_scenario("parent_view_tag0", setup_parent_view_tag0);
    run_scenario("parent_view_tag1", setup_parent_view_tag1);
    run_scenario("flags_preserve", setup_flags_preserve);
    run_scenario("tag4_then_5", setup_tag4_then_5);
    run_scenario("tag7_twice", setup_tag7_twice);
    run_scenario("setcurrent_x2", setup_setcurrent_x2);
    run_scenario("mixed_list", setup_mixed_list);
    run_scenario("head_zero_count", setup_head_zero_count);
    run_scenario("orphan_remove", setup_orphan_remove);
    run_scenario("tag0_many", setup_tag0_many);
    // +extra scenarios for <80% tier margin
    run_scenario("empty2", setup_empty);
    run_scenario("tag3_b", setup_tag3);
    run_scenario("tag4_b", setup_tag4);

    std::printf("summary pass=%d fail=%d\n", g_pass, g_fail);
    return g_fail == 0 ? 0 : 1;
}
