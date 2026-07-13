#include <cassert>
#include <cstdio>
#include <cstring>

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

static void retail_func_804592F0(CViewRectDataCore* self, const ml::CPnt16& size) {
    BOOL overW;
    BOOL overH;
    s16 maxHeight;
    s16 heightLimit;
    s16 partialH;
    s16 maxWidth;
    s16 widthLimit;

    *(ml::CPnt16*)&self->unk0 = size;
    maxWidth = (s16)(self->unk0 - self->unkC - self->unk10);
    widthLimit = (s16)(self->unk4 - maxWidth);
    partialH = (s16)(self->unk2 - self->unkE);
    if ((overW = (s32)self->unk8 > (s32)self->unk4 - (s32)maxWidth, maxHeight = (s16)(partialH - self->unk12), overW)) {
        self->unk8 = widthLimit;
    }
    heightLimit = (s16)(self->unk6 - maxHeight);
    if ((overH = (s32)self->unkA > (s32)self->unk6 - (s32)maxHeight, overH)) {
        self->unkA = heightLimit;
    }
    if (self->unk8 < 0) {
        self->unk8 = 0;
    }
    if (self->unkA >= 0) {
        return;
    }
    self->unkA = 0;
}

static void decomp_func_804592F0(CViewRectDataCore* self, const ml::CPnt16& size) {
    BOOL overW;
    BOOL overH;
    s16 maxHeight;
    s16 heightLimit;
    s16 partialH;
    s16 maxWidth;
    s16 widthLimit;

    *(ml::CPnt16*)&self->unk0 = size;
    maxWidth = (s16)(self->unk0 - self->unkC - self->unk10);
    widthLimit = (s16)(self->unk4 - maxWidth);
    partialH = (s16)(self->unk2 - self->unkE);
    if ((overW = (s32)self->unk8 > (s32)self->unk4 - (s32)maxWidth, maxHeight = (s16)(partialH - self->unk12), overW)) {
        self->unk8 = widthLimit;
    }
    heightLimit = (s16)(self->unk6 - maxHeight);
    if ((overH = (s32)self->unkA > (s32)self->unk6 - (s32)maxHeight, overH)) {
        self->unkA = heightLimit;
    }
    if (self->unk8 < 0) {
        self->unk8 = 0;
    }
    if (self->unkA >= 0) {
        return;
    }
    self->unkA = 0;
}

static void run_scenario(const char* name, const CViewRectDataCore& seed, s16 sx, s16 sy) {
    CViewRectDataCore retail{};
    CViewRectDataCore decomp{};
    ml::CPnt16 size{sx, sy};

    std::memcpy(&retail, &seed, sizeof(CViewRectDataCore));
    std::memcpy(&decomp, &seed, sizeof(CViewRectDataCore));
    retail_func_804592F0(&retail, size);
    decomp_func_804592F0(&decomp, size);

    if (std::memcmp(&retail, &decomp, sizeof(CViewRectDataCore)) != 0) {
        std::printf("FAIL %s: retail vs decomp state mismatch\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static CViewRectDataCore make_base() {
    CViewRectDataCore base{};
    base.unk0 = 0;
    base.unk2 = 0;
    base.unk4 = 640;
    base.unk6 = 480;
    base.unk8 = 400;
    base.unkA = 300;
    base.unkC = 0;
    base.unkE = 0;
    base.unk10 = 0;
    base.unk12 = 0;
    return base;
}

static CViewRectDataCore with_neg_unk8(const CViewRectDataCore& base) {
    CViewRectDataCore s = base;
    s.unk8 = -5;
    return s;
}

static CViewRectDataCore with_neg_unkA(const CViewRectDataCore& base) {
    CViewRectDataCore s = base;
    s.unkA = -3;
    return s;
}

static CViewRectDataCore with_large_margins(const CViewRectDataCore& base) {
    CViewRectDataCore s = base;
    s.unkC = 200;
    s.unkE = 150;
    s.unk10 = 100;
    s.unk12 = 80;
    return s;
}

static CViewRectDataCore with_tight_width(const CViewRectDataCore& base) {
    CViewRectDataCore s = base;
    s.unk8 = 600;
    s.unk4 = 500;
    return s;
}

static CViewRectDataCore with_tight_height(const CViewRectDataCore& base) {
    CViewRectDataCore s = base;
    s.unkA = 450;
    s.unk6 = 400;
    return s;
}

static CViewRectDataCore with_both_negative(const CViewRectDataCore& base) {
    CViewRectDataCore s = base;
    s.unk8 = -20;
    s.unkA = -15;
    return s;
}

int main() {
    CViewRectDataCore base = make_base();

    run_scenario("within_bounds", base, 320, 240);
    run_scenario("overflow_both_axes", base, 800, 600);
    run_scenario("negative_size", base, -10, -20);
    run_scenario("zero_size", base, 0, 0);
    run_scenario("width_only_overflow", base, 900, 240);
    run_scenario("height_only_overflow", base, 320, 900);
    run_scenario("negative_unk8", with_neg_unk8(base), 100, 100);
    run_scenario("negative_unkA", with_neg_unkA(base), 100, 100);

    CViewRectDataCore inset = base;
    inset.unkC = 16;
    inset.unkE = 32;
    inset.unk10 = 8;
    inset.unk12 = 16;
    run_scenario("with_insets", inset, 700, 500);
    run_scenario("insets_small_input", inset, 50, 40);
    run_scenario("insets_overflow", inset, 1000, 800);

    for (int i = 0; i < 8; ++i) {
        char name[64];
        CViewRectDataCore s = base;
        s.unk8 = (s16)(100 + i * 50);
        s.unkA = (s16)(80 + i * 40);
        s.unkC = (s16)(i * 4);
        s.unk10 = (s16)(i * 2);
        std::snprintf(name, sizeof(name), "grid_%d", i);
        run_scenario(name, s, (s16)(200 + i * 30), (s16)(150 + i * 20));
    }

    for (int w = 0; w < 6; ++w) {
        char name[64];
        CViewRectDataCore s = base;
        s.unk4 = (s16)(320 + w * 64);
        s.unk6 = (s16)(240 + w * 48);
        std::snprintf(name, sizeof(name), "frame_%d", w);
        run_scenario(name, s, (s16)(s.unk4 - 1), (s16)(s.unk6 - 1));
    }

    run_scenario("max_s16_size", base, 32767, 32767);
    run_scenario("min_s16_size", base, -32768, -32768);
    run_scenario("large_margins", with_large_margins(base), 640, 480);
    run_scenario("tight_width", with_tight_width(base), 400, 300);
    run_scenario("tight_height", with_tight_height(base), 300, 350);
    run_scenario("both_negative_outputs", with_both_negative(base), -5, -8);
    run_scenario("symmetric_clamp", base, 640, 480);

    std::printf("host view_rect_data_core: all scenarios passed\n");
    return 0;
}
