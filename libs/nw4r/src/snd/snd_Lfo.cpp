#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

extern "C" __declspec(section ".rodata") __attribute__((aligned(8))) const u8 lbl_eu_8051FF40[0x28] = {
    0x00, 0x06, 0x0C, 0x13, 0x19, 0x1F, 0x25, 0x2B, 0x31, 0x36, 0x3C, 0x41,
    0x47, 0x4C, 0x51, 0x55, 0x5A, 0x5E, 0x62, 0x66, 0x6A, 0x6D, 0x70, 0x73,
    0x75, 0x78, 0x7A, 0x7B, 0x7D, 0x7E, 0x7E, 0x7F, 0x7F, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00
};

// Retail .sdata2 constants referenced by name so no local pool is emitted.
extern "C" {
    extern const f32 lbl_eu_80669FB0; // 0.0f
    extern const f32 lbl_eu_80669FB4; // 6.25f
    extern const f32 lbl_eu_80669FB8; // 1000.0f
    extern const f32 lbl_eu_80669FC8; // 4.0f
    extern const f32 lbl_eu_80669FCC; // 32.0f
    extern const f32 lbl_eu_80669FD0; // 127.0f
}

// Retail inlines this into GetValue and never emits a standalone body.
static const int kTableSize = 32;

static s8 GetSinIdx(int idx) {
    if (idx < kTableSize) {
        return lbl_eu_8051FF40[idx];
    }

    if (idx < kTableSize * 2) {
        return lbl_eu_8051FF40[kTableSize - (idx - kTableSize)];
    }

    if (idx < kTableSize * 3) {
        return -lbl_eu_8051FF40[idx - kTableSize * 2];
    }

    return -lbl_eu_8051FF40[kTableSize - (idx - kTableSize * 3)];
}

void LfoParam::Init() {
    depth = lbl_eu_80669FB0;
    range = 1;
    speed = lbl_eu_80669FB4;
    delay = 0;
}

void Lfo::Reset() {
    mCounter = lbl_eu_80669FB0;
    mDelayCounter = 0;
}

void Lfo::Update(int msec) {
    if (mDelayCounter < mParam.delay) {
        if (mDelayCounter + msec <= mParam.delay) {
            mDelayCounter += msec;
            return;
        }

        msec -= mParam.delay - mDelayCounter;
        mDelayCounter = mParam.delay;
    }

    mCounter += mParam.speed * msec / lbl_eu_80669FB8;
    mCounter -= static_cast<int>(mCounter);
}

f32 Lfo::GetValue() const {
    if (lbl_eu_80669FB0 == mParam.depth) {
        return lbl_eu_80669FB0;
    }

    if (mDelayCounter < mParam.delay) {
        return lbl_eu_80669FB0;
    }

    f32 value = GetSinIdx(static_cast<int>(lbl_eu_80669FC8 * (lbl_eu_80669FCC * mCounter))) /
                lbl_eu_80669FD0;

    value *= mParam.depth;
    value *= mParam.range;

    return value;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
