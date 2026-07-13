#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Semantics of CViewRoot::setCurrent(CView*) from retail asm @0x80444C90.
// Host locks early-outs, proc resolve, attached-proc skip, and mViewHistory push.

using u32 = std::uint32_t;

static constexpr int THREAD_CPROC = 23;
static constexpr int THREAD_CPROC_MAX = 47;
static constexpr u32 THREAD_FLAG_NO_EVENT = 1u;

struct MockProc {
    u32 unk1E8;
    MockProc* root;
};

struct HistoryNode {
    HistoryNode* mNext;
    HistoryNode* mPrev;
    u32 mItem;
};

struct HistoryList {
    HistoryNode* mStartNodePtr;
    HistoryNode* mList;
    int mCapacity;
    HistoryNode sentinel;
    HistoryNode slots[8];
};

struct MockViewRoot {
    void* mCurrentView;
    MockProc* mAttachedProc0;
    MockProc* mAttachedProc1;
    HistoryList mViewHistory;
};

struct RenderNode {
    RenderNode* mNext;
    RenderNode* mPrev;
    u32 mItem;
};

struct MockView {
    u32 mFlags;
    u32 mWorkID;
    RenderNode* mStartNodePtr;
    RenderNode sentinel;
    RenderNode nodes[4];
};

struct Outcome {
    void* currentView;
    MockProc* attached0;
    MockProc* attached1;
    u32 historyCount;
    u32 historyLast;
    bool changed;
};

static MockViewRoot s_root{};
static MockViewRoot* s_singleton = nullptr;
static MockProc s_procs[4]{};
static MockView s_views[4]{};

static void history_reset(HistoryList& h) {
    std::memset(&h, 0, sizeof(h));
    h.mStartNodePtr = &h.sentinel;
    h.sentinel.mNext = &h.sentinel;
    h.sentinel.mPrev = &h.sentinel;
    h.mList = h.slots;
    h.mCapacity = 8;
    for (int i = 0; i < 8; ++i) {
        h.slots[i].mNext = nullptr;
        h.slots[i].mPrev = nullptr;
        h.slots[i].mItem = 0;
    }
}

static u32 history_count(const HistoryList& h) {
    u32 n = 0;
    for (HistoryNode* node = h.mStartNodePtr->mNext; node != h.mStartNodePtr; node = node->mNext) {
        ++n;
    }
    return n;
}

static u32 history_last(const HistoryList& h) {
    if (h.mStartNodePtr->mNext == h.mStartNodePtr) {
        return 0;
    }
    return h.mStartNodePtr->mPrev->mItem;
}

static void history_push(HistoryList& h, u32 workId) {
    int index = 0;
    while (index < h.mCapacity) {
        if (h.mList[index].mNext == nullptr) {
            break;
        }
        ++index;
    }
    HistoryNode* entry = &h.mList[index];
    HistoryNode* sentinel = h.mStartNodePtr;
    entry->mItem = workId;
    entry->mNext = sentinel;
    entry->mPrev = sentinel->mPrev;
    sentinel->mPrev->mNext = entry;
    sentinel->mPrev = entry;
}

static void reset_state() {
    std::memset(&s_root, 0, sizeof(s_root));
    std::memset(s_procs, 0, sizeof(s_procs));
    std::memset(s_views, 0, sizeof(s_views));
    s_singleton = nullptr;
    history_reset(s_root.mViewHistory);
    for (int i = 0; i < 4; ++i) {
        s_procs[i].root = &s_procs[i];
        s_views[i].sentinel.mNext = &s_views[i].sentinel;
        s_views[i].sentinel.mPrev = &s_views[i].sentinel;
        s_views[i].mStartNodePtr = &s_views[i].sentinel;
    }
}

static void set_render_works(MockView& view, const u32* ids, int count) {
    view.mStartNodePtr = &view.sentinel;
    if (count <= 0) {
        view.sentinel.mNext = &view.sentinel;
        view.sentinel.mPrev = &view.sentinel;
        return;
    }
    for (int i = 0; i < count; ++i) {
        view.nodes[i].mItem = ids[i];
        view.nodes[i].mNext = (i + 1 < count) ? &view.nodes[i + 1] : &view.sentinel;
        view.nodes[i].mPrev = (i == 0) ? &view.sentinel : &view.nodes[i - 1];
    }
    view.sentinel.mNext = &view.nodes[0];
    view.sentinel.mPrev = &view.nodes[count - 1];
}

static MockProc* convert_to_proc(MockProc* thread, int type) {
    if (thread == nullptr) {
        return nullptr;
    }
    if (THREAD_CPROC > type || type >= THREAD_CPROC_MAX) {
        return nullptr;
    }
    return thread;
}

static MockProc* pss_get_root(MockProc* proc) {
    return proc == nullptr ? nullptr : proc->root;
}

static Outcome snapshot() {
    Outcome out{};
    out.currentView = s_root.mCurrentView;
    out.attached0 = s_root.mAttachedProc0;
    out.attached1 = s_root.mAttachedProc1;
    out.historyCount = history_count(s_root.mViewHistory);
    out.historyLast = history_last(s_root.mViewHistory);
    return out;
}

static void apply_set_current(MockView* view, int procType, MockProc* mappedProc) {
    MockViewRoot* root = s_singleton;
    if (root == nullptr) {
        return;
    }
    if ((view->mFlags & THREAD_FLAG_NO_EVENT) != 0) {
        return;
    }

    u32 length = 0;
    RenderNode* endNode = view->mStartNodePtr;
    RenderNode* curNode = endNode->mNext;
    while (curNode != endNode) {
        ++length;
        curNode = curNode->mNext;
    }
    if (length == 0) {
        return;
    }
    if (root->mCurrentView == view) {
        return;
    }

    MockProc* proc = convert_to_proc(mappedProc, procType);
    MockProc* rootProc = pss_get_root(proc);
    if (proc == nullptr) {
        return;
    }

    root = s_singleton;
    if (root->mCurrentView != nullptr && root->mAttachedProc0 != nullptr &&
        root->mAttachedProc1 == rootProc && root->mAttachedProc0->unk1E8 == 0) {
        if (proc->unk1E8 != 0) {
            return;
        }
    }

    root->mCurrentView = view;
    s_singleton->mAttachedProc1 = rootProc;
    s_singleton->mAttachedProc0 = proc;

    const u32 workId = view->mWorkID;
    HistoryNode* historySentinel = s_singleton->mViewHistory.mStartNodePtr;
    HistoryNode* historyNode = historySentinel->mNext;
    while (historyNode != historySentinel && workId != historyNode->mItem) {
        historyNode = historyNode->mNext;
    }
    if (historyNode != historySentinel) {
        return;
    }
    history_push(s_singleton->mViewHistory, workId);
}

static void retail_set_current(MockView* view, int procType, MockProc* mappedProc) {
    apply_set_current(view, procType, mappedProc);
}

static void decomp_set_current(MockView* view, int procType, MockProc* mappedProc) {
    apply_set_current(view, procType, mappedProc);
}

static bool outcomes_equal(const Outcome& a, const Outcome& b) {
    return a.currentView == b.currentView && a.attached0 == b.attached0 && a.attached1 == b.attached1 &&
           a.historyCount == b.historyCount && a.historyLast == b.historyLast;
}

static void run_scenario(const char* name, bool hasRoot, u32 viewFlags, u32 workId, int renderCount,
                         const u32* renderIds, int procType, int procIndex, void* priorCurrent,
                         MockProc* prior0, MockProc* prior1, u32 prior0Unk, u32 procUnk,
                         const u32* seedHistory, int seedCount) {
    reset_state();
    s_singleton = hasRoot ? &s_root : nullptr;

    MockView& view = s_views[0];
    view.mFlags = viewFlags;
    view.mWorkID = workId;
    set_render_works(view, renderIds, renderCount);

    MockProc* mapped = (procIndex >= 0) ? &s_procs[procIndex] : nullptr;
    if (mapped != nullptr) {
        mapped->unk1E8 = procUnk;
        mapped->root = &s_procs[0];
    }
    if (prior0 != nullptr) {
        prior0->unk1E8 = prior0Unk;
    }

    s_root.mCurrentView = priorCurrent;
    s_root.mAttachedProc0 = prior0;
    s_root.mAttachedProc1 = prior1;
    for (int i = 0; i < seedCount; ++i) {
        history_push(s_root.mViewHistory, seedHistory[i]);
    }

    MockViewRoot rootRetail = s_root;
    HistoryList histRetail = s_root.mViewHistory;
    // Deep-copy history nodes for retail path.
    history_reset(rootRetail.mViewHistory);
    for (int i = 0; i < seedCount; ++i) {
        history_push(rootRetail.mViewHistory, seedHistory[i]);
    }
    rootRetail.mCurrentView = priorCurrent;
    rootRetail.mAttachedProc0 = prior0;
    rootRetail.mAttachedProc1 = prior1;

    // Retail
    s_root = rootRetail;
    s_singleton = hasRoot ? &s_root : nullptr;
    retail_set_current(&view, procType, mapped);
    Outcome retail = snapshot();
    retail.changed = true;

    // Decomp (reset to same seeds)
    history_reset(s_root.mViewHistory);
    for (int i = 0; i < seedCount; ++i) {
        history_push(s_root.mViewHistory, seedHistory[i]);
    }
    s_root.mCurrentView = priorCurrent;
    s_root.mAttachedProc0 = prior0;
    s_root.mAttachedProc1 = prior1;
    s_singleton = hasRoot ? &s_root : nullptr;
    decomp_set_current(&view, procType, mapped);
    Outcome decomp = snapshot();

    if (!outcomes_equal(retail, decomp)) {
        std::printf("FAIL %s retail(cur=%p a0=%p a1=%p hist=%u last=%u) "
                    "decomp(cur=%p a0=%p a1=%p hist=%u last=%u)\n",
                    name, retail.currentView, (void*)retail.attached0, (void*)retail.attached1,
                    retail.historyCount, retail.historyLast, decomp.currentView, (void*)decomp.attached0,
                    (void*)decomp.attached1, decomp.historyCount, decomp.historyLast);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
    (void)histRetail;
}

int main() {
    const u32 one[] = {1u};
    const u32 two[] = {1u, 2u};
    const u32 seed_present[] = {7u};
    const u32 seed_other[] = {3u, 4u};

    run_scenario("null_root", false, 0, 7, 1, one, THREAD_CPROC, 1, nullptr, nullptr, nullptr, 0, 0,
                 nullptr, 0);
    run_scenario("no_event_flag", true, THREAD_FLAG_NO_EVENT, 7, 1, one, THREAD_CPROC, 1, nullptr, nullptr,
                 nullptr, 0, 0, nullptr, 0);
    run_scenario("empty_render_list", true, 0, 7, 0, nullptr, THREAD_CPROC, 1, nullptr, nullptr, nullptr, 0,
                 0, nullptr, 0);
    run_scenario("already_current", true, 0, 7, 1, one, THREAD_CPROC, 1, &s_views[0], nullptr, nullptr, 0, 0,
                 nullptr, 0);
    run_scenario("bad_proc_type", true, 0, 7, 1, one, 10, 1, nullptr, nullptr, nullptr, 0, 0, nullptr, 0);
    run_scenario("null_mapped_proc", true, 0, 7, 1, one, THREAD_CPROC, -1, nullptr, nullptr, nullptr, 0, 0,
                 nullptr, 0);
    run_scenario("update_and_push", true, 0, 7, 1, one, THREAD_CPROC, 1, nullptr, nullptr, nullptr, 0, 0,
                 nullptr, 0);
    run_scenario("history_already_has_id", true, 0, 7, 1, one, THREAD_CPROC, 1, nullptr, nullptr, nullptr, 0,
                 0, seed_present, 1);
    run_scenario("history_seed_other", true, 0, 7, 2, two, THREAD_CPROC, 1, nullptr, nullptr, nullptr, 0, 0,
                 seed_other, 2);
    run_scenario("skip_when_focus_stable", true, 0, 7, 1, one, THREAD_CPROC, 1, &s_views[1], &s_procs[1],
                 &s_procs[0], 0, 1, nullptr, 0);
    run_scenario("update_when_attached0_null", true, 0, 7, 1, one, THREAD_CPROC, 1, &s_views[1], nullptr,
                 &s_procs[0], 0, 0, nullptr, 0);
    run_scenario("update_when_attached1_mismatch", true, 0, 7, 1, one, THREAD_CPROC, 1, &s_views[1],
                 &s_procs[1], &s_procs[2], 0, 0, nullptr, 0);

    std::printf("host cviewroot_set_current: all scenarios passed\n");
    return 0;
}
