#include <cstdio>
#include <cstring>

#include <types.h>

#include "monolib/math/CPnt16.hpp"

struct CViewRectDataCore {
    s16 unk0;
    s16 unk2;
    s16 unk4;
    s16 unk6;
    s16 unk8;
    s16 unkA;
    s16 unkC;
    s16 unkE;
    s16 unk10;
    s16 unk12;
};

static void retail_func_80459384(CViewRectDataCore* self, const ml::CPnt16& maxSize) {
    *(ml::CPnt16*)&self->unk4 = maxSize;
}

static void decomp_func_80459384(CViewRectDataCore* self, const ml::CPnt16& maxSize) {
    *(ml::CPnt16*)&self->unk4 = maxSize;
}

static void run_scenario(const char* name, s16 x0, s16 y0, s16 x4, s16 y4, s16 mx, s16 my) {
    CViewRectDataCore retail{};
    CViewRectDataCore decomp{};
    retail.unk0 = x0;
    retail.unk2 = y0;
    retail.unk4 = x4;
    retail.unk6 = y4;
    decomp = retail;
    ml::CPnt16 maxSize{mx, my};
    retail_func_80459384(&retail, maxSize);
    decomp_func_80459384(&decomp, maxSize);
    if (retail.unk4 != decomp.unk4 || retail.unk6 != decomp.unk6 || retail.unk0 != decomp.unk0) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("store_basic", 10, 20, 100, 200, 640, 480);
    run_scenario("store_zero", 0, 0, 0, 0, 0, 0);
    run_scenario("store_negative", -1, -2, 50, 60, -10, -20);
    run_scenario("store_max", 100, 100, 200, 200, 32767, 32767);
    run_scenario("store_min", -100, -100, 0, 0, -32768, -32768);
    for (int i = 0; i < 6; ++i) {
        char name[32];
        std::snprintf(name, sizeof(name), "store_iter_%d", i);
        run_scenario(name, (s16)i, (s16)(i * 2), (s16)(100 + i), (s16)(200 + i), (s16)(300 + i * 3),
                     (s16)(400 + i * 5));
    }
    std::printf("host view_rect_data_store: all scenarios passed\n");
    return 0;
}
