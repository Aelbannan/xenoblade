#include <cmath>
#include <cstdio>
#include <cstring>

#include <types.h>

static constexpr f32 kDeadzone = 0.5f;
static constexpr int MAX_PAD_INPUT_FLAGS = 32;
static constexpr int TURBO_HOLD_TIMER_THRESHOLD = 20;
static constexpr int TURBO_INPUT_FRAMES = 6;

static constexpr u32 PAD_INPUT_FLAG_LEFT = (1u << 0);
static constexpr u32 PAD_INPUT_FLAG_RIGHT = (1u << 1);
static constexpr u32 PAD_INPUT_FLAG_UP = (1u << 2);
static constexpr u32 PAD_INPUT_FLAG_DOWN = (1u << 3);
static constexpr u32 PAD_INPUT_FLAG_CORE_A = (1u << 4);
static constexpr u32 PAD_INPUT_FLAG_LSTICK_LEFT = (1u << 14);
static constexpr u32 PAD_INPUT_FLAG_LSTICK_RIGHT = (1u << 15);
static constexpr u32 PAD_INPUT_FLAG_LSTICK_UP = (1u << 16);
static constexpr u32 PAD_INPUT_FLAG_LSTICK_DOWN = (1u << 17);
static constexpr u32 PAD_INPUT_FLAG_DPAD =
    PAD_INPUT_FLAG_LEFT | PAD_INPUT_FLAG_RIGHT | PAD_INPUT_FLAG_UP | PAD_INPUT_FLAG_DOWN;
static constexpr u32 PAD_INPUT_FLAG_LSTICK =
    PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LSTICK_RIGHT | PAD_INPUT_FLAG_LSTICK_UP
    | PAD_INPUT_FLAG_LSTICK_DOWN;

struct PadSnapshot {
    f32 mLStickXRaw;
    f32 mLStickYRaw;
};

struct CfPadDataLite {
    PadSnapshot mPad;
    u32 mHeldButtonFlags;
    u32 mPrevHeldButtonFlags;
    u32 mPressedButtonFlags;
    u32 mTurboPressButtonFlags;
    u8 mButtonHoldTimersTurbo[MAX_PAD_INPUT_FLAGS];
};

static void retail_update_cf_pad_data(CfPadDataLite* dst, const PadSnapshot& srcPad, u32 srcHeld) {
    u32 prevHoldFlags = dst->mHeldButtonFlags;
    dst->mPrevHeldButtonFlags = prevHoldFlags;
    dst->mHeldButtonFlags = srcHeld;
    dst->mPad = srcPad;

    if (std::fabs(dst->mPad.mLStickXRaw) < kDeadzone) {
        dst->mHeldButtonFlags &= ~(PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LSTICK_RIGHT);
    }
    if (std::fabs(dst->mPad.mLStickYRaw) < kDeadzone) {
        dst->mHeldButtonFlags &= ~(PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_LSTICK_DOWN);
    }

    bool wasHoldingDpadButton = prevHoldFlags & PAD_INPUT_FLAG_DPAD;
    bool wasHoldingStickDir = prevHoldFlags & PAD_INPUT_FLAG_LSTICK;

    if (dst->mHeldButtonFlags & PAD_INPUT_FLAG_DPAD) {
        if (wasHoldingStickDir) {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_DPAD;
        } else {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_LSTICK;
        }
    } else {
        if (wasHoldingDpadButton) {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_LSTICK;
        } else {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_DPAD;
        }
    }

    u32 changedFlags = prevHoldFlags ^ dst->mHeldButtonFlags;
    u32 pressedButtonFlags = dst->mHeldButtonFlags & changedFlags;
    dst->mPressedButtonFlags = pressedButtonFlags;
    dst->mTurboPressButtonFlags = pressedButtonFlags;

    u32 bit = 1;
    for (int i = 0; i < MAX_PAD_INPUT_FLAGS; ++i) {
        if (dst->mHeldButtonFlags & bit) {
            if (dst->mPressedButtonFlags & bit) {
                dst->mButtonHoldTimersTurbo[i] = 0;
            }
            dst->mButtonHoldTimersTurbo[i]++;
            if (dst->mButtonHoldTimersTurbo[i]
                >= static_cast<u8>(TURBO_HOLD_TIMER_THRESHOLD + TURBO_INPUT_FRAMES)) {
                dst->mTurboPressButtonFlags |= bit;
                dst->mButtonHoldTimersTurbo[i] -= static_cast<u8>(TURBO_INPUT_FRAMES);
            }
        }
        bit <<= 1;
    }

    dst->mTurboPressButtonFlags &= dst->mHeldButtonFlags;
}

static void decomp_update_cf_pad_data(CfPadDataLite* dst, const PadSnapshot& srcPad, u32 srcHeld) {
    u32 prevHoldFlags = dst->mHeldButtonFlags;
    dst->mPrevHeldButtonFlags = prevHoldFlags;
    dst->mHeldButtonFlags = srcHeld;
    dst->mPad = srcPad;

    if (std::fabs(dst->mPad.mLStickXRaw) < kDeadzone) {
        dst->mHeldButtonFlags &= ~(PAD_INPUT_FLAG_LSTICK_LEFT | PAD_INPUT_FLAG_LSTICK_RIGHT);
    }
    if (std::fabs(dst->mPad.mLStickYRaw) < kDeadzone) {
        dst->mHeldButtonFlags &= ~(PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_LSTICK_DOWN);
    }

    bool wasHoldingDpadButton = prevHoldFlags & PAD_INPUT_FLAG_DPAD;
    bool wasHoldingStickDir = prevHoldFlags & PAD_INPUT_FLAG_LSTICK;

    if (dst->mHeldButtonFlags & PAD_INPUT_FLAG_DPAD) {
        if (wasHoldingStickDir) {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_DPAD;
        } else {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_LSTICK;
        }
    } else {
        if (wasHoldingDpadButton) {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_LSTICK;
        } else {
            dst->mHeldButtonFlags &= ~PAD_INPUT_FLAG_DPAD;
        }
    }

    u32 changedFlags = prevHoldFlags ^ dst->mHeldButtonFlags;
    u32 pressedButtonFlags = dst->mHeldButtonFlags & changedFlags;
    dst->mPressedButtonFlags = pressedButtonFlags;
    dst->mTurboPressButtonFlags = pressedButtonFlags;

    u32 bit = 1;
    for (int i = 0; i < MAX_PAD_INPUT_FLAGS; ++i) {
        if (dst->mHeldButtonFlags & bit) {
            if (dst->mPressedButtonFlags & bit) {
                dst->mButtonHoldTimersTurbo[i] = 0;
            }
            dst->mButtonHoldTimersTurbo[i]++;
            if (dst->mButtonHoldTimersTurbo[i]
                >= static_cast<u8>(TURBO_HOLD_TIMER_THRESHOLD + TURBO_INPUT_FRAMES)) {
                dst->mTurboPressButtonFlags |= bit;
                dst->mButtonHoldTimersTurbo[i] -= static_cast<u8>(TURBO_INPUT_FRAMES);
            }
        }
        bit <<= 1;
    }

    dst->mTurboPressButtonFlags &= dst->mHeldButtonFlags;
}

static void run_scenario(const char* name, CfPadDataLite initial, PadSnapshot srcPad, u32 srcHeld,
                         u32 expectedHeld, u32 expectedPressed, u32 expectedTurbo) {
    CfPadDataLite retail = initial;
    CfPadDataLite decomp = initial;
    retail_update_cf_pad_data(&retail, srcPad, srcHeld);
    decomp_update_cf_pad_data(&decomp, srcPad, srcHeld);

    if (std::memcmp(&retail, &decomp, sizeof(retail)) != 0 || retail.mHeldButtonFlags != expectedHeld
        || retail.mPressedButtonFlags != expectedPressed || retail.mTurboPressButtonFlags != expectedTurbo) {
        std::printf("FAIL %s held=%u pressed=%u turbo=%u\n", name, retail.mHeldButtonFlags,
                    retail.mPressedButtonFlags, retail.mTurboPressButtonFlags);
        std::exit(1);
    }
    std::printf("PASS %s\n", name);
}

static CfPadDataLite make_initial(u32 prevHeld) {
    CfPadDataLite data{};
    data.mHeldButtonFlags = prevHeld;
    data.mPrevHeldButtonFlags = prevHeld;
    return data;
}

int main() {
    run_scenario("deadzone_x_clears_stick", make_initial(0), {0.1f, 0.0f},
                 PAD_INPUT_FLAG_LSTICK_RIGHT | PAD_INPUT_FLAG_LSTICK_LEFT, 0, 0, 0);

    run_scenario("deadzone_y_clears_stick", make_initial(0), {0.0f, 0.2f},
                 PAD_INPUT_FLAG_LSTICK_UP | PAD_INPUT_FLAG_LSTICK_DOWN, 0, 0, 0);

    run_scenario("outside_deadzone_keeps_x", make_initial(0), {0.8f, 0.0f}, PAD_INPUT_FLAG_LSTICK_RIGHT,
                 PAD_INPUT_FLAG_LSTICK_RIGHT, PAD_INPUT_FLAG_LSTICK_RIGHT, PAD_INPUT_FLAG_LSTICK_RIGHT);

    run_scenario("dpad_over_stick", make_initial(PAD_INPUT_FLAG_LSTICK_RIGHT),
                 {0.0f, 0.0f}, PAD_INPUT_FLAG_RIGHT, 0, 0, 0);

    run_scenario("stick_over_dpad", make_initial(PAD_INPUT_FLAG_RIGHT), {0.8f, 0.0f},
                 PAD_INPUT_FLAG_LSTICK_RIGHT, 0, 0, 0);

    run_scenario("pressed_edge", make_initial(0), {0.0f, 0.0f}, PAD_INPUT_FLAG_CORE_A,
                 PAD_INPUT_FLAG_CORE_A, PAD_INPUT_FLAG_CORE_A, PAD_INPUT_FLAG_CORE_A);

    run_scenario("held_no_press", make_initial(PAD_INPUT_FLAG_CORE_A), {0.0f, 0.0f}, PAD_INPUT_FLAG_CORE_A,
                 PAD_INPUT_FLAG_CORE_A, 0, 0);

    CfPadDataLite turboSeed = make_initial(PAD_INPUT_FLAG_CORE_A);
    turboSeed.mButtonHoldTimersTurbo[4] = static_cast<u8>(TURBO_HOLD_TIMER_THRESHOLD + TURBO_INPUT_FRAMES - 1);
    run_scenario("turbo_fires", turboSeed, {0.0f, 0.0f}, PAD_INPUT_FLAG_CORE_A, PAD_INPUT_FLAG_CORE_A, 0,
                 PAD_INPUT_FLAG_CORE_A);

    std::printf("host cfpadtask_updatecfdapdata: all scenarios passed\n");
    return 0;
}
