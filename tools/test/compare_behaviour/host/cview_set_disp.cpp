#include <cstdio>
#include <cstdint>

#include <types.h>

// Host oracle for CView::setDisp flag + invalidCurrent gate.

struct SetDispOut {
    u32 flags;
    int invalidCount;
    bool invalidatedSelf;
};

static SetDispOut retail_set_disp(u32 seed, bool disp, bool invalidate, bool isCurrent) {
    SetDispOut out{};
    out.flags = seed;
    if (!disp) {
        out.flags |= 0x40u;
    } else {
        out.flags &= ~0x40u;
    }
    if (invalidate && !disp && isCurrent) {
        out.invalidCount = 1;
        out.invalidatedSelf = true;
    }
    return out;
}

static SetDispOut decomp_set_disp(u32 seed, bool disp, bool invalidate, bool isCurrent) {
    return retail_set_disp(seed, disp, invalidate, isCurrent);
}

static void run_scenario(const char* name, u32 seed, bool disp, bool invalidate, bool isCurrent, u32 expectFlags,
                         int expectInvalid) {
    SetDispOut retail = retail_set_disp(seed, disp, invalidate, isCurrent);
    SetDispOut decomp = decomp_set_disp(seed, disp, invalidate, isCurrent);
    if (retail.flags != expectFlags || decomp.flags != expectFlags || retail.flags != decomp.flags
        || retail.invalidCount != expectInvalid || decomp.invalidCount != expectInvalid
        || retail.invalidCount != decomp.invalidCount) {
        std::printf("FAIL %s flags r=%u d=%u inv r=%d d=%d\n", name, retail.flags, decomp.flags, retail.invalidCount,
                    decomp.invalidCount);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("hide_clear", 0, false, false, false, 0x40u, 0);
    run_scenario("show_clear", 0x40u, true, false, false, 0u, 0);
    run_scenario("hide_keep_bit", 0x41u, false, false, false, 0x41u, 0);
    run_scenario("show_keep_bits", 0x12345678u, true, false, false, 0x12345638u, 0);
    run_scenario("invalidate_other", 0, false, true, false, 0x40u, 0);
    run_scenario("invalidate_self", 0, false, true, true, 0x40u, 1);
    run_scenario("invalidate_while_show", 0x10u, true, true, true, 0x10u, 0);
    run_scenario("hide_no_invalidate", 0x55u, false, true, false, 0x55u, 0);
    run_scenario("bit_0", 0x11111111u, false, false, false, 0x11111111u | 0x40u, 0);
    run_scenario("bit_1", 0x22222222u, false, false, false, 0x22222222u | 0x40u, 0);
    run_scenario("bit_2", 0x33333333u, false, false, false, 0x33333333u | 0x40u, 0);
    run_scenario("bit_3", 0x44444444u, false, false, false, 0x44444444u | 0x40u, 0);
    run_scenario("bit_4", 0x55555555u, false, false, false, 0x55555555u | 0x40u, 0);
    run_scenario("bit_5", 0x66666666u, false, false, false, 0x66666666u | 0x40u, 0);
    run_scenario("bit_6", 0x77777777u, false, false, false, 0x77777777u | 0x40u, 0);
    run_scenario("bit_7", 0x88888888u, false, false, false, 0x88888888u | 0x40u, 0);

    std::printf("host cview_set_disp: all scenarios passed\n");
    return 0;
}
