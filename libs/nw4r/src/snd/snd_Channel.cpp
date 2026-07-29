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
    BOOL enabled = OSDisableInterrupts();

    if (!mInitialized) {
        OSRestoreInterrupts(enabled);
        return;
    }

    NW4R_UT_LINKLIST_FOREACH_SAFE(it, mChannelList, {
        if (it->mVoice != NULL) {
            it->mVoice->Stop();
            it->mVoice->Free();
            it->mVoice = NULL;
            it->mPauseFlag = false;
            it->mActiveFlag = false;
        }

        if (it->mCallback != NULL) {
            it->mCallback(&*it, Channel::CALLBACK_STATUS_STOPPED,
                          it->mCallbackData);
        }

        if (it->mNextLink != NULL) {
            reinterpret_cast<DisposeCallback*>(it->mNextLink)
                ->InvalidateData(
                    reinterpret_cast<const void*>(it->field_0xEC),
                    reinterpret_cast<const void*>(it->mCallbackData));
        }

        if (it->mAllocFlag) {
            it->mAllocFlag = false;

            mChannelList.Erase(&*it);
        }
    });

    mPool.Destroy(mMem, mMemSize);
    mInitialized = false;

    OSRestoreInterrupts(enabled);
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
    field_0xF4 = 0;
    mCallback = pCallback;
    mCallbackData = callbackArg;
    mNextLink = NULL;
    field_0xEC = 0;
    mPauseFlag = false;
    mAutoSweep = true;
    mReleasePriorityFixFlag = false;
    field_0x3A = false;
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
    mLfoTarget = LFO_TARGET_PITCH;
    mBiquadFilterValue = 0.0f;
    mPanMode = PAN_MODE_DUAL;
    mOutputLineFlag = OUTPUT_LINE_MAIN;
    mMainOutVolume = 1.0f;
    mMainSend = 0.0f;
    mFxSend[0] = 0.0f;
    mFxSend[1] = 0.0f;
    mFxSend[2] = 0.0f;
    mRemoteOutVolume[0] = 1.0f;
    mRemoteSend[0] = 0.0f;
    mRemoteFxSend[0] = 0.0f;
    mRemoteOutVolume[1] = 1.0f;
    mRemoteSend[1] = 0.0f;
    mRemoteFxSend[1] = 0.0f;
    mRemoteOutVolume[2] = 1.0f;
    mRemoteSend[2] = 0.0f;
    mRemoteFxSend[2] = 0.0f;
    mRemoteOutVolume[3] = 1.0f;
    mRemoteSend[3] = 0.0f;
    mRemoteFxSend[3] = 0.0f;
    mSilenceVolume.InitValue(SILENCE_VOLUME_MAX);
    mSweepPitch = 0.0f;
    mSweepLength = 0;
    mSweepCounter = 0;
    mEnvelope.Init();
    mLfo.GetParam().Init();
    mBiquadFilterType = 0;
    field_0xD4 = 0;
    field_0xD8 = 0;
    field_0xDC = 0;
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
    volume *= mSilenceVolume.GetValue() / static_cast<f32>(SILENCE_VOLUME_MAX);

    f32 veInitVolume = 1.0f;
    veInitVolume *= Util::CalcVolumeRatio(mEnvelope.GetValue());
    if (mLfoTarget == LFO_TARGET_VOLUME) {
        veInitVolume *= Util::CalcVolumeRatio(VOLUME_MAX_DB * lfoValue);
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
    cent += mKey - mOriginalKey;
    cent += GetSweepValue();
    cent += mUserPitch;
    if (mLfoTarget == LFO_TARGET_PITCH) {
        cent += lfoValue;
    }

    f32 pitchRatio = 1.0f;
    pitchRatio *= mTune;
    pitchRatio *= mUserPitchRatio;

    f32 pitch = Util::CalcPitchRatio(Util::MICROTONE_MAX * cent);
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

    f32 lpfFreq = 1.0f;
    lpfFreq += mUserLpfFreq;

    int remoteFilter = 0;
    remoteFilter += mRemoteFilter;

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
        veTargetVolume *= Util::CalcVolumeRatio(VOLUME_MAX_DB * nextLfoValue);
    }

    if (mVoice != NULL) {
        mVoice->SetPanMode(static_cast<PanMode>(mPanMode));
        mVoice->SetPanCurve(static_cast<PanCurve>(mPanCurve));
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
            // @bug Should use remoteFxSend
            mVoice->SetRemoteFxSend(i, remoteSend[i]);
        }
    }
}

void Channel::Start(const WaveData& rData, int length, u32 offset) {
    mLength = length;

    mLfo.Reset();
    mEnvelope.Reset();
    mSweepCounter = 0;

    mVoice->Setup(rData, offset);
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

    if (mCallback != NULL) {
        mCallback(this, CALLBACK_STATUS_STOPPED, mCallbackData);
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
    Channel* pChannel = static_cast<Channel*>(pCallbackArg);
    ChannelCallbackStatus channelStatus;

    if (status == Voice::CALLBACK_STATUS_DROP_VOICE) {
        channelStatus = CALLBACK_STATUS_DROP;
        goto cleanup;
    }
    if (status >= Voice::CALLBACK_STATUS_DROP_VOICE) {
        if (status >= Voice::CALLBACK_STATUS_DROP_DSP + 1) {
            goto cleanup;
        }
        channelStatus = CALLBACK_STATUS_DROP;
        goto cleanup;
    }
    if (status == Voice::CALLBACK_STATUS_FINISH_WAVE) {
        channelStatus = CALLBACK_STATUS_FINISH;
        pDropVoice->Free();
        goto cleanup;
    }
    channelStatus = CALLBACK_STATUS_CANCEL;
    pDropVoice->Free();

cleanup:
    if (pChannel->mCallback != NULL) {
        pChannel->mCallback(pChannel, channelStatus, pChannel->mCallbackData);
    }

    if (pChannel->mNextLink != NULL) {
        reinterpret_cast<DisposeCallback*>(pChannel->mNextLink)
            ->InvalidateData(reinterpret_cast<const void*>(pChannel->field_0xEC), NULL);
    }

    pChannel->mVoice = NULL;
    pChannel->mPauseFlag = false;
    pChannel->mActiveFlag = false;
    pChannel->mAllocFlag = false;

    ChannelManager::GetInstance().Free(pChannel);
}

Channel* Channel::AllocChannel(int channels, int voices, int priority,
                               ChannelCallback pCallback, u32 callbackArg) {
    Channel* pChannel = ChannelManager::GetInstance().Alloc();
    if (pChannel == NULL) {
        return NULL;
    }

    pChannel->mAllocFlag = true;

    Voice* pVoice = VoiceManager::GetInstance().AllocVoice(
        channels, voices, priority, VoiceCallbackFunc, pChannel);

    if (pVoice == NULL) {
        ChannelManager::GetInstance().Free(pChannel);
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


