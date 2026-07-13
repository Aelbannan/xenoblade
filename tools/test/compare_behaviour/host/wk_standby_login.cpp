#include <cstdio>
#include <cstring>

#include <types.h>

// Isolated arithmetic from CGame::wkStandbyLogin (8043BBA0 region).

static s16 retail_narrow_height(u16 efbHeight, u16 unk230) {
    return (s16)((u16)efbHeight - (u16)((u16)unk230 * 2));
}

static s16 decomp_narrow_height(u16 efbHeight, u16 unk230) {
    return (s16)((u16)efbHeight - (u16)((u32)unk230 << 1));
}

static s16 retail_narrow_y(u16 unk230) {
    return (s16)((u16)unk230 - 1);
}

static s16 decomp_narrow_y(u16 unk230) {
    return (s16)((u16)unk230 - 1);
}

struct ViewRectArgs {
    s16 x;
    s16 y;
    s16 w;
    s16 h;
};

static ViewRectArgs retail_wide_rect(u16 fbWidth, u16 efbHeight) {
    return ViewRectArgs{0, 0, (s16)fbWidth, (s16)efbHeight};
}

static ViewRectArgs decomp_wide_rect(u16 fbWidth, u16 efbHeight) {
    return ViewRectArgs{0, 0, (s16)fbWidth, (s16)efbHeight};
}

static ViewRectArgs retail_narrow_rect(u16 fbWidth, u16 efbHeight, u16 unk230) {
    return ViewRectArgs{0, retail_narrow_y(unk230), (s16)fbWidth, retail_narrow_height(efbHeight, unk230)};
}

static ViewRectArgs decomp_narrow_rect(u16 fbWidth, u16 efbHeight, u16 unk230) {
    return ViewRectArgs{0, decomp_narrow_y(unk230), (s16)fbWidth, decomp_narrow_height(efbHeight, unk230)};
}

struct Vec4Bits {
    u32 x;
    u32 y;
    u32 z;
    u32 w;
};

static Vec4Bits retail_vec4_spill() {
    float spills[4];
    spills[0] = 0.0f;
    spills[1] = 0.0f;
    spills[2] = 0.0f;
    spills[3] = 1.0f;
    Vec4Bits out{};
    out.x = *(u32*)&spills[0];
    out.y = *(u32*)&spills[1];
    out.z = *(u32*)&spills[2];
    out.w = *(u32*)&spills[3];
    return out;
}

static Vec4Bits decomp_vec4_spill() {
    float spills[4];
    spills[0] = 0.0f;
    spills[1] = 0.0f;
    spills[2] = 0.0f;
    spills[3] = 1.0f;
    Vec4Bits out{};
    out.x = *(u32*)&spills[0];
    out.y = *(u32*)&spills[1];
    out.z = *(u32*)&spills[2];
    out.w = *(u32*)&spills[3];
    return out;
}

static void run_scenario_height(const char* name, u16 efbHeight, u16 unk230, s16 expected) {
    s16 retail = retail_narrow_height(efbHeight, unk230);
    s16 decomp = decomp_narrow_height(efbHeight, unk230);
    if (retail != decomp || retail != expected) {
        std::printf("FAIL %s height: retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_y(const char* name, u16 unk230, s16 expected) {
    s16 retail = retail_narrow_y(unk230);
    s16 decomp = decomp_narrow_y(unk230);
    if (retail != decomp || retail != expected) {
        std::printf("FAIL %s y: retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_rect(const char* name, bool wide, u16 fbWidth, u16 efbHeight, u16 unk230,
                              const ViewRectArgs& expected) {
    ViewRectArgs retail = wide ? retail_wide_rect(fbWidth, efbHeight)
                               : retail_narrow_rect(fbWidth, efbHeight, unk230);
    ViewRectArgs decomp = wide ? decomp_wide_rect(fbWidth, efbHeight)
                               : decomp_narrow_rect(fbWidth, efbHeight, unk230);
    if (std::memcmp(&retail, &decomp, sizeof(retail)) != 0
        || std::memcmp(&retail, &expected, sizeof(retail)) != 0) {
        std::printf("FAIL %s rect\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_vec4(const char* name) {
    Vec4Bits retail = retail_vec4_spill();
    Vec4Bits decomp = decomp_vec4_spill();
    float rw, dw;
    std::memcpy(&rw, &retail.w, sizeof(rw));
    std::memcpy(&dw, &decomp.w, sizeof(dw));
    if (std::memcmp(&retail, &decomp, sizeof(retail)) != 0 || rw != 1.0f || dw != 1.0f) {
        std::printf("FAIL %s vec4\n", name);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario_height("default_480_57", 480, 57, (s16)(480 - 57 * 2));
    run_scenario_height("efb_574_57", 574, 57, (s16)(574 - 57 * 2));
    run_scenario_height("zero_band", 480, 0, 480);
    run_scenario_height("max_band", 480, 120, (s16)(480 - 240));
    run_scenario_height("band_one", 480, 1, 478);

    for (u16 band = 0; band < 12; ++band) {
        char name[32];
        std::snprintf(name, sizeof(name), "band_sweep_%u", band);
        run_scenario_height(name, 480, band, (s16)(480 - band * 2));
    }

    run_scenario_y("y_default_57", 57, 56);
    run_scenario_y("y_one", 1, 0);
    run_scenario_y("y_zero", 0, (s16)0xFFFF);
    for (u16 unk = 2; unk < 20; ++unk) {
        char name[32];
        std::snprintf(name, sizeof(name), "y_%u", unk);
        run_scenario_y(name, unk, (s16)(unk - 1));
    }

    run_scenario_rect("wide_640_480", true, 640, 480, 0, ViewRectArgs{0, 0, 640, 480});
    run_scenario_rect("wide_574_456", true, 574, 456, 0, ViewRectArgs{0, 0, 574, 456});
    run_scenario_rect("narrow_640_57", false, 640, 480, 57, ViewRectArgs{0, 56, 640, (s16)(480 - 114)});
    run_scenario_rect("narrow_574_57", false, 574, 574, 57, ViewRectArgs{0, 56, 574, (s16)(574 - 114)});

    run_scenario_vec4("vec4_spill_default");

    std::printf("host wk_standby_login: all scenarios passed\n");
    return 0;
}
