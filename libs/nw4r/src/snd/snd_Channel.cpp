#include <nw4r/snd.h>

#include <revolution/AX.h>

namespace nw4r {
namespace snd {
namespace detail {

ChannelManager& ChannelManager::GetInstance() {
    static ChannelManager instance;
    return instance;
}

ChannelManager::ChannelManager() : mInitialized(false), mChannelCount(0) {}

ChannelManager::~ChannelManager() {}

u32 ChannelManager::GetRequiredMemSize() {
    return (AXGetMaxVoices() + VOICE_MARGIN) * sizeof(Channel);
}

u32 ChannelManager::GetRequiredMemSize(int channelCount) {
    return (channelCount + 1) * 256;
}

void ChannelManager::Setup(void* pWork, u32 workSize) {
    ut::AutoInterruptLock lock;

    if (mInitialized) {
        return;
    }

    mChannelCount = mPool.Create(pWork, workSize);
    mMem = pWork;
    mMemSize = workSize;
    mInitialized = true;
}

void ChannelManager::Shutdown() {
    ut::AutoInterruptLock lock;

    if (!mInitialized) {
        return;
    }

    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mChannelList, { it->Stop(); })

    mPool.Destroy(mMem, mMemSize);
    mInitialized = false;
}

Channel* ChannelManager::Alloc() {
    Channel* pChannel = mPool.Alloc();
    mChannelList.PushBack(pChannel);
    return pChannel;
}

void ChannelManager::Free(Channel* pChannel) {
    mChannelList.Erase(pChannel);
    mPool.Free(pChannel);
}

void ChannelManager::UpdateAllChannel() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, mChannelList, { it->Update(true); }); //
}

Channel::Channel()
    : mPauseFlag(false), mActiveFlag(false), mAllocFlag(false), mVoice(NULL) {}

Channel::~Channel() {}

void Channel::InitParam(ChannelCallback pCallback, u32 callbackArg) {
    mNextLink = NULL;

    mCallback = pCallback;
    mCallbackData = callbackArg;

    mPauseFlag = false;
    mAutoSweep = true;
    mReleasePriorityFixFlag = false;

    mLength = 0;
    mKey = KEY_INIT;
    mOriginalKey = ORIGINAL_KEY_INIT;

    mInitVolume = 1.0f;
    mInitPan = 0.0f;
    mInitSurroundPan = 0.0f;
    mTune = 1.0f;

    mUserVolume = 1.0f;
    mUserPitch = 0.0f;
    mUserPitchRatio = 1.0f;
    mUserPan = 0.0f;
    mUserSurroundPan = 0.0f;
    mUserLpfFreq = 0.0f;

    mRemoteFilter = 0;
    mOutputLineFlag = OUTPUT_LINE_MAIN;

    mMainOutVolume = 1.0f;
    mMainSend = 0.0f;

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = 0.0f;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
        mRemoteSend[i] = 0.0f;
        mRemoteFxSend[i] = 0.0f;
    }

    mSilenceVolume.InitValue(SILENCE_VOLUME_MAX);

    mSweepPitch = 0.0f;
    mSweepLength = 0;
    mSweepCounter = 0;

    mEnvelope.Init();
    mLfo.GetParam().Init();

    mLfoTarget = LFO_TARGET_PITCH;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;
}

void Channel::Update(bool periodic) {
    if (!mActiveFlag) {
        return;
    }

    if (mPauseFlag) {
        periodic = false;
    }

    f32 lfoValue = mLfo.GetValue();
    mSilenceVolume.Update();

    f32 volume = 1.0f;
    volume *= mInitVolume;
    volume *= mUserVolume;
    volume *=
        static_cast<f32>(mSilenceVolume.GetValue()) / SILENCE_VOLUME_MAX;

    f32 veInitVolume = 1.0f;
    veInitVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());
    if (mLfoTarget == LFO_TARGET_VOLUME) {
        veInitVolume *=
            Util::CalcVolumeRatio(VOLUME_MAX_DB * lfoValue);
    }

    if (mEnvelope.GetStatus() == EnvGenerator::STATUS_RELEASE) {
        if (mCallback != NULL) {
            if (veInitVolume == 0.0f) {
                Stop();
                return;
            }
        } else if (volume * veInitVolume == 0.0f) {
            Stop();
            return;
        }
    }

    f32 cent = 0.0f;
    cent += static_cast<f32>(mKey - mOriginalKey);
    cent += GetSweepValue();
    cent += mUserPitch;
    if (mLfoTarget == LFO_TARGET_PITCH) {
        cent += lfoValue;
    }

    f32 pitchRatio = 1.0f;
    pitchRatio *= mTune;
    pitchRatio *= mUserPitchRatio;

    f32 pitch =
        Util::CalcPitchRatio(static_cast<int>(Util::MICROTONE_MAX * cent));
    pitch *= pitchRatio;

    f32 pan = 0.0f;
    pan += mInitPan;
    pan += mUserPan;
    if (mLfoTarget == LFO_TARGET_PAN) {
        pan += lfoValue;
    }

    f32 surroundPan = 0.0f;
    surroundPan += mInitSurroundPan;
    surroundPan += mUserSurroundPan;

    f32 lpfFreq = 0.0f;
    lpfFreq += mUserLpfFreq;

    int remoteFilter = mRemoteFilter;

    f32 mainOutVolume = 1.0f;
    mainOutVolume *= mMainOutVolume;

    f32 mainSend = 0.0f;
    mainSend += mMainSend;

    f32 fxSend[AUX_BUS_NUM];
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        fxSend[i] = 0.0f;
        fxSend[i] += mFxSend[i];
    }

    f32 remoteOutVolume[WPAD_MAX_CONTROLLERS];
    f32 remoteSend[WPAD_MAX_CONTROLLERS];
    f32 remoteFxSend[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVolume[i] = 1.0f;
        remoteOutVolume[i] *= mRemoteOutVolume[i];

        remoteSend[i] = 0.0f;
        remoteSend[i] += mRemoteSend[i];

        remoteFxSend[i] = 0.0f;
        remoteFxSend[i] += mRemoteFxSend[i];
    }

    if (periodic) {
        if (mAutoSweep) {
            UpdateSweep(3);
        }

        mLfo.Update(3);
        mEnvelope.Update(3);
    }

    f32 nextLfoValue = mLfo.GetValue();

    f32 veTargetVolume = 1.0f;
    veTargetVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());
    if (mLfoTarget == LFO_TARGET_VOLUME) {
        veTargetVolume *=
            Util::CalcVolumeRatio(VOLUME_MAX_DB * nextLfoValue);
    }

    if (mVoice != NULL) {
        mVoice->SetPanMode(mPanMode);
        mVoice->SetPanCurve(mPanCurve);
        mVoice->SetVolume(volume);
        mVoice->SetVeVolume(veTargetVolume, veInitVolume);
        mVoice->SetPitch(pitch);
        mVoice->SetPan(pan);
        mVoice->SetSurroundPan(surroundPan);
        mVoice->SetLpfFreq(lpfFreq);
        mVoice->SetRemoteFilter(remoteFilter);
        mVoice->SetOutputLine(mOutputLineFlag);
        mVoice->SetMainOutVolume(mainOutVolume);
        mVoice->SetMainSend(mainSend);

        for (int i = 0; i < AUX_BUS_NUM; i++) {
            mVoice->SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
        }

        for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
            mVoice->SetRemoteOutVolume(i, remoteOutVolume[i]);
            mVoice->SetRemoteSend(i, remoteSend[i]);
            mVoice->SetRemoteFxSend(i, remoteFxSend[i]);
        }
    }
}

void Channel::Start(const WaveData& rInfo, int length, u32 offset) {
    mLength = length;

    mLfo.Reset();
    mEnvelope.Reset();
    mSweepCounter = 0;

    mVoice->Setup(rInfo, offset);
    mVoice->Start();
    mActiveFlag = true;
}

void Channel::Release() {
    if (mEnvelope.GetStatus() != EnvGenerator::STATUS_RELEASE) {
        if (mVoice != NULL && !mReleasePriorityFixFlag) {
            mVoice->SetPriority(PRIORITY_RELEASE);
        }

        mEnvelope.SetStatus(EnvGenerator::STATUS_RELEASE);
    }

    mPauseFlag = false;
}

void Channel::NoteOff() {
    if (field_0x3A) {
        return;
    }

    if (mEnvelope.GetStatus() != EnvGenerator::STATUS_RELEASE) {
        if (mVoice != NULL && !mReleasePriorityFixFlag) {
            mVoice->SetPriority(PRIORITY_RELEASE);
        }

        mEnvelope.SetStatus(EnvGenerator::STATUS_RELEASE);
    }

    mPauseFlag = false;
}

void Channel::Stop() {
    if (mVoice == NULL) {
        return;
    }

    mVoice->Stop();
    mVoice->Free();

    mVoice = NULL;
    mPauseFlag = false;
    mActiveFlag = false;

    if (mCallbackData != 0) {
        reinterpret_cast<ChannelCallback>(mCallbackData)(
            this, CALLBACK_STATUS_STOPPED, reinterpret_cast<u32>(mVoice));
    }

    if (mNextLink != NULL) {
        reinterpret_cast<DisposeCallback*>(mNextLink)
            ->InvalidateData(reinterpret_cast<const void*>(field_0xEC), NULL);
    }

    if (mAllocFlag) {
        mAllocFlag = false;
        ChannelManager::GetInstance().Free(this);
    }
}

void Channel::UpdateSweep(int count) {
    mSweepCounter += count;

    if (mSweepCounter > mSweepLength) {
        mSweepCounter = mSweepLength;
    }
}

void Channel::SetSweepParam(f32 pitch, int time, bool autoUpdate) {
    mSweepPitch = pitch;
    mSweepLength = time;
    mAutoSweep = autoUpdate;
    mSweepCounter = 0;
}

void Channel::SetBiquadFilter(int type, f32 value) {
    mBiquadFilterType = type;
    mBiquadFilterValue = value;
}

f32 Channel::GetSweepValue() const {
    if (mSweepPitch == 0.0f) {
        return 0.0f;
    }

    if (mSweepCounter >= mSweepLength) {
        return 0.0f;
    }

    f32 sweep = mSweepPitch;
    sweep *= mSweepLength - mSweepCounter;
    sweep /= mSweepLength;

    return sweep;
}

void Channel::VoiceCallbackFunc(Voice* pDropVoice,
                                Voice::VoiceCallbackStatus status,
                                void* pCallbackArg) {
    ChannelCallbackStatus channelStatus;

    switch (status) {
    case Voice::CALLBACK_STATUS_FINISH_WAVE: {
        channelStatus = CALLBACK_STATUS_FINISH;
        pDropVoice->Free();
        break;
    }

    case Voice::CALLBACK_STATUS_CANCEL: {
        channelStatus = CALLBACK_STATUS_CANCEL;
        pDropVoice->Free();
        break;
    }
    case Voice::CALLBACK_STATUS_DROP_VOICE: {
        channelStatus = CALLBACK_STATUS_DROP;
        break;
    }

    case Voice::CALLBACK_STATUS_DROP_DSP: {
        channelStatus = CALLBACK_STATUS_DROP;
        break;
    }
    }

    Channel* pChannel = static_cast<Channel*>(pCallbackArg);

    if (pChannel->mCallback != NULL) {
        pChannel->mCallback(pChannel, channelStatus, pChannel->mCallbackData);
    }

    pChannel->mVoice = NULL;
    pChannel->mPauseFlag = false;
    pChannel->mActiveFlag = false;
    pChannel->mAllocFlag = false;

    ChannelManager::GetInstance().Free(pChannel);
}

Channel* Channel::AllocChannel(int channels, int voices, int priority,
                               ChannelCallback pCallback, u32 callbackArg) {
    ChannelManager& mgr = ChannelManager::GetInstance();

    void* p = ((PoolImpl*)&mgr)->AllocImpl();
    Channel* pChannel;
    if (p != NULL) {
        pChannel = static_cast<Channel*>(p);
        // Manual in-place construction
        new (&pChannel->mEnvelope) EnvGenerator();
        pChannel->mLfo.GetParam().Init();

        pChannel->mLfo.mDelayCounter = 0;
        pChannel->mLfo.mCounter = 0.0f;
        pChannel->mPauseFlag = false;
        pChannel->mActiveFlag = false;
        pChannel->mAllocFlag = false;

        // Zero the MoveValue<u8, u16> at 0xC0
        *(u8*)((u8*)pChannel + 0xC0) = 0;
        *(u8*)((u8*)pChannel + 0xC1) = 0;
        *(u16*)((u8*)pChannel + 0xC2) = 0;
        *(u16*)((u8*)pChannel + 0xC4) = 0;

        pChannel->mVoice = NULL;

        // Zero the link list node at 0xF8
        *(u32*)((u8*)pChannel + 0xF8) = 0;
        *(u32*)((u8*)pChannel + 0xFC) = 0;
    } else {
        pChannel = NULL;
    }

    // Insert into ChannelManager's list (reached from both branches)
    ((nw4r::ut::detail::LinkListImpl&)mgr.mChannelList)
        .Insert(((nw4r::ut::detail::LinkListImpl&)mgr.mChannelList).GetEndIter(),
                &pChannel->node);

    if (pChannel == NULL) {
        return NULL;
    }

    pChannel->mAllocFlag = true;

    Voice* pVoice = VoiceManager::GetInstance().AllocVoice(
        channels, voices, priority, VoiceCallbackFunc, pChannel);

    if (pVoice == NULL) {
        // Trigger lazy init (guard check) before cleanup
        ChannelManager::GetInstance();
        ((nw4r::ut::detail::LinkListImpl&)mgr.mChannelList).Erase(&pChannel->node);
        if (pChannel != NULL) {
            ((PoolImpl*)&mgr)->FreeImpl(pChannel);
        }
        return NULL;
    }

    pChannel->mVoice = pVoice;
    pChannel->InitParam(pCallback, callbackArg);
    return pChannel;
}

void Channel::FreeChannel(Channel* pChannel) {
    if (pChannel == NULL) {
        return;
    }

    pChannel->mCallbackData = 0;
    pChannel->mVoice = NULL;
}

} // namespace detail
} // namespace snd
} // namespace nw4r


