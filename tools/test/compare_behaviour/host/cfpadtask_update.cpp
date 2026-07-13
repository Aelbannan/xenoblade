#include <cmath>
#include <cstdio>
#include <cstdint>

#include <types.h>

static constexpr f32 kStickDead = 0.3f;
static constexpr f32 kStickScale = 0.7f;
static constexpr u32 kRStickXFlags = 0x60000u;
static constexpr u32 kRStickYFlags = 0x180000u;

static constexpr u32 PAD_TYPE_CORE = 2;
static constexpr u32 PAD_TYPE_FS = 3;
static constexpr u32 PAD_TYPE_CLASSIC = 4;

static f32 retail_normalize_rstick(f32 stick, u32 heldFlags, u32 axisFlags) {
    if (!(heldFlags & axisFlags)) {
        return 0.0f;
    }
    if (stick >= 0.0f) {
        return (stick - kStickDead) / kStickScale;
    }
    return -(std::fabs(stick) - kStickDead) / kStickScale;
}

static f32 decomp_normalize_rstick(f32 stick, u32 heldFlags, u32 axisFlags) {
    if (!(heldFlags & axisFlags)) {
        return 0.0f;
    }
    f32 local = stick;
    if (local >= 0.0f) {
        return (local - kStickDead) / kStickScale;
    }
    return -(std::fabs(local) - kStickDead) / kStickScale;
}

static bool retail_pad_accepted(u32 padType, u32 mainPadType, u32 lblFlags, bool connected) {
    u32 result = 0;
    if (padType == mainPadType || mainPadType == PAD_TYPE_CORE
        || (padType == PAD_TYPE_CORE && !(lblFlags & (1u << 28)))) {
        result = 1;
    }
    if (!connected) {
        result = 0;
    }
    return result != 0;
}

static bool decomp_pad_accepted(u32 padType, u32 mainPadType, u32 lblFlags, bool connected) {
    u32 result = 0;
    if (padType == mainPadType || mainPadType == PAD_TYPE_CORE
        || (padType == PAD_TYPE_CORE && !(lblFlags & (1u << 28)))) {
        result = 1;
    }
    if (!connected) {
        result = 0;
    }
    return result != 0;
}

static void run_scenario_stick(const char* name, f32 stick, u32 heldFlags, u32 axisFlags, f32 expected) {
    f32 retail = retail_normalize_rstick(stick, heldFlags, axisFlags);
    f32 decomp = decomp_normalize_rstick(stick, heldFlags, axisFlags);
    f32 expectedValue = expected;
    if (retail != decomp || std::fabs(retail - expectedValue) > 1e-5f) {
        std::printf("FAIL %s stick: retail=%f decomp=%f expected=%f\n", name, retail, decomp, expectedValue);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static void run_scenario_gate(const char* name, u32 padType, u32 mainPadType, u32 lblFlags, bool connected,
                              bool expected) {
    bool retail = retail_pad_accepted(padType, mainPadType, lblFlags, connected);
    bool decomp = decomp_pad_accepted(padType, mainPadType, lblFlags, connected);
    if (retail != decomp || retail != expected) {
        std::printf("FAIL %s gate: retail=%d decomp=%d expected=%d\n", name, retail, decomp, expected);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

int main() {
    run_scenario_stick("x_no_flag", 1.0f, 0, kRStickXFlags, 0.0f);
    run_scenario_stick("x_positive_full", 1.0f, kRStickXFlags, kRStickXFlags, (1.0f - kStickDead) / kStickScale);
    run_scenario_stick("x_positive_half", 0.65f, kRStickXFlags, kRStickXFlags, 0.5f);
    run_scenario_stick("x_at_deadzone", 0.3f, kRStickXFlags, kRStickXFlags, 0.0f);
    run_scenario_stick("x_negative_full", -1.0f, kRStickXFlags, kRStickXFlags,
                       -(1.0f - kStickDead) / kStickScale);
    run_scenario_stick("x_negative_half", -0.65f, kRStickXFlags, kRStickXFlags, -0.5f);

    run_scenario_stick("y_no_flag", 0.8f, 0, kRStickYFlags, 0.0f);
    run_scenario_stick("y_positive", 1.0f, kRStickYFlags, kRStickYFlags, (1.0f - kStickDead) / kStickScale);
    run_scenario_stick("y_negative", -1.0f, kRStickYFlags, kRStickYFlags, -(1.0f - kStickDead) / kStickScale);

    run_scenario_gate("gate_match_type", PAD_TYPE_CLASSIC, PAD_TYPE_CLASSIC, 0, true, true);
    run_scenario_gate("gate_core_main", PAD_TYPE_FS, PAD_TYPE_CORE, 0, true, true);
    run_scenario_gate("gate_core_pad_no_bit28", PAD_TYPE_CORE, PAD_TYPE_FS, 0, true, true);
    run_scenario_gate("gate_core_pad_bit28", PAD_TYPE_CORE, PAD_TYPE_FS, (1u << 28), true, false);
    run_scenario_gate("gate_mismatch_disconnected", PAD_TYPE_CLASSIC, PAD_TYPE_FS, 0, false, false);
    run_scenario_gate("gate_fs_classic_mismatch", PAD_TYPE_FS, PAD_TYPE_CLASSIC, 0, true, false);

    std::printf("host cfpadtask_update: all scenarios passed\n");
    return 0;
}
