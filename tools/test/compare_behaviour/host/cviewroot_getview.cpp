#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>

using u32 = std::uint32_t;

static constexpr int THREAD_CVIEW = 48;
static constexpr int THREAD_CVIEW_MAX = 53;
static constexpr u32 INVALID_WORK_ID = 0xFFFFFFFFu;

struct MockThread {
    int mType;
};

struct ReslistNode {
    ReslistNode* mNext;
    ReslistNode* mPrev;
    MockThread* mItem;
};

struct MockChildren {
    ReslistNode sentinel;
    ReslistNode nodes[4];
};

struct MockViewRoot {
    MockChildren mChildren;
};

static MockViewRoot s_root{};
static MockViewRoot* s_singleton = nullptr;
static MockThread s_threads[8]{};
static MockThread* s_workTable[8]{};

static void reset_state() {
    std::memset(&s_root, 0, sizeof(s_root));
    std::memset(s_threads, 0, sizeof(s_threads));
    std::memset(s_workTable, 0, sizeof(s_workTable));
    s_singleton = nullptr;
    s_root.mChildren.sentinel.mNext = &s_root.mChildren.sentinel;
    s_root.mChildren.sentinel.mPrev = &s_root.mChildren.sentinel;
}

static void set_children_count(int count) {
    if (count == 0) {
        s_root.mChildren.sentinel.mNext = &s_root.mChildren.sentinel;
        s_root.mChildren.sentinel.mPrev = &s_root.mChildren.sentinel;
        return;
    }

    for (int i = 0; i < count; ++i) {
        s_root.mChildren.nodes[i].mItem = nullptr;
        s_root.mChildren.nodes[i].mNext = (i + 1 < count) ? &s_root.mChildren.nodes[i + 1]
                                                            : &s_root.mChildren.sentinel;
        s_root.mChildren.nodes[i].mPrev = (i == 0) ? &s_root.mChildren.sentinel : &s_root.mChildren.nodes[i - 1];
    }
    s_root.mChildren.sentinel.mNext = &s_root.mChildren.nodes[0];
    s_root.mChildren.sentinel.mPrev = &s_root.mChildren.nodes[count - 1];
}

static MockThread* lookup_work_thread(u32 id) {
    if (id == INVALID_WORK_ID) {
        return nullptr;
    }
    if (id >= 8u) {
        return nullptr;
    }
    return s_workTable[id];
}

static MockThread* retail_get_view(u32 id) {
    if (s_singleton == nullptr) {
        return nullptr;
    }

    ReslistNode* sentinel = &s_singleton->mChildren.sentinel;
    ReslistNode* node = sentinel->mNext;
    MockThread* thread = nullptr;

    while (node != &s_singleton->mChildren.sentinel) {
        thread = lookup_work_thread(id);
        if (thread != nullptr) {
            break;
        }
        node = node->mNext;
    }

    if (thread == nullptr) {
        return nullptr;
    }

    const int type = thread->mType;
    if (type >= THREAD_CVIEW) {
        if (type >= THREAD_CVIEW_MAX) {
            thread = nullptr;
        }
    } else {
        thread = nullptr;
    }

    return thread;
}

static MockThread* decomp_get_view(u32 id) {
    if (s_singleton == nullptr) {
        return nullptr;
    }

    ReslistNode* sentinel = &s_singleton->mChildren.sentinel;
    ReslistNode* node = sentinel->mNext;
    MockThread* thread = nullptr;

    while (node != &s_singleton->mChildren.sentinel) {
        thread = lookup_work_thread(id);
        if (thread != nullptr) {
            break;
        }
        node = node->mNext;
    }

    if (thread == nullptr) {
        return nullptr;
    }

    const int type = thread->mType;
    if (type >= THREAD_CVIEW) {
        if (type >= THREAD_CVIEW_MAX) {
            thread = nullptr;
        }
    } else {
        thread = nullptr;
    }

    return thread;
}

static void run_scenario(const char* name, u32 id, int childCount, int tableIndex, int threadType,
                         MockThread* expected) {
    reset_state();
    s_singleton = &s_root;
    set_children_count(childCount);

    if (tableIndex >= 0 && id < 8u) {
        s_threads[tableIndex].mType = threadType;
        s_workTable[id] = &s_threads[tableIndex];
    }

    MockThread* retail = retail_get_view(id);
    MockThread* decomp = decomp_get_view(id);

    if (retail != decomp || retail != expected) {
        std::printf("FAIL %s retail=%p decomp=%p expected=%p\n", name,
                    static_cast<void*>(retail), static_cast<void*>(decomp), static_cast<void*>(expected));
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_singleton_null(const char* name) {
    reset_state();
    s_singleton = nullptr;
    if (retail_get_view(1u) != nullptr || decomp_get_view(1u) != nullptr) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario_singleton_null("singleton_null");
    run_scenario("empty_children_skips_lookup", 2u, 0, 1, THREAD_CVIEW, nullptr);
    run_scenario("missing_work_thread", 3u, 2, -1, 0, nullptr);
    run_scenario("cview_min_type", 4u, 1, 4, THREAD_CVIEW, &s_threads[4]);
    run_scenario("cview_max_minus_one", 5u, 1, 5, THREAD_CVIEW_MAX - 1, &s_threads[5]);
    run_scenario("below_cview_range", 6u, 1, 6, THREAD_CVIEW - 1, nullptr);
    run_scenario("at_cview_max", 7u, 1, 7, THREAD_CVIEW_MAX, nullptr);
    run_scenario("above_cview_max", 1u, 1, 1, THREAD_CVIEW_MAX + 3, nullptr);
    run_scenario("invalid_work_id", INVALID_WORK_ID, 1, -1, THREAD_CVIEW, nullptr);
    run_scenario("walk_two_nodes_then_hit", 2u, 2, 2, THREAD_CVIEW, &s_threads[2]);
    run_scenario("walk_three_nodes_miss", 3u, 3, -1, 0, nullptr);
    run_scenario("proc_type_rejected", 4u, 1, 4, 24, nullptr);
    run_scenario("desktop_type_rejected", 5u, 1, 5, 47, nullptr);
    std::printf("host cviewroot_getview: all scenarios passed\n");
    return 0;
}
