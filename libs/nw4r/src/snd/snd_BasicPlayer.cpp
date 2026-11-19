#include <nw4r/snd.h>

namespace nw4r {
namespace snd {
namespace detail {

BasicPlayer::BasicPlayer() : mId(BasicSound::INVALID_ID) {
    mParam.Init();
}

void PlayerParamSet::Init() {
    mVolume = 1.0f;
    mPitch = 1.0f;
    mPan = 0.0f;
    mSurroundPan = 0.0f;
    mLpfFreq = 0.0f;
    mBiquadType = 0;
    mBiquadFreq = 0.0f;
    mRemoteFilter = 0;
    mOutputLine = OUTPUT_LINE_MAIN;
    mMainOutVolume = 1.0f;
    mMainSend = 0.0f;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;

    mFxSend[0] = 0.0f;
    mFxSend[1] = 0.0f;
    mFxSend[2] = 0.0f;

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
        mRemoteSend[i] = 0.0f;
        mRemoteFxSend[i] = 0.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mVoiceOutParam[i].volume = 1.0f;
        mVoiceOutParam[i].pitch = 1.0f;
        mVoiceOutParam[i].pan = 0.0f;
        mVoiceOutParam[i].surroundPan = 0.0f;
        mVoiceOutParam[i].fxSend = 0.0f;
        mVoiceOutParam[i].lpfFreq = 0.0f;
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
