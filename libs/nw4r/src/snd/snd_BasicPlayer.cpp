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
    // Fill the per-voice output groups via a bounded walk (retail keeps a
    // rolled fill loop with runtime trip counts; the head peels).
    VoiceOutParam* it = &mParam.mVoiceOutParam[0];

    do {
        it->volume = lbl_eu_80669EE0;
        it->pitch = lbl_eu_80669EE0;
        it->pan = lbl_eu_80669EE4;
        it->surroundPan = lbl_eu_80669EE4;
        it->fxSend = lbl_eu_80669EE4;
        it->lpfFreq = lbl_eu_80669EE4;
        it++;
    } while (it < &mParam.mVoiceOutParam[WPAD_MAX_CONTROLLERS]);

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
        mVoiceOutParam[i].volume = one;
        mVoiceOutParam[i].pitch = one;
        mVoiceOutParam[i].pan = zero;
        mVoiceOutParam[i].surroundPan = zero;
        mVoiceOutParam[i].fxSend = zero;
        mVoiceOutParam[i].lpfFreq = zero;
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
