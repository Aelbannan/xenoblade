#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail sin table (33 bytes) ships from the nw4r data unit (nw4r_data.s);
// referenced by name so no local .rodata copy is emitted.
extern "C" const u8 lbl_eu_8051FF40[0x21];

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
