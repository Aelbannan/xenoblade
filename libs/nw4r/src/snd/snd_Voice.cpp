#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// Node-anchored mirror used by the VoiceList scan in Acquire(). Retail reads
// the voice's priority / voice-count fields at fixed NEGATIVE offsets off the
// list node pointer (retail embeds the LinkListNode at 0x11C, so e.g. node-0x74
// is mPriority). The snd_Voice.h class places `node` at a different offset, so
// these reads are routed through this node-relative mirror instead.
struct VoiceNodeLayout {
    int mChannelCount; // 0x0  (node - 0x90)
    int mVoiceOutCount; // 0x4  (node - 0x8c)
    u8 _pad[0x14];     // 0x8..0x1b
    int mPriority;     // 0x1c (node - 0x74)
};

// Retail-accurate mirror of the Voice object field layout. The class defined
// in snd_Voice.h has different (non-retail) member offsets, so the setter/loop
// helpers below access the fields through this mirror at their exact retail
// offsets (mSyncFlag=0xA2, mBiquadType=0xA5, mBiquadFreq=0xB8, ...).
struct VoiceLayout {
    u8 _pad0[0xC];                                 // 0x0
    AxVoice* mAxVoice[CHANNEL_MAX][4];            // 0xC  (VOICES_MAX=4)
    float mVoiceOutParam[4][6];                   // 0x2C  (VOICES_MAX=4, 6 f32/elt)
    int mChannelCount;                             // 0x8C
    int mVoiceOutCount;                            // 0x90
    void* mCallback;                               // 0x94
    void* mCallbackArg;                            // 0x98
    bool mIsActive;                                // 0x9C
    bool mIsStarting;                              // 0x9D
    bool mIsStarted;                               // 0x9E
    bool mIsPause;                                 // 0x9F
    bool mIsPausing;                               // 0xA0
    u8 field_0xA1;                                 // 0xA1
    u16 mSyncFlag;                                 // 0xA2
    u8 mRemoteFilter;                              // 0xA4
    u8 mBiquadType;                                // 0xA5
    int mPriority;                                 // 0xA8
    float mPan;                                    // 0xAC
    float mSurroundPan;                            // 0xB0
    float mLpfFreq;                                // 0xB4
    float mBiquadFreq;                             // 0xB8
    int mOutputLineFlag;                           // 0xBC
    float mMainOutVolume;                          // 0xC0
    float mMainSend;                               // 0xC4
    float mFxSend[AUX_BUS_NUM];                    // 0xC8
    float mRemoteOutVolume[WPAD_MAX_CONTROLLERS];  // 0xD4
    float mRemoteSend[WPAD_MAX_CONTROLLERS];       // 0xE4
    float mRemoteFxSend[WPAD_MAX_CONTROLLERS];     // 0xF4
    float mPitch;                                  // 0x104
    float mVolume;                                 // 0x108
    float mVeInitVolume;                           // 0x10C
    float mVeTargetVolume;                         // 0x110
    int field_0x114;                               // 0x114 (PanMode)
    int field_0x118;                               // 0x118 (PanCurve)
};

static inline VoiceLayout& VoiceRef(Voice* self) {
    return *reinterpret_cast<VoiceLayout*>(self);
}

// Recover a VoiceNodeLayout* from a VoiceList const-iterator. The iterator
// stores the LinkListNode*; retracing the model's `node` offset and then
// stepping back to the retail node-0x90 anchor yields the node-relative view.
static inline const VoiceNodeLayout& VoiceNodeRef(Voice* pItVoice) {
    const u8* pNode = reinterpret_cast<const u8*>(pItVoice) + offsetof(Voice, node);
    return *reinterpret_cast<const VoiceNodeLayout*>(pNode - 0x90);
}

// Local mirror of the shared CalcMixVolume that adds the explicit `volume <= 0`
// early-return retail emits before every output conversion. The shared header
// variant (snd_AxVoice.h) omits that clamp and therefore cannot reproduce the
// retail up-front zero check; kept local to this TU for that reason.
static inline u16 mixVolume(f32 volume) {
    if (volume <= lbl_eu_8066A09C) {
        return 0;
    }
    return ut::Min<u32>(USHRT_MAX, lbl_eu_8066A0C8 * volume);
}

Voice::Voice() {
    VoiceLayout& v = VoiceRef(this);

    // Default every voice-out param to { volume=1, pitch=1, pan=0, surround=0,
    // fxSend=0, lpf=0 }. Retail peels element 0 inline, then loops the rest.
    v.mVoiceOutParam[0][0] = lbl_eu_8066A098;
    v.mVoiceOutParam[0][1] = lbl_eu_8066A098;
    v.mVoiceOutParam[0][2] = lbl_eu_8066A09C;
    v.mVoiceOutParam[0][3] = lbl_eu_8066A09C;
    v.mVoiceOutParam[0][4] = lbl_eu_8066A09C;
    v.mVoiceOutParam[0][5] = lbl_eu_8066A09C;

    for (int i = 1; i < VOICES_MAX; i++) {
        v.mVoiceOutParam[i][0] = lbl_eu_8066A098;
        v.mVoiceOutParam[i][1] = lbl_eu_8066A098;
        v.mVoiceOutParam[i][2] = lbl_eu_8066A09C;
        v.mVoiceOutParam[i][3] = lbl_eu_8066A09C;
        v.mVoiceOutParam[i][4] = lbl_eu_8066A09C;
        v.mVoiceOutParam[i][5] = lbl_eu_8066A09C;
    }

    for (int i = 0; i < CHANNEL_MAX; i++) {
        for (int j = 0; j < VOICES_MAX; j++) {
            v.mAxVoice[i][j] = NULL;
        }
    }

    v.mCallback = NULL;
    v.mIsActive = false;
    v.mIsStarting = false;
    v.mIsStarted = false;
    v.mIsPause = false;
    v.mSyncFlag = 0;
    v.field_0x114 = 0;
    v.field_0x118 = 0;
}

Voice::~Voice() {
    for (int i = 0; i < CHANNEL_MAX; i++) {
        for (int j = 0; j < VOICES_MAX; j++) {
            AxVoice* pVoice = mAxVoice[i][j];

            if (pVoice != NULL) {
                AxVoiceManager::GetInstance().FreeAxVoice(pVoice);
            }
        }
    }
}

void Voice::InitParam(int channels, int voices, VoiceCallback pCallback,
                      void* pCallbackArg) {
    VoiceLayout& v = VoiceRef(this);

    v.mChannelCount = channels;
    v.mVoiceOutCount = voices;
    v.mCallback = pCallback;
    v.mCallbackArg = pCallbackArg;

    v.mSyncFlag = 0;
    v.mIsPause = false;
    v.mIsPausing = false;
    v.mIsStarted = false;
    v.field_0xA1 = 0;

    v.mVolume = lbl_eu_8066A098;
    v.mVeInitVolume = lbl_eu_8066A09C;
    v.mVeTargetVolume = lbl_eu_8066A098;
    v.mLpfFreq = lbl_eu_8066A098;
    v.mBiquadType = 0;
    v.mBiquadFreq = lbl_eu_8066A09C;
    v.mPan = lbl_eu_8066A09C;
    v.mSurroundPan = lbl_eu_8066A09C;
    v.mOutputLineFlag = OUTPUT_LINE_MAIN;
    v.mMainOutVolume = lbl_eu_8066A098;
    v.mMainSend = lbl_eu_8066A098;

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        v.mFxSend[i] = lbl_eu_8066A09C;
    }

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        v.mRemoteOutVolume[i] = lbl_eu_8066A098;
        v.mRemoteSend[i] = lbl_eu_8066A098;
        v.mRemoteFxSend[i] = lbl_eu_8066A09C;
    }

    v.mPitch = lbl_eu_8066A098;
    v.mRemoteFilter = 0;
    v.field_0x114 = 0;
    v.field_0x118 = 0;
}

void Voice::StopFinished() {
    VoiceLayout& v = VoiceRef(this);

    if (v.mIsActive && v.mIsStarted && IsPlayFinished()) {
        if (v.mCallback != NULL) {
            ((VoiceCallback)v.mCallback)(this, CALLBACK_STATUS_FINISH_WAVE,
                                        v.mCallbackArg);
        }

        v.mIsStarted = false;
        v.mIsStarting = false;
    }
}

void Voice::Calc() {
    VoiceLayout& v = VoiceRef(this);

    if (!v.mIsStarting) {
        return;
    }

    if (v.mSyncFlag & SYNC_AX_SRC) {
        // CalcAxSrc(false): per-voice pitch x voiceOutParam.pitch when the
        // pitch-modulation gate field_0xA1 is clear, then SetSrc every voice.
        for (int i = 0; i < v.mVoiceOutCount; i++) {
            f32 pitch = v.mPitch;
            if (v.field_0xA1 == 0) {
                pitch *= v.mVoiceOutParam[i][1];
            }
            for (int j = 0; j < v.mChannelCount; j++) {
                if (v.mAxVoice[j][i] != NULL) {
                    v.mAxVoice[j][i]->SetSrc(pitch, false);
                }
            }
        }
        v.mSyncFlag &= ~SYNC_AX_SRC;
    }

    if (v.mSyncFlag & SYNC_AX_VE) {
        CalcAxVe();
        v.mSyncFlag &= ~SYNC_AX_VE;
    }

    if (v.mSyncFlag & SYNC_AX_MIX) {
        if (!CalcAxMix()) {
            v.mSyncFlag &= ~SYNC_AX_MIX;
        }
    }

    if (v.mSyncFlag & SYNC_AX_LPF) {
        // CalcAxLpf(): per-voice LPF cutoff scaled by voiceOutParam.lpf.
        for (int i = 0; i < v.mVoiceOutCount; i++) {
            u16 freq = static_cast<u16>(
                Util::CalcLpfFreq(v.mLpfFreq + v.mVoiceOutParam[i][5]));
            for (int j = 0; j < v.mChannelCount; j++) {
                if (v.mAxVoice[j][i] != NULL) {
                    v.mAxVoice[j][i]->SetLpf(freq);
                }
            }
        }
        v.mSyncFlag &= ~SYNC_AX_LPF;
    }

    if (v.mSyncFlag & SYNC_AX_BIQUAD) {
        for (int i = 0; i < v.mVoiceOutCount; i++) {
            for (int j = 0; j < v.mChannelCount; j++) {
                if (v.mAxVoice[j][i] != NULL) {
                    SetBiquad__Q44nw4r3snd6detail7AxVoiceFUcf(
                        v.mAxVoice[j][i], v.mBiquadType, v.mBiquadFreq);
                }
            }
        }
        v.mSyncFlag &= ~SYNC_AX_BIQUAD;
    }

    if (v.mSyncFlag & SYNC_AX_REMOTE) {
        // CalcAxRemoteFilter(): broadcast the remote filter flag.
        for (int i = 0; i < v.mVoiceOutCount; i++) {
            for (int j = 0; j < v.mChannelCount; j++) {
                if (v.mAxVoice[j][i] != NULL) {
                    v.mAxVoice[j][i]->SetRemoteFilter(v.mRemoteFilter);
                }
            }
        }
        v.mSyncFlag &= ~SYNC_AX_REMOTE;
    }
}

void Voice::Update() {
    VoiceLayout& v = VoiceRef(this);

    u32 ints = OSDisableInterrupts();

    if (!v.mIsActive) {
        OSRestoreInterrupts(ints);
        return;
    }

    // state: 0 = no action, 1 = run all, 2 = stop all (retail r31 dispatch).
    int state = 0;

    if ((v.mSyncFlag & SYNC_AX_SRC_INITIAL) != 0 && v.mIsStarting &&
        !v.mIsStarted) {
        // CalcAxSrc(true): pitch x voiceOutParam.pitch gate, SetSrc every voice.
        for (int i = 0; i < v.mVoiceOutCount; i++) {
            f32 pitch = v.mPitch;
            if (v.field_0xA1 == 0) {
                pitch *= v.mVoiceOutParam[i][1];
            }
            for (int j = 0; j < v.mChannelCount; j++) {
                if (v.mAxVoice[j][i] != NULL) {
                    v.mAxVoice[j][i]->SetSrc(pitch, true);
                }
            }
        }

        state = 1;
        v.mIsStarted = true;
        v.mSyncFlag &= ~SYNC_AX_SRC_INITIAL;
        v.mSyncFlag &= ~SYNC_AX_SRC;
    }

    if (v.mIsStarted) {
        if ((v.mSyncFlag & SYNC_AX_VOICE) != 0 && v.mIsStarting) {
            if (v.mIsPause) {
                v.mIsPausing = true;
                state = 2;
            } else {
                v.mIsPausing = false;
                state = 1;
            }
            v.mSyncFlag &= ~SYNC_AX_VOICE;
        }

        // SyncAxVoice(): sync every live voice.
        for (int i = 0; i < v.mChannelCount; i++) {
            for (int j = 0; j < v.mVoiceOutCount; j++) {
                if (v.mAxVoice[i][j] != NULL) {
                    v.mAxVoice[i][j]->Sync();
                }
            }
        }
    }

    switch (state) {
    case 1: // RUN / start the voices.
        for (int i = 0; i < v.mChannelCount; i++) {
            for (int j = 0; j < v.mVoiceOutCount; j++) {
                if (v.mAxVoice[i][j] != NULL) {
                    v.mAxVoice[i][j]->Run();
                }
            }
        }
        break;

    case 2: // STOP (pause request or disk error).
        for (int i = 0; i < v.mChannelCount; i++) {
            for (int j = 0; j < v.mVoiceOutCount; j++) {
                if (v.mAxVoice[i][j] != NULL) {
                    v.mAxVoice[i][j]->Stop();
                }
            }
        }
        break;
    }

    OSRestoreInterrupts(ints);
}

bool Voice::Acquire(int channels, int voices, int priority,
                    VoiceCallback pCallback, void* pCallbackArg) {
    channels = ut::Clamp(channels, CHANNEL_MIN, CHANNEL_MAX);
    voices = ut::Clamp(voices, VOICES_MIN, VOICES_MAX);

    VoiceLayout& v = VoiceRef(this);

    ut::AutoInterruptLock lock;

    u32 axPrio;
    if (priority == PRIORITY_MAX) {
        axPrio = AX_PRIORITY_MAX;
    } else {
        axPrio = (AX_PRIORITY_MAX / 2) + 1;
    }

    int required = channels * voices;
    AxVoice* voiceTable[CHANNEL_MAX * VOICES_MAX];

    for (int i = 0; required > i; i++) {
        AxVoice* pAxVoice = AxVoiceManager::GetInstance().AcquireAxVoice(
            axPrio, AxVoiceCallbackFunc, this);

        if (pAxVoice == NULL) {
            int rest = required - i;

            const VoiceList& rVoiceList =
                VoiceManager::GetInstance().GetVoiceList();

            for (VoiceList::ConstIterator it = rVoiceList.GetBeginIter();
                 it != rVoiceList.GetEndIter(); ++it) {

                // Read priority / voice-count via the node-anchored retail
                // mirror (the class header node offset differs from retail).
                const VoiceNodeLayout& vn = VoiceNodeRef(const_cast<Voice*>(&*it));

                if (priority < vn.mPriority) {
                    break;
                }

                rest -= vn.mChannelCount * vn.mVoiceOutCount;
                if (rest <= 0) {
                    break;
                }
            }

            if (rest > 0) {
                for (int j = 0; j < i; j++) {
                    AxVoiceManager::GetInstance().FreeAxVoice(voiceTable[j]);
                }

                return false;
            }

            u32 allocPrio;
            if (axPrio == AX_PRIORITY_MAX) {
                allocPrio = AX_PRIORITY_MAX;
            } else {
                allocPrio = (AX_PRIORITY_MAX / 2) + 2;
            }

            pAxVoice = AxVoiceManager::GetInstance().AcquireAxVoice(
                allocPrio, AxVoiceCallbackFunc, this);
        }

        if (pAxVoice == NULL) {
            for (int j = 0; j < i; j++) {
                AxVoiceManager::GetInstance().FreeAxVoice(voiceTable[j]);
            }

            return false;
        }

        voiceTable[i] = pAxVoice;
    }

    int idx = 0;
    for (int i = 0; i < channels; i++) {
        for (int j = 0; j < voices; j++) {
            voiceTable[idx]->SetPriority(axPrio);
            v.mAxVoice[i][j] = voiceTable[idx];
            idx++;
        }
    }

    InitParam(channels, voices, pCallback, pCallbackArg);
    v.mIsActive = true;
    return true;
}

void Voice::Free() {
    ut::AutoInterruptLock lock;

    VoiceLayout& v = VoiceRef(this);

    if (!v.mIsActive) {
        return;
    }

    for (int i = 0; i < v.mChannelCount; i++) {
        for (int j = 0; j < v.mVoiceOutCount; j++) {
            AxVoice* pAxVoice = v.mAxVoice[i][j];

            if (pAxVoice != NULL) {
                AxVoiceManager::GetInstance().FreeAxVoice(pAxVoice);
                v.mAxVoice[i][j] = NULL;
            }
        }
    }

    v.mChannelCount = 0;
    VoiceManager::GetInstance().FreeVoice(this);
    v.mIsActive = false;
}

void Voice::Setup(const WaveData& rData, u32 offset) {
    AxVoice::Format format = WaveFormatToAxFormat(rData.sampleFormat);
    int sampleRate = rData.sampleRate;

    for (int i = 0; i < mChannelCount; i++) {
        if (mAxVoice[i][0] == NULL) {
            continue;
        }

        void* pAddr = rData.channelParam[i].dataAddr;
        const ChannelParam& rParam = rData.channelParam[i];
        const AdpcmInfo& rInfo = rData.channelParam[i].adpcmInfo;

        AdpcmParam param;
        if (format == AxVoice::FORMAT_ADPCM) {
            param = rInfo.param;
            AxVoice::CalcOffsetAdpcmParam(&param.pred_scale, &param.yn1,
                                          &param.yn2, offset, pAddr, param);
        }

        for (int j = 0; j < mVoiceOutCount; j++) {
            AxVoice* pAxVoice = mAxVoice[i][j];
            if (pAxVoice == NULL) {
                continue;
            }

            pAxVoice->Setup(rData.channelParam[i].dataAddr, format, sampleRate);
            pAxVoice->SetAddr(rData.loopFlag, pAddr, offset, rData.loopStart,
                              rData.loopEnd);

            if (format == AxVoice::FORMAT_ADPCM) {
                pAxVoice->SetAdpcm(&param);
                pAxVoice->SetAdpcmLoop(&rInfo.loopParam);
            }

            pAxVoice->SetSrcType(AxVoice::SRC_4TAP_AUTO, mPitch);
            pAxVoice->SetVoiceType(AxVoice::VOICE_TYPE_NORMAL);
        }
    }

    for (int i = 0; i < mVoiceOutCount; i++) {
        mVoiceOutParam[i][0] = 1.0f;
        mVoiceOutParam[i][1] = 1.0f;
        mVoiceOutParam[i][2] = 0.0f;
        mVoiceOutParam[i][3] = 0.0f;
        mVoiceOutParam[i][4] = 0.0f;
        mVoiceOutParam[i][5] = 0.0f;
    }

    mIsPause = false;
    mIsPausing = false;

    mIsStarting = false;
    mIsStarted = false;

    mSyncFlag |= (SYNC_AX_LPF | SYNC_AX_MIX | SYNC_AX_VE);
}

void Voice::Start() {
    mIsStarting = true;
    mIsPause = false;
    mSyncFlag |= SYNC_AX_SRC_INITIAL;
}

void Voice::Stop() {
    VoiceLayout& v = VoiceRef(this);

    if (v.mIsStarted) {
        StopAllAxVoice();
        v.mIsStarted = false;
    }

    v.mIsPausing = false;
    v.mIsPause = false;
    v.mIsStarting = false;
}

void Voice::Pause(bool flag) {
    if (mIsPause == flag) {
        return;
    }

    mIsPause = flag;
    mSyncFlag |= SYNC_AX_VOICE;
}

AxVoice::Format Voice::GetFormat() const {
    if (IsActive()) {
        return mAxVoice[0][0]->GetFormat();
    }

    return static_cast<AxVoice::Format>(1);
}

void Voice::SetVolume(f32 volume) {
    volume = ut::Clamp(volume, 0.0f, 1.0f);

    VoiceLayout& v = VoiceRef(this);
    if (volume != v.mVolume) {
        v.mVolume = volume;
        v.mSyncFlag |= SYNC_AX_VE;
    }
}

void Voice::SetVeVolume(f32 target, f32 init) {
    target = ut::Clamp(target, 0.0f, 1.0f);
    init = ut::Clamp(init, 0.0f, 1.0f);

    VoiceLayout& v = VoiceRef(this);

    // @bug Unreachable code
    if (init < 0.0f) {
        if (target == v.mVeTargetVolume) {
            return;
        }

        v.mVeTargetVolume = target;
        v.mSyncFlag |= SYNC_AX_VE;
        return;
    }

    if (init == v.mVeInitVolume && target == v.mVeTargetVolume) {
        return;
    }

    v.mVeInitVolume = init;
    v.mVeTargetVolume = target;
    v.mSyncFlag |= SYNC_AX_VE;
}

void Voice::SetPitch(f32 pitch) {
    if (pitch == mPitch) {
        return;
    }

    mPitch = pitch;
    mSyncFlag |= SYNC_AX_SRC;
}

void Voice::SetPanMode(PanMode mode) {
    if (mode == mPanMode) {
        return;
    }

    mPanMode = mode;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetPanCurve(PanCurve curve) {
    if (curve == mPanCurve) {
        return;
    }

    mPanCurve = curve;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetPan(f32 pan) {
    if (pan == mPan) {
        return;
    }

    mPan = pan;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetSurroundPan(f32 pan) {
    if (pan == mSurroundPan) {
        return;
    }

    mSurroundPan = pan;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetLpfFreq(f32 freq) {
    if (freq != VoiceRef(this).mLpfFreq) {
        VoiceRef(this).mLpfFreq = freq;
        VoiceRef(this).mSyncFlag |= SYNC_AX_LPF;
    }
}

void Voice::SetRemoteFilter(int filter) {
    filter = ut::Clamp(filter, 0, REMOTE_FILTER_MAX);

    if (filter == mRemoteFilter) {
        return;
    }

    mRemoteFilter = filter;
    mSyncFlag |= SYNC_AX_REMOTE;
}

void Voice::SetOutputLine(int flag) {
    if (flag == mOutputLineFlag) {
        return;
    }

    mOutputLineFlag = flag;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetMainOutVolume(f32 volume) {
    volume = ut::Clamp(volume, 0.0f, 1.0f);

    if (volume == mMainOutVolume) {
        return;
    }

    mMainOutVolume = volume;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetMainSend(f32 send) {
    send += 1.0f;
    send = ut::Clamp(send, 0.0f, 1.0f);

    if (send == mMainSend) {
        return;
    }

    mMainSend = send;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetFxSend(AuxBus bus, f32 send) {
    send = ut::Clamp(send, 0.0f, 1.0f);

    if (send == mFxSend[bus]) {
        return;
    }

    mFxSend[bus] = send;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetRemoteOutVolume(int remote, f32 volume) {
    volume = ut::Clamp(volume, 0.0f, 1.0f);

    if (volume == mRemoteOutVolume[remote]) {
        return;
    }

    mRemoteOutVolume[remote] = volume;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetRemoteSend(int remote, f32 send) {
    send += 1.0f;
    send = ut::Clamp(send, 0.0f, 1.0f);

    if (send == mRemoteSend[remote]) {
        return;
    }

    mRemoteSend[remote] = send;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetRemoteFxSend(int remote, f32 send) {
    send = ut::Clamp(send, 0.0f, 1.0f);

    if (send == mRemoteFxSend[remote]) {
        return;
    }

    mRemoteFxSend[remote] = send;
    mSyncFlag |= SYNC_AX_MIX;
}

void Voice::SetPriority(int priority) {
    VoiceLayout& v = VoiceRef(this);

    v.mPriority = priority;
    VoiceManager::GetInstance().ChangeVoicePriority(this);

    if (v.mPriority != 1) {
        return;
    }

    for (int i = 0; i < v.mChannelCount; i++) {
        for (int j = 0; j < v.mVoiceOutCount; j++) {
            AxVoice* pAxVoice = v.mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetPriority(AX_PRIORITY_MAX / 2);
            }
        }
    }
}

void Voice::UpdateVoicesPriority() {
    if (VoiceRef(this).mPriority == 1) {
        return;
    }

    for (int i = 0; i < VoiceRef(this).mChannelCount; i++) {
        for (int j = 0; j < VoiceRef(this).mVoiceOutCount; j++) {
            AxVoice* pAxVoice = VoiceRef(this).mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetPriority((AX_PRIORITY_MAX / 2) + 1);
            }
        }
    }
}

void Voice::SetAdpcmLoop(int channel, const AdpcmLoopParam* pParam) {
    VoiceLayout& v = VoiceRef(this);

    for (int i = 0; i < v.mVoiceOutCount; i++) {
        AxVoice* pAxVoice = v.mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->SetAdpcmLoop(pParam);
        }
    }
}

u32 Voice::GetCurrentPlayingSample() const {
    if (IsActive()) {
        return mAxVoice[0][0]->GetCurrentPlayingSample();
    }

    return 0;
}

void Voice::SetLoopStart(int channel, const void* pBase, u32 samples) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        AxVoice* pAxVoice = mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->SetLoopStart(pBase, samples);
        }
    }
}

void Voice::SetLoopEnd(int channel, const void* pBase, u32 samples) {
    for (int i = 0; i < mVoiceOutCount; i++) {
        AxVoice* pAxVoice = mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->SetLoopEnd(pBase, samples);
        }
    }
}

void Voice::SetLoopFlag(bool loop) {
    for (int i = 0; i < VoiceRef(this).mChannelCount; i++) {
        for (int j = 0; j < VoiceRef(this).mVoiceOutCount; j++) {
            AxVoice* pAxVoice = VoiceRef(this).mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetLoopFlag(loop);
            }
        }
    }
}

void Voice::StopAtPoint(int channel, const void* pBase, u32 samples) {
    VoiceLayout& v = VoiceRef(this);

    for (int i = 0; i < v.mVoiceOutCount; i++) {
        AxVoice* pAxVoice = v.mAxVoice[channel][i];

        if (pAxVoice != NULL) {
            pAxVoice->StopAtPoint(pBase, samples);
        }
    }
}

void Voice::SetVoiceType(AxVoice::VoiceType type) {
    for (int i = 0; i < VoiceRef(this).mChannelCount; i++) {
        for (int j = 0; j < VoiceRef(this).mVoiceOutCount; j++) {
            AxVoice* pAxVoice = VoiceRef(this).mAxVoice[i][j];

            if (pAxVoice != NULL) {
                pAxVoice->SetVoiceType(type);
            }
        }
    }
}

void Voice::CalcAxVe() {
    VoiceLayout& v = VoiceRef(this);

    f32 baseVolume = lbl_eu_8066A098;
    baseVolume *= v.mVolume;
    baseVolume *= AxManager::GetInstance().GetOutputVolume();

    for (int i = 0; i < v.mVoiceOutCount; i++) {
        f32 volume = baseVolume * v.mVoiceOutParam[i][0];
        f32 target = volume * v.mVeTargetVolume;
        f32 init = volume * v.mVeInitVolume;

        for (int j = 0; j < v.mChannelCount; j++) {
            AxVoice* pAxVoice = v.mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->SetVe(target, init);
            }
        }
    }
}

bool Voice::CalcAxMix() {
    VoiceLayout& v = VoiceRef(this);

    AxVoice::MixParam param;
    AxVoice::RemoteMixParam rmtParam;

    bool nextUpdate = false;

    for (int i = 0; i < v.mChannelCount; i++) {
        for (int j = 0; j < v.mVoiceOutCount; j++) {
            AxVoice* pAxVoice = v.mAxVoice[i][j];
            if (pAxVoice == NULL) {
                continue;
            }

            CalcMixParam(i, j, &param, &rmtParam);
            nextUpdate |= pAxVoice->SetMix(param);

            if (v.mOutputLineFlag == 0 ||
                v.mOutputLineFlag == OUTPUT_LINE_MAIN) {
                pAxVoice->EnableRemote(false);
            } else {
                pAxVoice->EnableRemote(true);
                pAxVoice->SetRmtMix(rmtParam);
            }
        }
    }

    return nextUpdate;
}

void Voice::ResetDelta() {
    VoiceLayout& v = VoiceRef(this);

    for (int i = 0; i < v.mVoiceOutCount; i++) {
        for (int j = 0; j < v.mChannelCount; j++) {
            AxVoice* pAxVoice = v.mAxVoice[j][i];

            if (pAxVoice != NULL) {
                pAxVoice->ResetDelta();
            }
        }
    }
}

void Voice::AxVoiceCallbackFunc(AxVoice* pDropVoice,
                                AxVoice::AxVoiceCallbackStatus status,
                                void* pCallbackArg) {
    Voice* p = static_cast<Voice*>(pCallbackArg);
    VoiceLayout& v = VoiceRef(p);

    VoiceCallbackStatus voiceStatus;
    bool freeDropVoice = false;

    switch (status) {
    case AxVoice::CALLBACK_STATUS_CANCEL: {
        voiceStatus = CALLBACK_STATUS_CANCEL;
        break;
    }

    case AxVoice::CALLBACK_STATUS_DROP_DSP: {
        voiceStatus = CALLBACK_STATUS_DROP_DSP;
        freeDropVoice = true;
        break;
    }
    }

    for (int i = 0; i < v.mChannelCount; i++) {
        for (int j = 0; j < v.mVoiceOutCount; j++) {
            AxVoice* pAxVoice = v.mAxVoice[i][j];

            if (pAxVoice != NULL) {
                if (pAxVoice == pDropVoice) {
                    if (!freeDropVoice) {
                        AxVoiceManager::GetInstance().FreeAxVoice(pAxVoice);
                    }
                } else {
                    pAxVoice->Stop();
                    AxVoiceManager::GetInstance().FreeAxVoice(pAxVoice);
                }

                v.mAxVoice[i][j] = NULL;
            }
        }
    }

    v.mIsPause = false;
    v.mIsStarting = false;
    v.mChannelCount = 0;

    if (freeDropVoice) {
        p->Free();
    }

    if (v.mCallback != NULL) {
        ((VoiceCallback)v.mCallback)(p, voiceStatus, v.mCallbackArg);
    }
}

void Voice::TransformDpl2Pan(f32* pPan, f32* pSurroundPan, f32 pan,
                             f32 surroundPan) {
    surroundPan -= 1.0f;

    if (ut::Abs(pan) <= ut::Abs(surroundPan)) {
        if (surroundPan <= 0.0f) {
            *pPan = pan;
            *pSurroundPan = -0.12f + 0.88f * surroundPan;
        } else {
            *pPan = 0.5f * pan;
            *pSurroundPan = -0.12f + 1.12f * surroundPan;
        }
    } else if (pan >= 0.0f) {
        if (surroundPan <= 0.0f) {
            *pPan =
                (0.85f + (1.0f - 0.85f) * (-surroundPan / pan)) * ut::Abs(pan);
            *pSurroundPan = -0.12f + (2.0f * surroundPan + 0.88f * pan);
        } else {
            *pPan =
                (0.85f + (1.0f - 0.65f) * (-surroundPan / pan)) * ut::Abs(pan);
            *pSurroundPan = -0.12f + 1.12f * pan;
        }
    } else if (surroundPan <= 0.0f) {
        *pPan = ((1.0f - 0.85f) * (-surroundPan / pan) - 0.85f) * ut::Abs(pan);
        *pSurroundPan = -0.12f + (2.0f * surroundPan - 1.12f * pan);
    } else {
        *pPan = ((1.0f - 0.65f) * (-surroundPan / pan) - 0.85f) * ut::Abs(pan);
        *pSurroundPan = -0.12f + 1.12f * -pan;
    }

    *pSurroundPan += 1.0f;
}

void Voice::CalcMixParam(int channel, int voice, AxVoice::MixParam* pMix,
                         AxVoice::RemoteMixParam* pRmtMix) {
    VoiceLayout& v = VoiceRef(this);

    f32 mainVolume = lbl_eu_8066A09C;
    f32 mainSend = lbl_eu_8066A09C;

    f32 fxSendA = lbl_eu_8066A09C;
    f32 fxSendB = lbl_eu_8066A09C;
    f32 fxSendC = lbl_eu_8066A09C;

    if (v.mOutputLineFlag & OUTPUT_LINE_MAIN) {
        mainVolume = v.mMainOutVolume;
        mainSend = v.mMainSend;

        fxSendA = ut::Clamp(v.mFxSend[AUX_A] + v.mVoiceOutParam[voice][4],
                            lbl_eu_8066A09C, lbl_eu_8066A098);
        fxSendB = v.mFxSend[AUX_B];
        fxSendC = v.mFxSend[AUX_C];
    }

    f32 main = mainVolume * mainSend;
    f32 fx_a = mainVolume * fxSendA;
    f32 fx_b = mainVolume * fxSendB;
    f32 fx_c = mainVolume * fxSendC;

    f32 remote[WPAD_MAX_CONTROLLERS];
    f32 remoteFx[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        f32 remoteVolume = lbl_eu_8066A09C;
        f32 remoteSend = lbl_eu_8066A09C;
        f32 remoteFxSend = lbl_eu_8066A09C;

        if (v.mOutputLineFlag & (OUTPUT_LINE_REMOTE_N << i)) {
            remoteVolume = v.mRemoteOutVolume[i];
            remoteSend = v.mRemoteSend[i];
            remoteFxSend = v.mRemoteFxSend[i];
        }

        remote[i] = remoteVolume * remoteSend;
        remoteFx[i] = remoteVolume * remoteFxSend;
    }

    f32 left, right, surround, lrMixed;
    f32 front, rear;

    Util::PanInfo panInfo;

    switch (v.field_0x118) {
    case PAN_CURVE_SQRT: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
        break;
    }
    case PAN_CURVE_SQRT_0DB: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
        panInfo.centerZero = true;
        break;
    }
    case PAN_CURVE_SQRT_0DB_CLAMP: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
        panInfo.centerZero = true;
        panInfo.zeroClamp = true;
        break;
    }

    case PAN_CURVE_SINCOS: {
        panInfo.curve = Util::PAN_CURVE_SINCOS;
        break;
    }
    case PAN_CURVE_SINCOS_0DB: {
        panInfo.curve = Util::PAN_CURVE_SINCOS;
        panInfo.centerZero = true;
        break;
    }
    case PAN_CURVE_SINCOS_0DB_CLAMP: {
        panInfo.curve = Util::PAN_CURVE_SINCOS;
        panInfo.centerZero = true;
        panInfo.zeroClamp = true;
        break;
    }

    case PAN_CURVE_LINEAR: {
        panInfo.curve = Util::PAN_CURVE_LINEAR;
        break;
    }
    case PAN_CURVE_LINEAR_0DB: {
        panInfo.curve = Util::PAN_CURVE_LINEAR;
        panInfo.centerZero = true;
        break;
    }
    case PAN_CURVE_LINEAR_0DB_CLAMP: {
        panInfo.curve = Util::PAN_CURVE_LINEAR;
        panInfo.centerZero = true;
        panInfo.zeroClamp = true;
        break;
    }

    default: {
        panInfo.curve = Util::PAN_CURVE_SQRT;
    }
    }

    if (v.mChannelCount > 1 && v.field_0x114 == PAN_MODE_BALANCE) {
        f32 pan = v.mPan + v.mVoiceOutParam[voice][2];
        f32 surroundPan = v.mSurroundPan + v.mVoiceOutParam[voice][3];

        if (channel == 0) {
            left = Util::CalcPanRatio(pan, panInfo);
            right = lbl_eu_8066A09C;
        } else if (channel == 1) {
            left = lbl_eu_8066A09C;
            right = Util::CalcPanRatio(-pan, panInfo);
        }

        front = Util::CalcSurroundPanRatio(surroundPan, panInfo);
        rear = Util::CalcSurroundPanRatio(lbl_eu_8066A0B8 - surroundPan, panInfo);
    } else {
        f32 voicePan = lbl_eu_8066A09C;
        f32 pan, surroundPan;

        if (v.mChannelCount == 2) {
            if (channel == 0) {
                voicePan = lbl_eu_8066A0C0;
            }
            if (channel == 1) {
                voicePan = lbl_eu_8066A098;
            }
        }

        switch (AxManager::GetInstance().GetOutputMode()) {
        case OUTPUT_MODE_DPL2: {
            TransformDpl2Pan(
                &pan, &surroundPan,
                v.mPan + voicePan + v.mVoiceOutParam[voice][2],
                v.mSurroundPan + v.mVoiceOutParam[voice][3]);
            break;
        }

        case OUTPUT_MODE_STEREO:
        case OUTPUT_MODE_SURROUND:
        case OUTPUT_MODE_MONO:
        default: {
            pan = v.mPan + voicePan + v.mVoiceOutParam[voice][2];
            surroundPan = v.mSurroundPan + v.mVoiceOutParam[voice][3];
            break;
        }
        }

        left = Util::CalcPanRatio(pan, panInfo);
        right = Util::CalcPanRatio(-pan, panInfo);
        front = Util::CalcSurroundPanRatio(surroundPan, panInfo);
        rear = Util::CalcSurroundPanRatio(lbl_eu_8066A0B8 - surroundPan, panInfo);
    }

    surround = Util::CalcVolumeRatio(lbl_eu_8066A0C4);
    lrMixed = lbl_eu_8066A0A8 * (left + right);

    f32 m_l, m_r, m_s;
    f32 a_l, a_r, a_s;
    f32 b_l, b_r, b_s;
    f32 c_l, c_r, c_s;

    f32& m_sl = m_s;
    f32& m_sr = c_l;

    f32& a_sl = a_s;
    f32& a_sr = c_r;

    f32& b_sl = b_s;
    f32& b_sr = c_s;

    switch (AxManager::GetInstance().GetOutputMode()) {
    case OUTPUT_MODE_STEREO: {
        m_l = main * left;
        m_r = main * right;
        m_s = lbl_eu_8066A09C;

        a_l = fx_a * left;
        a_r = fx_a * right;
        a_s = lbl_eu_8066A09C;

        b_l = fx_b * left;
        b_r = fx_b * right;
        b_s = lbl_eu_8066A09C;

        c_l = fx_c * left;
        c_r = fx_c * right;
        c_s = lbl_eu_8066A09C;
        break;
    }

    case OUTPUT_MODE_MONO: {
        m_l = main * lrMixed;
        m_r = main * lrMixed;
        m_s = lbl_eu_8066A09C;

        a_l = fx_a * lrMixed;
        a_r = fx_a * lrMixed;
        a_s = lbl_eu_8066A09C;

        b_l = fx_b * lrMixed;
        b_r = fx_b * lrMixed;
        b_s = lbl_eu_8066A09C;

        c_l = fx_c * lrMixed;
        c_r = fx_c * lrMixed;
        c_s = lbl_eu_8066A09C;
        break;
    }

    case OUTPUT_MODE_SURROUND: {
        f32 fl = left * front;
        f32 fr = right * front;
        f32 rs = surround * rear;

        m_l = main * fl;
        m_r = main * fr;
        m_s = main * rs;

        a_l = fx_a * fl;
        a_r = fx_a * fr;
        a_s = fx_a * rs;

        b_l = fx_b * fl;
        b_r = fx_b * fr;
        b_s = fx_b * rs;

        c_l = fx_c * fl;
        c_r = fx_c * fr;
        c_s = fx_c * rs;
        break;
    }

    case OUTPUT_MODE_DPL2: {
        f32 fl = left * front;
        f32 fr = right * front;
        f32 rl = left * rear;
        f32 rr = right * rear;

        m_l = main * fl;
        m_r = main * fr;
        m_sl = main * rl;
        m_sr = main * rr;

        a_l = fx_a * fl;
        a_r = fx_a * fr;
        a_sl = fx_a * rl;
        a_sr = fx_a * rr;

        b_l = fx_b * fl;
        b_r = fx_b * fr;
        b_sl = fx_b * rl;
        b_sr = fx_b * rr;
        break;
    }

    default: {
        break;
    }
    }

    f32 rmt[WPAD_MAX_CONTROLLERS];
    f32 rmtFx[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        rmt[i] = lrMixed * remote[i];
        rmtFx[i] = lrMixed * remoteFx[i];
    }

    pMix->vL = mixVolume(m_l);
    pMix->vR = mixVolume(m_r);
    pMix->vS = mixVolume(m_s);

    pMix->vAuxAL = mixVolume(a_l);
    pMix->vAuxAR = mixVolume(a_r);
    pMix->vAuxAS = mixVolume(a_s);

    pMix->vAuxBL = mixVolume(b_l);
    pMix->vAuxBR = mixVolume(b_r);
    pMix->vAuxBS = mixVolume(b_s);

    pMix->vAuxCL = mixVolume(c_l);
    pMix->vAuxCR = mixVolume(c_r);
    pMix->vAuxCS = mixVolume(c_s);

    pRmtMix->vMain0 = mixVolume(rmt[0]);
    pRmtMix->vAux0 = 0;

    pRmtMix->vMain1 = mixVolume(rmt[1]);
    pRmtMix->vAux1 = 0;

    pRmtMix->vMain2 = mixVolume(rmt[2]);
    pRmtMix->vAux2 = 0;

    pRmtMix->vMain3 = mixVolume(rmt[3]);
    pRmtMix->vAux3 = 0;
}

void Voice::StopAllAxVoice() {
    VoiceLayout& v = VoiceRef(this);

    for (int i = 0; i < v.mChannelCount; i++) {
        for (int j = 0; j < v.mVoiceOutCount; j++) {
            if (v.mAxVoice[i][j] != NULL) {
                v.mAxVoice[i][j]->Stop();
            }
        }
    }
}

void Voice::InvalidateWaveData(const void* pStart, const void* pEnd) {
    VoiceLayout& v = VoiceRef(this);

    bool dispose = false;

    for (int i = 0; i < v.mChannelCount; i++) {
        AxVoice* pAxVoice = v.mAxVoice[i][0];

        if (pAxVoice != NULL && pAxVoice->IsDataAddressCoverd(pStart, pEnd)) {
            dispose = true;
            break;
        }
    }

    if (dispose) {
        Stop();

        if (v.mCallback != NULL) {
            ((VoiceCallback)v.mCallback)(this, CALLBACK_STATUS_CANCEL,
                                        v.mCallbackArg);
        }
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// SetBiquadFilter(int, float): clamp the frequency into [0, 1], then if either
// the type or the frequency actually changed, mark the biquad sync flag so a
// later Calc() re-arms the AX biquad filter.
void SetBiquadFilter__Q44nw4r3snd6detail5VoiceFif(nw4r::snd::detail::Voice* self,
                                                  int type, f32 frequency) {
    frequency = nw4r::ut::Clamp(frequency, lbl_eu_8066A09C, lbl_eu_8066A098);

    nw4r::snd::detail::VoiceLayout& v = nw4r::snd::detail::VoiceRef(self);
    bool changed = false;

    if (type != v.mBiquadType) {
        v.mBiquadType = type;
        changed = true;
    }
    if (frequency != v.mBiquadFreq) {
        v.mBiquadFreq = frequency;
        changed = true;
    }
    if (changed) {
        v.mSyncFlag |= nw4r::snd::detail::Voice::SYNC_AX_BIQUAD;
    }
}
extern "C" void SetVoiceOutParam__Q44nw4r3snd6detail5VoiceFiRCQ34nw4r3snd13VoiceOutParam(unsigned char* self, int index, const float* param) {
    float* dst = reinterpret_cast<float*>(self + index * 0x18 + 0x2c);
    dst[0] = param[0];
    dst[1] = param[1];
    dst[2] = param[2];
    dst[3] = param[3];
    dst[4] = param[4];
    dst[5] = param[5];
    *reinterpret_cast<unsigned short*>(self + 0xa2) |= 0x003c;
}
extern "C" void InvalidateData__Q44nw4r3snd6detail5VoiceFPCvPCv() {}
