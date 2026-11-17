#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

static s8 GetSinIdx(int idx) {
    static const int TABLE_SIZE = 32;
    static const s8 sinTable[TABLE_SIZE + 1] = {
        0,   6,   12,  19,  25,  31,  37,  43,  49,  54,  60,
        65,  71,  76,  81,  85,  90,  94,  98,  102, 106, 109,
        112, 115, 117, 120, 122, 123, 125, 126, 126, 127, 127
    };

    if (idx < TABLE_SIZE) {
        return sinTable[idx];
    }

    if (idx < TABLE_SIZE * 2) {
        return sinTable[TABLE_SIZE - (idx - TABLE_SIZE)];
    }

    if (idx < TABLE_SIZE * 3) {
        return -sinTable[idx - TABLE_SIZE * 2];
    }

    return -sinTable[TABLE_SIZE - (idx - TABLE_SIZE * 3)];
}

void LfoParam::Init() {
    depth = 0.0f;
    range = 1;
    speed = 6.25f;
    delay = 0;
}

void Lfo::Reset() {
    mCounter = 0.0f;
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

    mCounter += mParam.speed * msec / 1000.0f;
    mCounter -= static_cast<int>(mCounter);
}

f32 Lfo::GetValue() const {
    if (0.0f == mParam.depth) {
        return 0.0f;
    }

    if (mDelayCounter < mParam.delay) {
        return 0.0f;
    }

    f32 value = GetSinIdx(static_cast<int>(4.0f * (32.0f * mCounter))) /
                127.0f;

    value *= mParam.depth;
    value *= mParam.range;

    return value;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
