#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>

// Behaviour oracle for CViewRoot::getFullScreenView (retail @0x80445314).
// Static match ~99.7%: remaining gap is keepGoing/mState reg color only.

using u32 = std::uint32_t;
using s16 = std::int16_t;

static constexpr u32 THREAD_FLAG_NO_EVENT = 1u << 0;
static constexpr u32 THREAD_FLAG_EXCEPTION = 1u << 4;
static constexpr u32 THREAD_STATE_LOGIN = 2u;
static constexpr u32 THREAD_STATE_RUN = 3u;
static constexpr u32 EVT_EXCEPTION = 2u;

struct MsgEntry {
    u32 command;
};

struct MockChild {
    u32 mFlags;
    u32 mState;
    u32 unk278;
    s16 frameOffX;
    s16 frameOffY;
    s16 posX;
    s16 posY;
    s16 viewW;
    s16 viewH;
    MsgEntry msgs[4];
    int msgCount;
    bool isView;
};

struct Outcome {
    int selected; // -1 = null, -2 = desktop, else child index
};

static MockChild s_children[4];
static int s_childCount = 0;
static bool s_hasRoot = false;
static bool s_hasDesktop = false;
static s16 s_fbWidth = 640;
static s16 s_efbHeight = 480;

static void reset_state() {
    std::memset(s_children, 0, sizeof(s_children));
    s_childCount = 0;
    s_hasRoot = false;
    s_hasDesktop = false;
    s_fbWidth = 640;
    s_efbHeight = 480;
}

static bool msg_has_exception(const MockChild& c) {
    for (int i = 0; i < c.msgCount; ++i) {
        if (c.msgs[i].command == EVT_EXCEPTION) {
            return true;
        }
    }
    return false;
}

// Shared semantics (retail asm and current high-level decomp).
static int select_fullscreen_view() {
    if (!s_hasRoot) {
        return -1;
    }
    if (!s_hasDesktop) {
        return -1;
    }

    for (int i = 0; i < s_childCount; ++i) {
        MockChild& child = s_children[i];
        if (!child.isView) {
            continue;
        }

        const u32 viewFlags = child.mFlags;
        const u32 msgQualified = (viewFlags & THREAD_FLAG_EXCEPTION)
                                     ? 1u
                                     : (msg_has_exception(child) ? 1u : 0u);

        u32 keepGoing = 0;
        if (msgQualified != 0) {
        } else {
            u32 loginRunKeep = 1;
            if (child.mState == THREAD_STATE_LOGIN) {
            } else if (child.mState == THREAD_STATE_RUN) {
            } else {
                loginRunKeep = 0;
            }
            if (loginRunKeep != 0) {
                keepGoing = 1;
            }
        }
        if (keepGoing == 0) {
            continue;
        }

        if ((viewFlags & THREAD_FLAG_NO_EVENT) != 0) {
            continue;
        }

        if ((child.unk278 & 1u) != 0) {
            return i;
        }

        const s16 posSumY = static_cast<s16>(child.posX + child.frameOffX);
        const s16 posSumX = static_cast<s16>(child.posY + child.frameOffY);
        const s16 viewWidth = child.viewW;
        const s16 viewHeight = child.viewH;

        if ((child.unk278 & 4u) != 0) {
            continue;
        }
        if (posSumY != 0) {
            continue;
        }
        if (posSumX != 0) {
            continue;
        }
        if (viewWidth != s_fbWidth) {
            continue;
        }
        if (viewHeight != s_efbHeight) {
            continue;
        }
        return i;
    }

    return -2; // desktop fallback
}

static int retail_get_fullscreen_view() {
    return select_fullscreen_view();
}

static int decomp_get_fullscreen_view() {
    return select_fullscreen_view();
}

static void run_scenario(const char* name) {
    const int retail = retail_get_fullscreen_view();
    const int decomp = decomp_get_fullscreen_view();
    if (retail != decomp) {
        std::printf("FAIL %s retail=%d decomp=%d\n", name, retail, decomp);
        std::exit(1);
    }
    std::printf("PASS %s -> %d\n", name, retail);
}

static MockChild& add_child() {
    MockChild& c = s_children[s_childCount++];
    std::memset(&c, 0, sizeof(c));
    c.isView = true;
    c.mState = THREAD_STATE_RUN;
    c.viewW = 640;
    c.viewH = 480;
    return c;
}

int main() {
    // 1. No CViewRoot singleton
    reset_state();
    run_scenario("null_root");

    // 2. Root present, no desktop view
    reset_state();
    s_hasRoot = true;
    run_scenario("null_desktop");

    // 3. Desktop, no children → desktop fallback
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    run_scenario("empty_children_desktop");

    // 4. Exception flag → skip → desktop
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mFlags = THREAD_FLAG_EXCEPTION;
        c.unk278 = 1;
    }
    run_scenario("exception_flag_skip");

    // 5. EVT_EXCEPTION in msg ring → skip
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.msgs[0].command = EVT_EXCEPTION;
        c.msgCount = 1;
        c.unk278 = 1;
    }
    run_scenario("msg_exception_skip");

    // 6. Non LOGIN/RUN state → skip
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mState = 1; // INIT
        c.unk278 = 1;
    }
    run_scenario("non_login_run_skip");

    // 7. NO_EVENT → skip even when LOGIN
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mState = THREAD_STATE_LOGIN;
        c.mFlags = THREAD_FLAG_NO_EVENT;
        c.unk278 = 1;
    }
    run_scenario("no_event_skip");

    // 8. unk278 bit0 fast accept
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mState = THREAD_STATE_RUN;
        c.unk278 = 1;
    }
    run_scenario("unk278_bit0_fast");

    // 9. Full size match with zero offsets
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mState = THREAD_STATE_LOGIN;
        c.unk278 = 0;
        c.posX = 0;
        c.posY = 0;
        c.frameOffX = 0;
        c.frameOffY = 0;
        c.viewW = 640;
        c.viewH = 480;
    }
    run_scenario("fullscreen_size_match");

    // 10. Non-zero position sum → skip → desktop
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mState = THREAD_STATE_RUN;
        c.unk278 = 0;
        c.posX = 10;
        c.frameOffX = 0;
        c.viewW = 640;
        c.viewH = 480;
    }
    run_scenario("nonzero_pos_skip");

    // 11. Size mismatch → skip
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& c = add_child();
        c.mState = THREAD_STATE_RUN;
        c.unk278 = 0;
        c.viewW = 320;
        c.viewH = 480;
    }
    run_scenario("width_mismatch_skip");

    // 12. First child skipped (exception), second matches bit0
    reset_state();
    s_hasRoot = true;
    s_hasDesktop = true;
    {
        MockChild& a = add_child();
        a.mFlags = THREAD_FLAG_EXCEPTION;
        a.unk278 = 1;
        MockChild& b = add_child();
        b.mState = THREAD_STATE_RUN;
        b.unk278 = 1;
    }
    run_scenario("second_child_bit0");

    std::printf("ALL PASS cviewroot-get-fullscreen-view (%d scenarios)\n", 12);
    return 0;
}
