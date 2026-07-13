#include <cstdio>
#include <cstring>

#include <types.h>

struct GXRenderModeObj {
    u16 fbWidth;
    u16 efbHeight;
    u16 pad;
    u16 viWidth;
};

static GXRenderModeObj gMode{640, 480, 0, 640};

extern "C" GXRenderModeObj* getRenderModeObj__9CDeviceVIFv() {
    return &gMode;
}

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

static CViewRectDataCore* retail_func_80459270(CViewRectDataCore* self) {
    s16 efbHeight;
    s16 zero = 0;

    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    self->unk0 = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    self->unk2 = efbHeight;
    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    self->unk4 = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    self->unk6 = efbHeight;
    self->unk8 = zero;
    self->unkA = zero;
    self->unkC = zero;
    self->unkE = zero;
    self->unk10 = zero;
    self->unk12 = zero;
    return self;
}

static CViewRectDataCore* decomp_func_80459270(CViewRectDataCore* self) {
    s16 efbHeight;
    s16 zero = 0;

    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    self->unk0 = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    self->unk2 = efbHeight;
    efbHeight = getRenderModeObj__9CDeviceVIFv()->efbHeight;
    self->unk4 = getRenderModeObj__9CDeviceVIFv()->fbWidth;
    self->unk6 = efbHeight;
    self->unk8 = zero;
    self->unkA = zero;
    self->unkC = zero;
    self->unkE = zero;
    self->unk10 = zero;
    self->unk12 = zero;
    return self;
}

static void run_scenario(const char* name, u16 fbWidth, u16 efbHeight) {
    gMode.fbWidth = fbWidth;
    gMode.efbHeight = efbHeight;
    gMode.viWidth = fbWidth;

    CViewRectDataCore retail{};
    CViewRectDataCore decomp{};
    retail_func_80459270(&retail);
    decomp_func_80459270(&decomp);
    if (std::memcmp(&retail, &decomp, sizeof(CViewRectDataCore)) != 0) {
        std::printf("FAIL %s\n", name);
        std::exit(1);
    }
    if (retail.unk0 != (s16)fbWidth || retail.unk4 != (s16)fbWidth || retail.unk2 != (s16)efbHeight
        || retail.unk6 != (s16)efbHeight) {
        std::printf("FAIL %s: unexpected field values\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario("ntsc_640x480", 640, 480);
    run_scenario("wide_856x480", 856, 480);
    run_scenario("pal_640x574", 640, 574);
    run_scenario("small_320x240", 320, 240);
    run_scenario("large_1280x720", 1280, 720);
    for (int i = 1; i <= 8; ++i) {
        char name[32];
        std::snprintf(name, sizeof(name), "mode_%d", i);
        run_scenario(name, (u16)(320 + i * 40), (u16)(240 + i * 30));
    }
    std::printf("host view_rect_data_init: all scenarios passed\n");
    return 0;
}
