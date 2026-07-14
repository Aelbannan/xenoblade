#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <types.h>

// Isolated semantics of CUIWindowManager::Move (retail 8013D0C8, size 0x4DC).
// Two reslist<IUIWindow*> queues (mWindowList1 @0x5C, mWindowList2 @0x7C):
//   1) report the first live transition timer found across both queues into
//      lbl_eu_8066408C (list1 scanned via lbl_eu_80664088, then list2);
//   2) for each queue independently, walk from the head looking for the
//      first window with unk65!=0 or unkA1!=0; once found, set unk65=true on
//      that window and every window after it in the same queue (NOT a
//      restart-from-head "mark everyone" pass — propagation starts exactly
//      where the scan stopped);
//   3) for each queue independently, collect every window with unk64!=0 or
//      unkA0!=0, call SetRemove() on it, then unlink all collected nodes;
//   4) clear unkA0/unkA1.
//
// retail_* is reconstructed from docs/evidence/decomp/batch_2026-07-14g/
// asm_Move_CUIWindowManager.s. decomp_* mirrors
// src/kyoshin/CUIWindowManager.cpp — keep both in sync when editing the
// source.

namespace {

struct SubView {
    s32 timer828; // stand-in for IUIWindowSubView::unk828
};

struct Window {
    bool mIsRemove; // CProcess::mIsRemove @0x39
    SubView* unk5C;
    s32 unk60;
    bool unk64;
    bool unk65;
};

struct Node {
    Node* mNext;
    Node* mPrev;
    Window* mItem;
};

struct WindowQueue {
    Node sentinel;

    void init() {
        sentinel.mNext = &sentinel;
        sentinel.mPrev = &sentinel;
        sentinel.mItem = nullptr;
    }

    void push_back(Node* node, Window* item) {
        node->mItem = item;
        node->mNext = &sentinel;
        node->mPrev = sentinel.mPrev;
        sentinel.mPrev->mNext = node;
        sentinel.mPrev = node;
    }
};

struct Manager {
    WindowQueue list1;
    WindowQueue list2;
    bool unkA0;
    bool unkA1;
};

s16 g_timerOut;
Manager* g_instance;

s32 window_timer(const Window* w) {
    return w->unk5C != nullptr ? w->unk5C->timer828 : w->unk60;
}

// ---------------------------------------------------------------------
// Retail oracle (asm reconstruction)
// ---------------------------------------------------------------------

void retail_scan_timer(WindowQueue& q) {
    for (Node* n = q.sentinel.mNext; n != &q.sentinel; n = n->mNext) {
        s32 timer = window_timer(n->mItem);
        if (timer > 0) {
            g_timerOut = (s16)timer;
            break;
        }
    }
}

void retail_mark_from_first_hit(WindowQueue& q, bool unkA1) {
    Node* n = q.sentinel.mNext;
    for (; n != &q.sentinel; n = n->mNext) {
        if (n->mItem->unk65 != 0 || unkA1 != 0) {
            break;
        }
    }
    for (; n != &q.sentinel; n = n->mNext) {
        n->mItem->unk65 = true;
    }
}

void retail_collect_and_remove(WindowQueue& q, bool unkA0) {
    Node* pending[18];
    int count = 0;
    for (Node* n = q.sentinel.mNext; n != &q.sentinel; n = n->mNext) {
        Window* w = n->mItem;
        if (w->unk64 != 0 || unkA0 != 0) {
            pending[count++] = n;
            w->mIsRemove = true;
        }
    }
    for (int i = 0; i < count; i++) {
        Node* prev = pending[i]->mPrev;
        Node* next = pending[i]->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        pending[i]->mNext = nullptr;
    }
}

void retail_move() {
    Manager* inst = g_instance;
    if (inst == nullptr) {
        return;
    }

    retail_scan_timer(inst->list1);
    retail_scan_timer(inst->list2);

    retail_mark_from_first_hit(inst->list1, inst->unkA1);
    retail_mark_from_first_hit(inst->list2, inst->unkA1);

    retail_collect_and_remove(inst->list1, inst->unkA0);
    retail_collect_and_remove(inst->list2, inst->unkA0);

    inst->unkA0 = false;
    inst->unkA1 = false;
}

// ---------------------------------------------------------------------
// Decomp mirror (kept in sync with src/kyoshin/CUIWindowManager.cpp)
// ---------------------------------------------------------------------

void decomp_scan_timer(WindowQueue& q) {
    for (Node* node = q.sentinel.mNext; node != &q.sentinel; node = node->mNext) {
        Window* window = node->mItem;
        s32 timer = window->unk5C != nullptr ? window->unk5C->timer828 : window->unk60;
        if (timer > 0) {
            g_timerOut = (s16)timer;
            break;
        }
    }
}

void decomp_move() {
    Manager* inst = g_instance;
    if (inst == nullptr) {
        return;
    }

    decomp_scan_timer(inst->list1);
    decomp_scan_timer(inst->list2);

    {
        Node* it = inst->list1.sentinel.mNext;
        for (; it != &inst->list1.sentinel; it = it->mNext) {
            if (it->mItem->unk65 != 0 || inst->unkA1 != 0) {
                break;
            }
        }
        for (; it != &inst->list1.sentinel; it = it->mNext) {
            it->mItem->unk65 = true;
        }
    }
    {
        Node* it = inst->list2.sentinel.mNext;
        for (; it != &inst->list2.sentinel; it = it->mNext) {
            if (it->mItem->unk65 != 0 || inst->unkA1 != 0) {
                break;
            }
        }
        for (; it != &inst->list2.sentinel; it = it->mNext) {
            it->mItem->unk65 = true;
        }
    }

    Node* node;
    Node* pending[18];
    int pendingCount;
    int i;

    pendingCount = 0;
    for (node = inst->list1.sentinel.mNext; node != &inst->list1.sentinel; node = node->mNext) {
        Window* window = node->mItem;
        if (window->unk64 != 0 || inst->unkA0 != 0) {
            pending[pendingCount++] = node;
            window->mIsRemove = true;
        }
    }
    for (i = 0; i < pendingCount; i++) {
        Node* prev = pending[i]->mPrev;
        Node* next = pending[i]->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        pending[i]->mNext = nullptr;
    }

    pendingCount = 0;
    for (node = inst->list2.sentinel.mNext; node != &inst->list2.sentinel; node = node->mNext) {
        Window* window = node->mItem;
        if (window->unk64 != 0 || inst->unkA0 != 0) {
            pending[pendingCount++] = node;
            window->mIsRemove = true;
        }
    }
    for (i = 0; i < pendingCount; i++) {
        Node* prev = pending[i]->mPrev;
        Node* next = pending[i]->mNext;
        prev->mNext = next;
        next->mPrev = prev;
        pending[i]->mNext = nullptr;
    }

    inst->unkA0 = false;
    inst->unkA1 = false;
}

// ---------------------------------------------------------------------
// Scenario harness
// ---------------------------------------------------------------------

struct WindowSpec {
    s32 timerViaSubview; // used only when hasSubview
    bool hasSubview;
    s32 unk60;
    bool unk64;
    bool unk65;
};

struct Snapshot {
    int listSize[2];
    bool isRemove[2][18];
    bool unk65[2][18];
    s16 timerOut;
    bool unkA0;
    bool unkA1;
};

void build_manager(Manager& mgr, SubView (&subviews)[2][18], Window (&windows)[2][18], Node (&nodes)[2][18],
                    const WindowSpec (&specs)[2][18], const int listLen[2], bool unkA0, bool unkA1) {
    mgr.list1.init();
    mgr.list2.init();
    mgr.unkA0 = unkA0;
    mgr.unkA1 = unkA1;

    WindowQueue* queues[2] = {&mgr.list1, &mgr.list2};
    for (int q = 0; q < 2; q++) {
        for (int i = 0; i < listLen[q]; i++) {
            const WindowSpec& spec = specs[q][i];
            subviews[q][i].timer828 = spec.timerViaSubview;
            windows[q][i].mIsRemove = false;
            windows[q][i].unk5C = spec.hasSubview ? &subviews[q][i] : nullptr;
            windows[q][i].unk60 = spec.unk60;
            windows[q][i].unk64 = spec.unk64;
            windows[q][i].unk65 = spec.unk65;
            queues[q]->push_back(&nodes[q][i], &windows[q][i]);
        }
    }
}

void snapshot(Manager& mgr, const Window (&windows)[2][18], const int listLen[2], s16 timerOut, Snapshot& out) {
    WindowQueue* queues[2] = {&mgr.list1, &mgr.list2};
    out.timerOut = timerOut;
    out.unkA0 = mgr.unkA0;
    out.unkA1 = mgr.unkA1;
    for (int q = 0; q < 2; q++) {
        out.listSize[q] = 0;
        for (Node* n = queues[q]->sentinel.mNext; n != &queues[q]->sentinel; n = n->mNext) {
            out.listSize[q]++;
        }
        for (int i = 0; i < listLen[q]; i++) {
            out.isRemove[q][i] = windows[q][i].mIsRemove;
            out.unk65[q][i] = windows[q][i].unk65;
        }
    }
}

bool snapshots_equal(const Snapshot& a, const Snapshot& b, const int listLen[2]) {
    if (a.timerOut != b.timerOut) return false;
    if (a.unkA0 != b.unkA0 || a.unkA1 != b.unkA1) return false;
    for (int q = 0; q < 2; q++) {
        if (a.listSize[q] != b.listSize[q]) return false;
        for (int i = 0; i < listLen[q]; i++) {
            if (a.isRemove[q][i] != b.isRemove[q][i]) return false;
            if (a.unk65[q][i] != b.unk65[q][i]) return false;
        }
    }
    return true;
}

void run_scenario(const char* name, const WindowSpec (&specs)[2][18], const int listLen[2], bool unkA0, bool unkA1,
                   bool instanceIsNull) {
    Manager retailMgr{};
    Manager decompMgr{};
    SubView retailSub[2][18]{};
    SubView decompSub[2][18]{};
    Window retailWin[2][18]{};
    Window decompWin[2][18]{};
    Node retailNode[2][18]{};
    Node decompNode[2][18]{};

    build_manager(retailMgr, retailSub, retailWin, retailNode, specs, listLen, unkA0, unkA1);
    build_manager(decompMgr, decompSub, decompWin, decompNode, specs, listLen, unkA0, unkA1);

    g_timerOut = 0;
    g_instance = instanceIsNull ? nullptr : &retailMgr;
    retail_move();
    s16 retailTimer = g_timerOut;

    g_timerOut = 0;
    g_instance = instanceIsNull ? nullptr : &decompMgr;
    decomp_move();
    s16 decompTimer = g_timerOut;

    Snapshot retailSnap{};
    Snapshot decompSnap{};
    snapshot(retailMgr, retailWin, listLen, retailTimer, retailSnap);
    snapshot(decompMgr, decompWin, listLen, decompTimer, decompSnap);

    if (!snapshots_equal(retailSnap, decompSnap, listLen)) {
        std::printf("FAIL %s outcome mismatch\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

WindowSpec W(s32 timerViaSubview, bool hasSubview, s32 unk60, bool unk64, bool unk65) {
    WindowSpec s;
    s.timerViaSubview = timerViaSubview;
    s.hasSubview = hasSubview;
    s.unk60 = unk60;
    s.unk64 = unk64;
    s.unk65 = unk65;
    return s;
}

} // namespace

int main() {
    // 1) instance pointer null: everything must be a no-op.
    {
        WindowSpec specs[2][18] = {};
        int len[2] = {0, 0};
        run_scenario("null_instance", specs, len, false, false, true);
    }

    // 2) both queues empty, no flags: fully idle.
    {
        WindowSpec specs[2][18] = {};
        int len[2] = {0, 0};
        run_scenario("both_empty", specs, len, false, false, false);
    }

    // 3) single window in list1, fallback timer positive.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 5, false, false);
        int len[2] = {1, 0};
        run_scenario("list1_single_fallback_timer", specs, len, false, false, false);
    }

    // 4) single window in list1, subview timer positive.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(7, true, 0, false, false);
        int len[2] = {1, 0};
        run_scenario("list1_single_subview_timer", specs, len, false, false, false);
    }

    // 5) single window in list1, timer zero: no report.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        int len[2] = {1, 0};
        run_scenario("list1_single_zero_timer", specs, len, false, false, false);
    }

    // 6) single window in list1, timer negative: no report.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, -3, false, false);
        int len[2] = {1, 0};
        run_scenario("list1_single_negative_timer", specs, len, false, false, false);
    }

    // 7) list1 has no timer, list2's window has a timer.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[1][0] = W(0, false, 9, false, false);
        int len[2] = {1, 1};
        run_scenario("list2_reports_timer", specs, len, false, false, false);
    }

    // 8) both queues have timers: list2's later scan overwrites the output.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 4, false, false);
        specs[1][0] = W(0, false, 6, false, false);
        int len[2] = {1, 1};
        run_scenario("both_report_timer_list2_wins", specs, len, false, false, false);
    }

    // 9) first window's timer is zero, second window's is positive: scan continues.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 8, false, false);
        int len[2] = {2, 0};
        run_scenario("list1_second_window_reports", specs, len, false, false, false);
    }

    // 10) subview present but its timer is <=0: falls through to unk60? (No —
    // retail only reads unk60 when unk5C itself is null, so subview<=0 just
    // yields <=0 and the scan continues to the next window.)
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, true, 99, false, false); // subview timer 0, unk60 ignored while subview present
        specs[0][1] = W(0, false, 12, false, false);
        int len[2] = {2, 0};
        run_scenario("subview_present_zero_falls_to_next_window", specs, len, false, false, false);
    }

    // 11) mark-all: first window already has unk65 set -> propagates to all after it.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, true);
        specs[0][1] = W(0, false, 0, false, false);
        specs[0][2] = W(0, false, 0, false, false);
        int len[2] = {3, 0};
        run_scenario("mark_all_from_head", specs, len, false, false, false);
    }

    // 12) mark propagation starts at the middle hit, not from the head.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, false, true);
        specs[0][2] = W(0, false, 0, false, false);
        int len[2] = {3, 0};
        run_scenario("mark_propagates_from_middle_only", specs, len, false, false, false);
    }

    // 13) mark propagation starts at the last window: earlier windows stay unmarked.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, false, false);
        specs[0][2] = W(0, false, 0, false, true);
        int len[2] = {3, 0};
        run_scenario("mark_propagates_from_tail_only", specs, len, false, false, false);
    }

    // 14) unkA1 forces marking of every window in the queue even if none were set.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, false, false);
        int len[2] = {2, 0};
        run_scenario("unkA1_marks_all_from_head", specs, len, false, true, false);
    }

    // 15) no unk65 set anywhere and unkA1 false: nobody gets marked.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, false, false);
        int len[2] = {2, 0};
        run_scenario("no_mark_when_nothing_set", specs, len, false, false, false);
    }

    // 16) list2 mark-all propagation independent of list1's outcome.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[1][0] = W(0, false, 0, false, false);
        specs[1][1] = W(0, false, 0, false, true);
        int len[2] = {1, 2};
        run_scenario("list2_mark_independent_of_list1", specs, len, false, false, false);
    }

    // 17) collect+remove: single flagged window in the middle of list1.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, true, false);
        specs[0][2] = W(0, false, 0, false, false);
        int len[2] = {3, 0};
        run_scenario("remove_single_middle_list1", specs, len, false, false, false);
    }

    // 18) remove the head window.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, true, false);
        specs[0][1] = W(0, false, 0, false, false);
        int len[2] = {2, 0};
        run_scenario("remove_head", specs, len, false, false, false);
    }

    // 19) remove the tail window.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, true, false);
        int len[2] = {2, 0};
        run_scenario("remove_tail", specs, len, false, false, false);
    }

    // 20) remove every window in list1 (all flagged individually).
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, true, false);
        specs[0][1] = W(0, false, 0, true, false);
        specs[0][2] = W(0, false, 0, true, false);
        int len[2] = {3, 0};
        run_scenario("remove_all_flagged_individually", specs, len, false, false, false);
    }

    // 21) unkA0 forces removal of every window even without individual flags.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, false, false);
        specs[0][2] = W(0, false, 0, false, false);
        int len[2] = {3, 0};
        run_scenario("unkA0_removes_all", specs, len, true, false, false);
    }

    // 22) alternating flagged/unflagged windows in list1 (non-contiguous removal).
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, true, false);
        specs[0][1] = W(0, false, 0, false, false);
        specs[0][2] = W(0, false, 0, true, false);
        specs[0][3] = W(0, false, 0, false, false);
        specs[0][4] = W(0, false, 0, true, false);
        int len[2] = {5, 0};
        run_scenario("remove_alternating_pattern", specs, len, false, false, false);
    }

    // 23) removal independent across list1/list2.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, true, false);
        specs[1][0] = W(0, false, 0, false, false);
        specs[1][1] = W(0, false, 0, true, false);
        int len[2] = {1, 2};
        run_scenario("remove_independent_across_lists", specs, len, false, false, false);
    }

    // 24) large list1 (9 windows) exercising the 8x-unrolled collect/remove path.
    {
        WindowSpec specs[2][18] = {};
        for (int i = 0; i < 9; i++) {
            specs[0][i] = W(0, false, 0, (i % 2) == 0, false);
        }
        int len[2] = {9, 0};
        run_scenario("large_list1_unrolled_boundary", specs, len, false, false, false);
    }

    // 25) exactly 8 flagged windows (unroll boundary, no remainder).
    {
        WindowSpec specs[2][18] = {};
        for (int i = 0; i < 8; i++) {
            specs[0][i] = W(0, false, 0, true, false);
        }
        int len[2] = {8, 0};
        run_scenario("exactly_eight_flagged", specs, len, false, false, false);
    }

    // 26) 17 windows, all flagged (near pending[] capacity, multi-unroll-block).
    {
        WindowSpec specs[2][18] = {};
        for (int i = 0; i < 17; i++) {
            specs[0][i] = W(0, false, 0, true, false);
        }
        int len[2] = {17, 0};
        run_scenario("seventeen_flagged_near_capacity", specs, len, false, false, false);
    }

    // 27) large list2 with a mixed remove pattern while list1 stays untouched.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        for (int i = 0; i < 10; i++) {
            specs[1][i] = W(0, false, 0, (i % 3) == 0, false);
        }
        int len[2] = {1, 10};
        run_scenario("large_list2_mixed_remove", specs, len, false, false, false);
    }

    // 28) combined: timer report + mark-all + selective removal all in one pass.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 5, false, false);
        specs[0][1] = W(0, false, 0, true, true);
        specs[0][2] = W(0, false, 0, false, false);
        specs[1][0] = W(3, true, 0, false, false);
        specs[1][1] = W(0, false, 0, false, false);
        int len[2] = {3, 2};
        run_scenario("combined_timer_mark_remove", specs, len, false, false, false);
    }

    // 29) unkA0 and unkA1 both set simultaneously across both queues.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[0][1] = W(0, false, 0, false, false);
        specs[1][0] = W(0, false, 0, false, false);
        int len[2] = {2, 1};
        run_scenario("unkA0_and_unkA1_together", specs, len, true, true, false);
    }

    // 30) every window already has unk65 set: mark loop is a no-op, but still
    // walks (found at the very first node).
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, true);
        specs[0][1] = W(0, false, 0, false, true);
        specs[0][2] = W(0, false, 0, false, true);
        int len[2] = {3, 0};
        run_scenario("all_already_marked", specs, len, false, false, false);
    }

    // 31) single-element queues in both lists, nothing flagged, no timers.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0, false, false);
        specs[1][0] = W(0, false, 0, false, false);
        int len[2] = {1, 1};
        run_scenario("single_element_both_lists_idle", specs, len, false, false, false);
    }

    // 32) list1 empty, list2 has a full removal + mark-all combination.
    {
        WindowSpec specs[2][18] = {};
        specs[1][0] = W(0, false, 0, true, false);
        specs[1][1] = W(0, false, 0, false, false);
        specs[1][2] = W(0, false, 0, true, true);
        int len[2] = {0, 3};
        run_scenario("list1_empty_list2_full_combo", specs, len, false, false, false);
    }

    // 33) unk60 fallback with a huge positive value (still fits s16 truncation check).
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0, false, 0x7FFF, false, false);
        int len[2] = {1, 0};
        run_scenario("fallback_timer_max_s16", specs, len, false, false, false);
    }

    // 34) subview timer larger than s16 range: output truncates to (s16) like retail's sth.
    {
        WindowSpec specs[2][18] = {};
        specs[0][0] = W(0x12345, true, 0, false, false);
        int len[2] = {1, 0};
        run_scenario("subview_timer_truncates_to_s16", specs, len, false, false, false);
    }

    std::printf("host uiwindowmanager_move: all scenarios passed\n");
    return 0;
}
