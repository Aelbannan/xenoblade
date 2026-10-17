#include <nw4r/snd.h>
#include <nw4r/ut.h>

#include <revolution/WPAD.h>

namespace nw4r {
namespace snd {
namespace detail {

// Retail runtime wave descriptor filled by WsdCallback::GetWaveSoundData and
// consumed by Channel::Start(const WaveInfo&, int, u32). Only numChannels,
// sampleRate and loopEnd are read here; the trailing channel params
// (channelParam[2], 0x34 each) are opaque to this function.
struct WaveDataBlock {
    u32 sampleFormat;  // at 0x00
    u8 loopFlag;       // at 0x04
    u8 _pad0x5[3];
    u32 numChannels;   // at 0x08
    u32 sampleRate;    // at 0x0C
    u32 loopStart;     // at 0x10
    u32 loopEnd;       // at 0x14
    u8 _pad0x18[0x68]; // at 0x18
};                     // sizeof 0x80

// Retail Channel::Start overload takes const WaveInfo&; the locked header only
// declares the WaveData form, so call the retail-mangled entry point directly
// (same convention as snd_Channel.cpp / snd_StrmPlayer.cpp).
extern "C" void Start__Q44nw4r3snd6detail7ChannelFRCQ44nw4r3snd6detail8WaveInfoiUl(
    Channel* pChannel, const WaveDataBlock& rData, int length, u32 offset);

// nw4r::snd::VoiceOutParam (6 floats, 0x18 bytes) - same shape as
// StrmPlayer's VoiceOutParamRetail; the parameter of Voice::SetVoiceOutParam.
struct VoiceOutParamRetail {
    f32 volume;       // at 0x0
    f32 pitch;        // at 0x4
    f32 pan;          // at 0x8
    f32 surroundPan;  // at 0xC
    f32 lpfFreq;      // at 0x10
    f32 remoteFilter; // at 0x14
};                    // sizeof 0x18

// Retail WsdPlayer::UpdateChannel reads BasicPlayer::mParam's biquad fields
// (this+0x18/0x1C) and the voice-out-param table (this+0x70) directly, and
// the Voice* out of its Channel (Channel+0xF0). mParam/mVoice are private,
// so mirror the retail layout locally (same convention as WaveDataBlock
// above and StrmPlayer's retail-layout views).
struct BasicPlayerUpdateView {
    u8 _pad0x00[0x18];                    // vtable + mVolume..mLpfFreq
    f32 biquadValue;                      // at 0x18 (PlayerParamSet::mUnk0x14)
    u8 biquadType;                        // at 0x1C (PlayerParamSet::mUnk0x18)
    u8 _pad0x1D[0x70 - 0x1D];             // ..mRemoteFxSend
    VoiceOutParamRetail voiceOutParam[4]; // at 0x70 (PlayerParamSet::mUnk0x6C)
};

struct ChannelVoiceView {
    u8 _pad0x00[0xF0];
    Voice* voice; // at 0xF0 (Channel::mVoice)
};

// Retail Voice::SetVoiceOutParam entry point (same convention as
// snd_StrmPlayer.cpp); the locked headers declare no VoiceOutParam type.
extern "C" void SetVoiceOutParam__Q44nw4r3snd6detail5VoiceFiRCQ34nw4r3snd13VoiceOutParam(
    Voice* pVoice, int index, const VoiceOutParamRetail& rParam);

WsdPlayer::WsdPlayer() : mActiveFlag(false) {}

void WsdPlayer::InitParam(int voices, const WsdCallback* pCallback,
                          u32 callbackArg) {
    BasicPlayer::InitParam();

    mStartedFlag = false;
    mPauseFlag = false;
    mReleasePriorityFixFlag = false;

    mPanRange = 1.0f;
    mVoiceOutCount = voices;
    mPriority = DEFAULT_PRIORITY;

    mCallback = pCallback;
    mCallbackData = callbackArg;

    mWsdData = NULL;
    mWsdIndex = -1;

    mWaveSoundInfo.pitch = 1.0f;
    mWaveSoundInfo.pan = 64;
    mWaveSoundInfo.surroundPan = 0;
    mWaveSoundInfo.fxSendA = 0;
    mWaveSoundInfo.fxSendB = 0;
    mWaveSoundInfo.fxSendC = 0;
    mWaveSoundInfo.mainSend = 127;

    mLfoParam.Init();
    mWavePlayFlag = false;
    mChannel = NULL;
}

bool WsdPlayer::Prepare(const void* pWsdData, int index,
                        StartOffsetType startType, int startOffset, int voices,
                        const WsdCallback* pCallback, u32 callbackArg) {
    SoundThread::AutoLock lock;

    if (mActiveFlag) {
        FinishPlayer();
    }

    InitParam(voices, pCallback, callbackArg);

    mWsdData = pWsdData;
    mWsdIndex = index;
    mStartOffsetType = startType;
    mStartOffset = startOffset;

    DisposeCallbackManager::GetInstance().RegisterDisposeCallback(this);
    mActiveFlag = true;

    return true;
}

bool WsdPlayer::Start() {
    SoundThread::AutoLock lock;

    SoundThread::GetInstance().RegisterPlayerCallback(this);
    mStartedFlag = true;

    return true;
}

void WsdPlayer::Stop() {
    SoundThread::AutoLock lock;

    FinishPlayer();
}

void WsdPlayer::Pause(bool flag) {
    SoundThread::AutoLock lock;

    mPauseFlag = static_cast<u8>(flag);

    if (IsChannelActive() && flag != mChannel->IsPause()) {
        mChannel->Pause(flag);
    }
}

void WsdPlayer::SetChannelPriority(int priority) {
    mPriority = static_cast<u8>(priority);
}

void WsdPlayer::SetReleasePriorityFix(bool flag) {
    mReleasePriorityFixFlag = flag;
}

void WsdPlayer::InvalidateData(const void* pStart, const void* pEnd) {
    SoundThread::AutoLock lock;

    if (mActiveFlag) {
        const void* pWsdData = GetWsdDataAddress();

        if (pStart <= pWsdData && pWsdData <= pEnd) {
            FinishPlayer();
        }
    }
}

void WsdPlayer::FinishPlayer() {
    SoundThread::AutoLock lock;

    if (mStartedFlag) {
        SoundThread::GetInstance().UnregisterPlayerCallback(this);
        mStartedFlag = false;
    }

    if (mActiveFlag) {
        DisposeCallbackManager::GetInstance().UnregisterDisposeCallback(this);
        mActiveFlag = false;
    }

    CloseChannel();
}

void WsdPlayer::Update() {
    SoundThread::AutoLock lock;

    if (!mActiveFlag) {
        return;
    }

    if (!mStartedFlag) {
        return;
    }

    if (!mPauseFlag) {
        if (mWavePlayFlag && mChannel == NULL) {
            FinishPlayer();
            return;
        }

        if (!mWavePlayFlag && !StartChannel(mCallback, mCallbackData)) {
            FinishPlayer();
            return;
        }
    }

    UpdateChannel();
}

bool WsdPlayer::StartChannel(const WsdCallback* pCallback, u32 callbackArg) {
    SoundThread::AutoLock lock;

    int priority = GetChannelPriority() + DEFAULT_PRIORITY;

    WaveDataBlock waveData;
    WaveSoundNoteInfo waveSoundNoteInfo;
    if (!pCallback->GetWaveSoundData(
            &mWaveSoundInfo, &waveSoundNoteInfo,
            reinterpret_cast<WaveInfo*>(&waveData), GetWsdDataAddress(),
            mWsdIndex, 0, callbackArg)) {
        return false;
    }

    u32 startSample;
    if (mStartOffsetType == START_OFFSET_TYPE_SAMPLE) {
        startSample = mStartOffset;
    } else if (mStartOffsetType == START_OFFSET_TYPE_MILLISEC) {
        startSample = (static_cast<s64>(mStartOffset) *
                       static_cast<s32>(waveData.sampleRate)) /
                      1000;
    }

    if (startSample > waveData.loopEnd) {
        return false;
    }

    Channel* pChannel = Channel::AllocChannel(
        ut::Min<int>(waveData.numChannels, CHANNEL_MAX), GetVoiceOutCount(),
        priority, ChannelCallbackFunc, reinterpret_cast<u32>(this));

    if (pChannel == NULL) {
        return false;
    }

    pChannel->SetAttack(waveSoundNoteInfo.attack);
    pChannel->SetHold(waveSoundNoteInfo.hold);
    pChannel->SetDecay(waveSoundNoteInfo.decay);
    pChannel->SetSustain(waveSoundNoteInfo.sustain);
    pChannel->SetRelease(waveSoundNoteInfo.release);
    // Inline setter (Channel.h): retail emits a direct byte store to +0x39.
    pChannel->SetReleasePriorityFix(mReleasePriorityFixFlag);

    Start__Q44nw4r3snd6detail7ChannelFRCQ44nw4r3snd6detail8WaveInfoiUl(
        pChannel, waveData, -1, startSample);
    mChannel = pChannel;
    mWavePlayFlag = true;

    return true;
}

void WsdPlayer::CloseChannel() {
    SoundThread::AutoLock lock;

    if (IsChannelActive()) {
        UpdateChannel();
        mChannel->Release();
    }

    if (mChannel != NULL) {
        Channel::FreeChannel(mChannel);
    }

    mChannel = NULL;
}

void WsdPlayer::UpdateChannel() {
    SoundThread::AutoLock lock;

    if (mChannel == NULL) {
        return;
    }

    // Declaration order drives saved-FPR color (first-declared takes the
    // highest register); the initializations stay in computation order.
    f32 volume;
    f32 pitchRatio;
    f32 pan;
    f32 surroundPan;
    f32 lpfFreq;
    f32 biquadValue;
    f32 mainSend;

    pitchRatio = 1.0f;
    pitchRatio *= GetPitch();
    pitchRatio *= mWaveSoundInfo.pitch;

    volume = 1.0f;
    volume *= GetVolume();

    pan = 0.0f;
    if (mWaveSoundInfo.pan <= 1) {
        pan += (mWaveSoundInfo.pan - 63) / 63.0f;
    } else {
        pan += (mWaveSoundInfo.pan - 64) / 63.0f;
    }

    pan *= GetPanRange();
    pan += GetPan();

    surroundPan = 0.0f;
    if (mWaveSoundInfo.surroundPan <= 1) {
        surroundPan += (mWaveSoundInfo.surroundPan + 1) / 63.0f;
    } else {
        surroundPan += mWaveSoundInfo.surroundPan / 63.0f;
    }

    const BasicPlayerUpdateView* pParamView =
        reinterpret_cast<const BasicPlayerUpdateView*>(this);
    biquadValue = pParamView->biquadValue;

    surroundPan += mWaveSoundInfo.surroundPan / 64.0f;
    surroundPan += GetSurroundPan();

    mainSend = 0.0f;
    mainSend += (mWaveSoundInfo.mainSend / 127.0f) - 1.0f;
    mainSend += GetMainSend();

    lpfFreq = 0.0f;
    lpfFreq += GetLpfFreq();

    int remoteFilter;
    remoteFilter = GetRemoteFilter();

    u8 infoSend[AUX_BUS_NUM];
    infoSend[AUX_A] = mWaveSoundInfo.fxSendA;
    infoSend[AUX_B] = mWaveSoundInfo.fxSendB;
    infoSend[AUX_C] = mWaveSoundInfo.fxSendC;

    int biquadType;
    biquadType = pParamView->biquadType;

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += infoSend[i] / 127.0f;
        fxSend[i] += GetFxSend(static_cast<AuxBus>(i));
    }

    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteSend[i] = 0.0f;
        remoteSend[i] += GetRemoteSend(i);

        remoteFxSend[i] = 0.0f;
        remoteFxSend[i] += GetRemoteFxSend(i);
    }

    mChannel->SetPanMode(GetPanMode());
    mChannel->SetPanCurve(GetPanCurve());
    mChannel->SetUserVolume(volume);
    mChannel->SetUserPitchRatio(pitchRatio);
    mChannel->SetUserPan(pan);
    mChannel->SetUserSurroundPan(surroundPan);
    mChannel->SetUserLpfFreq(lpfFreq);
    mChannel->SetBiquadFilter(biquadType, biquadValue);
    mChannel->SetRemoteFilter(remoteFilter);
    mChannel->SetOutputLine(GetOutputLine());
    mChannel->SetMainOutVolume(GetMainOutVolume());
    mChannel->SetMainSend(mainSend);

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mChannel->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mChannel->SetRemoteOutVolume(i, GetRemoteOutVolume(i));
        mChannel->SetRemoteSend(i, remoteSend[i]);
        mChannel->SetRemoteFxSend(i, remoteFxSend[i]);
    }

    for (int i = 0; i < GetVoiceOutCount(); i++) {
        Voice* pVoice =
            reinterpret_cast<const ChannelVoiceView*>(mChannel)->voice;
        SetVoiceOutParam__Q44nw4r3snd6detail5VoiceFiRCQ34nw4r3snd13VoiceOutParam(
            pVoice, i,
            reinterpret_cast<const BasicPlayerUpdateView*>(this)->voiceOutParam[i]);
    }

    mChannel->SetLfoParam(mLfoParam);
}

void WsdPlayer::ChannelCallbackFunc(Channel* pDropChannel,
                                    Channel::ChannelCallbackStatus status,
                                    u32 callbackArg) {
    SoundThread::AutoLock lock;

    WsdPlayer* p = reinterpret_cast<WsdPlayer*>(callbackArg);

    if (status == Channel::CALLBACK_STATUS_FINISH) {
        Channel::FreeChannel(pDropChannel);
    }

    p->mChannel = NULL;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

