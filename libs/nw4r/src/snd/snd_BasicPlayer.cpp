#include <nw4r/snd.h>

// Retail .sdata2 pools for PlayerParamSet::Init (US target relocs).
extern "C" {
extern const f32 lbl_eu_80669EE0; // 1.0f
extern const f32 lbl_eu_80669EE4; // 0.0f
}

namespace nw4r {
namespace snd {
namespace detail {

BasicPlayer::BasicPlayer() {
    // Fill the remote voice-out groups via a bounded walk (retail keeps
    // a rolled fill loop with runtime trip counts; the head peels).
    f32* it = &mParam.mUnk0x6C[0];

    do {
        it[0] = lbl_eu_80669EE0;
        it[1] = lbl_eu_80669EE0;
        it[2] = lbl_eu_80669EE4;
        it[3] = lbl_eu_80669EE4;
        it[4] = lbl_eu_80669EE4;
        it[5] = lbl_eu_80669EE4;
        it += 6;
    } while (it < &mParam.mUnk0x6C[WPAD_MAX_CONTROLLERS * 6]);

    mParam.Init();

    mId = BasicSound::INVALID_ID;

    mParam.Init();
}

void PlayerParamSet::Init() {
    const f32 zero = lbl_eu_80669EE4;
    const f32 one = lbl_eu_80669EE0;

    // Store order matches US retail (pan before volume/pitch; filter fields
    // around mBiquadFreq; remotes interleaved per index).
    mPan = zero;
    mVolume = one;
    mPitch = one;
    mSurroundPan = zero;
    mLpfFreq = zero;
    mBiquadType = 0;
    mBiquadFreq = zero;
    mRemoteFilter = 0;
    mOutputLine = OUTPUT_LINE_MAIN;
    mMainOutVolume = one;
    mMainSend = zero;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;

    mFxSend[0] = zero;
    mFxSend[1] = zero;
    mFxSend[2] = zero;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = one;
        mRemoteSend[i] = zero;
        mRemoteFxSend[i] = zero;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        int base = i * 6;
        mUnk0x6C[base + 0] = one;
        mUnk0x6C[base + 1] = one;
        mUnk0x6C[base + 2] = zero;
        mUnk0x6C[base + 3] = zero;
        mUnk0x6C[base + 4] = zero;
        mUnk0x6C[base + 5] = zero;
    }
}

void BasicPlayer::InitParam() {
    mParam.Init();
}

void BasicPlayer::SetFxSend(AuxBus bus, f32 send) {
    mParam.mFxSend[bus] = send;
}

f32 BasicPlayer::GetFxSend(AuxBus bus) const {
    return mParam.mFxSend[bus];
}

void BasicPlayer::SetRemoteOutVolume(int remote, f32 volume) {
    mParam.mRemoteOutVolume[remote] = volume;
}

f32 BasicPlayer::GetRemoteOutVolume(int remote) const {
    return mParam.mRemoteOutVolume[remote];
}

f32 BasicPlayer::GetRemoteSend(int remote) const {
    return mParam.mRemoteSend[remote];
}

f32 BasicPlayer::GetRemoteFxSend(int remote) const {
    return mParam.mRemoteFxSend[remote];
}

void BasicPlayer::SetRemoteFilter(int filter) {
    mParam.mRemoteFilter = filter;
}

void BasicPlayer::SetBiquadFilter(int filter, f32 value) {
    mParam.mBiquadType = filter;
    mParam.mBiquadFreq = value;
}

} // namespace detail
} // namespace snd
} // namespace nw4r
