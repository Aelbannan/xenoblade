#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

// Decibel square table and attack table are defined in the retail data slice
// (lbl_eu_8051FC40 / lbl_eu_8051FD40, declared extern in snd_EnvGenerator.h);
// this TU must not emit them (retail split carries no data).

// VOLUME_INIT is not defined here: the retail split carries no .sdata2 and the
// retail image supplies the static member via its data slice. The ctor passes
// lbl_eu_80669F30 explicitly to match retail relocs; no function in this TU
// odr-uses the default arg.

EnvGenerator::EnvGenerator() {
    Init(lbl_eu_80669F30);
}

void EnvGenerator::Init(f32 db) {
    SetAttack(127);
    mHold = 0;
    mDecay = lbl_eu_80669F34;
    mSustain = 127;
    mRelease = lbl_eu_80669F34;
    mValue = lbl_eu_80669F38 * db;
    mStatus = STATUS_ATTACK;
}

void EnvGenerator::Reset(f32 db) {
    mValue = lbl_eu_80669F38 * db;
    mStatus = STATUS_ATTACK;
}

f32 EnvGenerator::GetValue() const {
    // Constant on the left: MWCC loads the named 0.0f into f1 before the
    // field (retail lfs f1, lbl_eu_80669F3C@sda2 first, fcmpu f1, f0).
    if (mStatus == STATUS_ATTACK && lbl_eu_80669F3C == mAttack) {
        return lbl_eu_80669F3C;
    }

    return mValue / lbl_eu_80669F38;
}

void EnvGenerator::Update(int msec) {
    switch (mStatus) {
    case STATUS_ATTACK: {
        int i = msec;

        while (i-- > 0) {
            mValue = mValue * mAttack;
            if (mValue > lbl_eu_80669F40) {
                mValue = lbl_eu_80669F3C;
                mStatus = STATUS_HOLD;
                mHoldCounter = mHold;
                return;
            }
        }
        break;
    }

    case STATUS_HOLD: {
        if (msec < mHoldCounter) {
            mHoldCounter -= msec;
        } else {
            msec -= mHoldCounter;
            mHoldCounter = 0;
            mStatus = STATUS_DECAY;
        }

        if (mStatus != STATUS_DECAY) {
            break;
        }
        // FALLTHROUGH
    }

    case STATUS_DECAY: {
        f32 target = (f32)lbl_eu_8051FC40[mSustain];
        mValue -= mDecay * (f32)msec;

        if (mValue < target) {
            mValue = target;
            mStatus = STATUS_SUSTAIN;
        }
        break;
    }

    case STATUS_SUSTAIN: {
        break;
    }

    case STATUS_RELEASE: {
        mValue -= mRelease * (f32)msec;
        break;
    }
    }
}


void EnvGenerator::SetAttack(int attack) {
    mAttack = lbl_eu_8051FD40[attack];
}

void EnvGenerator::SetDecay(int decay) {
    mDecay = CalcRelease(decay);
}

void EnvGenerator::SetSustain(int sustain) {
    mSustain = sustain;
}

void EnvGenerator::SetRelease(int release) {
    mRelease = CalcRelease(release);
}

void EnvGenerator::SetHold(int hold) {
    mHold = ((hold + 1) * (hold + 1)) / 4;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
