// NOTE: retail exports Channel::Start(const WaveInfo&, int, u32), but
// snd_Channel.h (outside writable scope) only declares the WaveData overload.
// Per MWCC_PATTERNS.md "Explicit retail-name entry points", the retail-named
// entry point is emitted as an extern "C" definition below, which needs member
// access. Access specifiers have no codegen/mangling effect.
#define private public
#include <nw4r/snd.h>
#undef private

// Retail Voice::Setup takes const WaveInfo&; locked header declares WaveData.
extern "C" void Setup__Q44nw4r3snd6detail5VoiceFRCQ44nw4r3snd6detail8WaveInfoUl(
    nw4r::snd::detail::Voice* self, const nw4r::snd::detail::WaveInfo& rInfo,
    u32 offset);

// Retail InitParam pools its 0.0f/1.0f literals under these sdata2 labels.
extern const f32 lbl_eu_80669F08; // 0.0f
extern const f32 lbl_eu_80669F0C; // 1.0f

// Mirror of DisposeCallback used by Channel::Stop/VoiceCallbackFunc: retail
// invokes vtable slot 0xC with a SINGLE argument (no r5 ever set), so the
// callee is declared here with one parameter.
class DisposeCallbackShim {
public:
    virtual ~DisposeCallbackShim() {}                              // at 0x8
    virtual void InvalidateData(const void* pStart) {}             // at 0xC
    virtual void InvalidateWaveData(const void* pStart,
                                    const void* pEnd) {} // at 0x10
};

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

    mInitVolume = lbl_eu_80669F0C;
    mInitPan = lbl_eu_80669F08;
    mInitSurroundPan = lbl_eu_80669F08;
    mTune = lbl_eu_80669F0C;

    mUserVolume = lbl_eu_80669F0C;
    mUserPitch = lbl_eu_80669F08;
    mUserPitchRatio = lbl_eu_80669F0C;
    mUserPan = lbl_eu_80669F08;
    mUserSurroundPan = lbl_eu_80669F08;
    mUserLpfFreq = lbl_eu_80669F08;

    mBiquadFilterType = 0;
    mBiquadFilterValue = lbl_eu_80669F08;
    mRemoteFilter = 0;
    mOutputLineFlag = OUTPUT_LINE_MAIN;

    mMainOutVolume = lbl_eu_80669F0C;
    mMainSend = lbl_eu_80669F08;

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mFxSend[i] = lbl_eu_80669F08;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = lbl_eu_80669F0C;
        mRemoteSend[i] = lbl_eu_80669F08;
        mRemoteFxSend[i] = lbl_eu_80669F08;
    }

    mSilenceVolume.InitValue(SILENCE_VOLUME_MAX);

    mSweepPitch = lbl_eu_80669F08;
    mSweepLength = 0;
    mSweepCounter = 0;

    mEnvelope.Init(lbl_eu_80669F30);
    mLfo.GetParam().Init();

    mLfoTarget = LFO_TARGET_PITCH;
    mPanMode = PAN_MODE_DUAL;
    mPanCurve = PAN_CURVE_SQRT;
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

// Retail entry point takes `const WaveInfo&` (mangled ...8WaveInfoiUl), but
// snd_Channel.h (outside writable scope) declares Start with WaveData.
// Per MWCC_PATTERNS.md "Explicit retail-name entry points", emit the exact
// retail symbol via an extern "C" definition with member ABI (this=r3).
extern "C" void Start__Q44nw4r3snd6detail7ChannelFRCQ44nw4r3snd6detail8WaveInfoiUl(
    Channel* self, const WaveInfo& rInfo, int length, u32 offset) {
    self->mLength = length;

    self->mLfo.Reset();
    // Pass the retail-pooled constant explicitly so the SDA21 reloc matches
    // (default arg resolves to the TU-local VOLUME_INIT symbol).
    self->mEnvelope.Reset(lbl_eu_80669F30);
    self->mSweepCounter = 0;

    // Retail Voice::Setup also takes WaveInfo; the locked snd_Voice.h still
    // declares WaveData, so call the retail-mangled entry point directly.
    Setup__Q44nw4r3snd6detail5VoiceFRCQ44nw4r3snd6detail8WaveInfoUl(
        self->mVoice, rInfo, offset);
    self->mVoice->Start();

    self->mActiveFlag = true;
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
    if (mVoice != NULL) {
        mVoice->Stop();
        mVoice->Free();

        mVoice = NULL;
        mPauseFlag = false;
        mActiveFlag = false;

        if (mCallback != NULL) {
            mCallback(this, CALLBACK_STATUS_STOPPED, mCallbackData);
        }

        if (mNextLink != NULL) {
            reinterpret_cast<DisposeCallbackShim*>(mNextLink)
                ->InvalidateData(reinterpret_cast<const void*>(field_0xEC));
        }

        if (mAllocFlag) {
            mAllocFlag = false;
            ChannelManager::GetInstance().Free(this);
        }
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

    if (pChannel->mNextLink != NULL) {
        reinterpret_cast<DisposeCallbackShim*>(pChannel->mNextLink)
            ->InvalidateData(
                reinterpret_cast<const void*>(pChannel->field_0xEC));
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

    // NOTE: mirrors retail control flow, including the unconditional list
    // insert (even when the pool alloc failed) and the redundant NULL
    // re-checks - both are present in the retail binary.
    Channel* pChannel =
        static_cast<Channel*>(((PoolImpl*)&mgr)->AllocImpl());

    if (pChannel != NULL) {
        // Manual in-place construction of the fresh channel.
        // Nested re-test mirrors the retail binary's dead branch.
        if (pChannel != NULL) {
            new (&pChannel->mEnvelope) EnvGenerator();
            pChannel->mLfo.GetParam().Init();

            pChannel->mLfo.mDelayCounter = 0;
            // Retail pools the 0.0f through the sdata2 label (SDA21 reloc).
            pChannel->mLfo.mCounter = lbl_eu_80669F08;
            pChannel->mPauseFlag = false;
            pChannel->mActiveFlag = false;
            pChannel->mAllocFlag = false;

            pChannel->mSilenceVolume.InitValue(0);

            pChannel->mVoice = NULL;

            // Clear the intrusive link node before it is handed to the list.
            pChannel->node.mPrev = NULL;
            pChannel->node.mNext = NULL;
        }
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
        // Second GetInstance reference: retail inlines the lazy-init guard
        // again on this path.
        ChannelManager::GetInstance();

        ((nw4r::ut::detail::LinkListImpl&)mgr.mChannelList)
            .Erase(&pChannel->node);
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

    pChannel->mCallback = NULL;
    pChannel->mCallbackData = 0;
}

} // namespace detail
} // namespace snd
} // namespace nw4r


