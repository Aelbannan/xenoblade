#include <nw4r/snd.h>

#include <climits>
#include <cstring>

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_BASE(BasicSound);

namespace {

} // namespace

// name-guess: retail pool starts with 1.0f; a default-volume accessor is
// plausible and is link-dead in this unit.
f32 BasicSound_GetDefaultVolume() {
    return 1.0f;
}

BasicSound::BasicSound(int priority, int arg)
    : mHeap(NULL),
      mGeneralHandle(NULL),
      mTempGeneralHandle(NULL),
      mSoundPlayer(NULL),
      mSoundActor(NULL),
      mExtSoundPlayer(NULL) {
    f32 zero = 0.0f;
    f32 one = 1.0f;
    mAmbientParam.volume = one;
    mAmbientParam.pitch = one;
    mAmbientParam.pan = zero;
    mAmbientParam.surroundPan = zero;
    mAmbientParam.fxSend = zero;
    mAmbientParam.lpf = zero;
    mAmbientParam.unk18 = zero;
    mAmbientParam.priority = 0;

    mUnk0x54 = one;
    mUnk0x58 = one;
    mUnk0x5C = zero;

    mFadeVolume.InitValue(zero);
    mPauseFadeVolume.InitValue(zero);

    mId = INVALID_ID;
    mExtMoveVolume.InitValue(zero);

    // Link-list nodes (retail zeros after ExtMoveVolume).
    nodePrio.Init();
    nodePlayerPlay.Init();
    nodePlayerPrio.Init();
    nodeExtPlay.Init();

    mAmbientParamUpdateCallback = NULL;
    mAmbientArgUpdateCallback = NULL;
    mAmbientArgAllocaterCallback = NULL;
    mAmbientArg = NULL;
    mAmbientArgSize = 0;

    mVoiceOutCount = 1;
    mPriority = priority;
    mUnk0x50 = arg;
}

int BasicSound::GetVoiceOutCount() const {
    return mVoiceOutCount;
}

u32 BasicSound::GetAmbientPriority(const AmbientInfo& info, u32 param) {
    if (info.paramUpdateCallback == NULL) return 0;
    return info.paramUpdateCallback->GetPriority(info.arg, param);
}

void BasicSound::UpdateMoveValue() {
    mFadeVolume.Update();
    mExtMoveVolume.Update();
}

void BasicSound::UpdateParam() {
    // Volume: 1.0 * init * player * move/fade chains * ambient * unk54.
    f32 volume = 1.0f;
    volume *= mInitVolume;
    volume *= mSoundPlayer->GetVolume();
    volume *= mExtMoveVolume.GetValue();
    volume *= mFadeVolume.GetValue();
    volume *= mPauseFadeVolume.GetValue();
    volume *= mAmbientParam.volume;
    volume *= mUnk0x54;

    f32 pan = 0.0f;
    pan += mExtPan;
    pan += mAmbientParam.pan;
    pan += mUnk0x5C;

    f32 surroundPan = 0.0f;
    surroundPan += mExtSurroundPan;
    surroundPan += mAmbientParam.surroundPan;

    f32 pitch = 1.0f;
    pitch *= mExtPitch;
    pitch *= mAmbientParam.pitch;
    pitch *= mUnk0x58;

    f32 lpfFreq = mUnk0xBC;
    lpfFreq += mAmbientParam.lpf;
    lpfFreq += mSoundPlayer->mUnk0x30;

    // Biquad filter id/value: the sound's own, else the player's, else the
    // ambient info's (retail reads the player fields directly).
    u32 filter = mOutputLineFlagEnable;
    f32 filterValue = mUnk0xC0;
    if (filter == 0) {
        filter = mSoundPlayer->mOutputLineFlagEnable;
        filterValue = mSoundPlayer->mUnk0x40;
        if (filter == 0) {
            filter = mAmbientParam.priority;
            filterValue = mAmbientParam.unk18;
        }
    }

    f32 mainOutVol = 1.0f;
    mainOutVol *= mMainOutVolume;
    mainOutVol *= mSoundPlayer->detail_GetMainOutVolume();

    // Retail loads the output-line flag before the loops and reuses it after.
    int outputLine = mOutputLineFlag;

    f32 remoteOutVol[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVol[i] = 1.0f;
        remoteOutVol[i] *= mSoundPlayer->GetRemoteOutVolume(i);
        remoteOutVol[i] *= mRemoteOutVolume[i];
    }

    // Main send + aux sends: sound's own (mUnk0xCC) + player sends + ambient.
    f32 zero = 0.0f;
    f32 mainSend = zero;
    f32 fxSend[AUX_BUS_NUM] = { zero, zero,
                                zero };
    mainSend += mUnk0xCC[0];
    fxSend[0] += mUnk0xCC[1];
    fxSend[1] += mUnk0xCC[2];
    fxSend[2] += mUnk0xCC[3];
    mainSend += mSoundPlayer->mUnk0x54;
    fxSend[0] += mSoundPlayer->mFxSend[0];
    fxSend[1] += mSoundPlayer->mFxSend[1];
    fxSend[2] += mSoundPlayer->mFxSend[2];
    fxSend[0] += mAmbientParam.fxSend;

    // Retail fetches the player only once all values are computed.
    BasicPlayer& rPlayer = GetBasicPlayer();

    rPlayer.SetVolume(volume);
    rPlayer.SetPan(pan);
    rPlayer.SetSurroundPan(surroundPan);
    rPlayer.SetPitch(pitch);
    rPlayer.SetLpfFreq(lpfFreq);
    rPlayer.SetBiquadFilter(static_cast<int>(filter), filterValue);
    rPlayer.SetOutputLine(outputLine);
    rPlayer.SetMainOutVolume(mainOutVol);
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        rPlayer.SetRemoteOutVolume(i, remoteOutVol[i]);
    }
    rPlayer.SetMainSend(mainSend);
    for (int i = 0; i < AUX_BUS_NUM; i++) {
        rPlayer.SetFxSend(static_cast<AuxBus>(i), fxSend[i]);
    }
}

void BasicSound::InitParam() {
    mPauseState = 0;
    mPauseFadeFlag = false;
    mStartFlag = false;
    mStartedFlag = false;
    mAutoStopFlag = false;
    mFadeOutFlag = false;

    mAutoStopCounter = 0;
    mUpdateCounter = 0;

    mFadeVolume.InitValue(0.0f);
    mPauseFadeVolume.InitValue(1.0f);
    mFadeVolume.SetTarget(1.0f, 1);

    mInitVolume = 1.0f;
    mExtPitch = 1.0f;
    mExtPan = 0.0f;
    mExtSurroundPan = 0.0f;
    mExtMoveVolume.InitValue(1.0f);

    mUnk0xBC = 0.0f;
    mOutputLineFlagEnable = false;
    mUnk0xC0 = 0.0f;

    mOutputLineFlag =
        (mSoundPlayer != NULL) ? mSoundPlayer->detail_GetOutputLine()
                               : OUTPUT_LINE_MAIN;

    mMainOutVolume = 1.0f;
    for (int i = 0; i < 4; i++) {
        mUnk0xCC[i] = 0.0f;
    }
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = 1.0f;
    }

    mAmbientParam.volume = 1.0f;
    mAmbientParam.pitch = 1.0f;
    mAmbientParam.pan = 0.0f;
    mAmbientParam.surroundPan = 0.0f;
    mAmbientParam.fxSend = 0.0f;
    mAmbientParam.lpf = 0.0f;
    mAmbientParam.unk18 = 0.0f;
    mAmbientParam.priority = 0;
    mUnk0x50 = 0;
}

void BasicSound::StartPrepared() {
    if (!mStartedFlag) {
        mStartFlag = true;
    }
}

void BasicSound::Stop(int frames) {
    BasicPlayer& rPlayer = GetBasicPlayer();

    if (frames == 0 || !rPlayer.IsActive() || !rPlayer.IsStarted() ||
        rPlayer.IsPause()) {
        Shutdown();
        return;
    }

    int t = frames * mFadeVolume.GetValue();
    mFadeVolume.SetTarget(0.0f, t);

    // Inlined SetPlayerPriority(0) - retail does not bl.
    mPriority = 0;
    if (mSoundPlayer != NULL) {
        mSoundPlayer->detail_SortPriorityList(this);
    }
    OnUpdatePlayerPriority();

    mAutoStopFlag = false;
    mPauseState = 0;
    mPauseFadeFlag = false;
    mFadeOutFlag = true;
}

void BasicSound::Pause(bool flag, int frames) {
    // Xenoblade Pause is driven by mPauseState, not mPauseFlag / player.Pause.
    if (flag) {
        switch (mPauseState) {
        case PAUSE_STATE_NONE:
        case PAUSE_STATE_PAUSING:
        case PAUSE_STATE_UNPAUSING: {
            int t = frames * mPauseFadeVolume.GetValue();
            if (t <= 0) {
                t = 1;
            }
            mPauseFadeVolume.SetTarget(0.0f, t);
            mPauseState = PAUSE_STATE_PAUSING;
            mPauseFadeFlag = false;
            break;
        }

        default:
            return;
        }
    } else {
        switch (mPauseState) {
        case PAUSE_STATE_NONE:
            return;

        case PAUSE_STATE_PAUSING:
        case PAUSE_STATE_PAUSED:
        case PAUSE_STATE_UNPAUSING: {
            int t = frames * (1.0f - mPauseFadeVolume.GetValue());
            if (t <= 0) {
                t = 1;
            }
            mPauseFadeVolume.SetTarget(1.0f, t);
            mPauseState = PAUSE_STATE_UNPAUSING;
            mPauseFadeFlag = true;
            break;
        }

        default:
            return;
        }
    }
}

void BasicSound::SetAutoStopCounter(int count) {
    mAutoStopCounter = count;
    mAutoStopFlag = count > 0;
}

void BasicSound::FadeIn(int frames) {
    if (mFadeOutFlag) {
        return;
    }

    int t = frames * (1.0f - mFadeVolume.GetValue());
    mFadeVolume.SetTarget(1.0f, t);
}

bool BasicSound::IsPause() const {
    return mPauseFlag;
}

void BasicSound::Update() {
    BasicPlayer& rPlayer = GetBasicPlayer();

    if (mAutoStopFlag && rPlayer.IsActive()) {
        if (mAutoStopCounter == 0) {
            if (mPauseState == PAUSE_STATE_NONE ||
                mPauseState == PAUSE_STATE_UNPAUSING) {
                Stop(0);
                return;
            }
        } else {
            mAutoStopCounter--;
        }
    }

    bool startPlayer = false;
    if (!mStartedFlag) {
        if (!mStartFlag) {
            return;
        }

        if (!IsPrepared()) {
            return;
        }

        startPlayer = true;
    }

    if (rPlayer.IsStarted() && mUpdateCounter < ULONG_MAX) {
        mUpdateCounter++;
    }

    if (!rPlayer.IsActive()) {
        Shutdown();
        return;
    }

    switch (mPauseState) {
    case PAUSE_STATE_NONE:
        UpdateMoveValue();
        break;

    case PAUSE_STATE_PAUSING:
        mPauseFadeVolume.Update();
        break;

    case PAUSE_STATE_UNPAUSING:
        mPauseFadeVolume.Update();
        UpdateMoveValue();
        break;
    }

    if (mAmbientArgUpdateCallback != NULL) {
        mAmbientArgUpdateCallback->detail_Update(mAmbientArg, this);
    }

    if (mAmbientParamUpdateCallback != NULL) {
        // US retail defaults an ambient block on the stack and hands it to the
        // callback: the sound param (which the callback rewrites), the player
        // priority offset that follows it, and one 6-float voice-out group per
        // voice. The result is applied to this sound and to its player.
        SoundParam param;
        param.volume = 1.0f;
        param.pitch = 1.0f;
        param.pan = 0.0f;
        param.surroundPan = 0.0f;
        param.fxSend = 0.0f;
        param.lpf = 0.0f;
        param.unk18 = 0.0f;
        param.priority = 0;

        u32 priority = 0;

        VoiceOutParam voiceOutParam[PlayerParamSet::VOICE_OUT_NUM];

        voiceOutParam[0].volume = 1.0f;
        voiceOutParam[0].pitch = 1.0f;
        voiceOutParam[0].pan = 0.0f;
        voiceOutParam[0].surroundPan = 0.0f;
        voiceOutParam[0].fxSend = 0.0f;
        voiceOutParam[0].lpfFreq = 0.0f;

        VoiceOutParam* pParam = &voiceOutParam[1];
        VoiceOutParam* pEnd = &voiceOutParam[PlayerParamSet::VOICE_OUT_NUM];
        while (pParam < pEnd) {
            pParam->volume = 1.0f;
            pParam->pitch = 1.0f;
            pParam->pan = 0.0f;
            pParam->surroundPan = 0.0f;
            pParam->fxSend = 0.0f;
            pParam->lpfFreq = 0.0f;
            pParam++;
        }

        mAmbientParamUpdateCallback->detail_Update(mAmbientArg, mId,
                                                   mVoiceOutCount, &param);

        mAmbientParam = param;
        mUnk0x50 = priority;

        for (int i = 0; i < mVoiceOutCount; i++) {
            rPlayer.SetVoiceOutParam(i, voiceOutParam[i]);
        }
    }

    UpdateParam();

    if (mFadeOutFlag && mFadeVolume.IsFinished()) {
        mFadeOutFlag = false;
        Shutdown();
        return;
    }

    if (startPlayer) {
        if (rPlayer.Start()) {
            mStartedFlag = true;
            mStartFlag = false;
        } else {
            Shutdown();
            return;
        }
    }

    if (mPauseState == PAUSE_STATE_PAUSING) {
        if (mPauseFadeVolume.IsFinished()) {
            rPlayer.Pause(true);
            mPauseState = PAUSE_STATE_PAUSED;
        }
    } else if (mPauseState == PAUSE_STATE_UNPAUSING) {
        if (mPauseFadeVolume.IsFinished()) {
            mPauseState = PAUSE_STATE_NONE;
        }
    }

    if (mPauseFadeFlag) {
        rPlayer.Pause(false);
        mPauseFadeFlag = false;
    }
}

void BasicSound::Shutdown() {
    BasicPlayer& rPlayer = GetBasicPlayer();

    if (rPlayer.IsActive()) {
        if (mFadeOutFlag) {
            rPlayer.SetVolume(0.0f);
        }

        rPlayer.Stop();
    }

    SetId(INVALID_ID);

    if (IsAttachedGeneralHandle()) {
        DetachGeneralHandle();
    }

    if (IsAttachedTempGeneralHandle()) {
        DetachTempGeneralHandle();
    }

    if (IsAttachedTempSpecialHandle()) {
        DetachTempSpecialHandle();
    }

    if (mHeap != NULL) {
        mSoundPlayer->detail_FreePlayerHeap(this);
    }

    if (mSoundPlayer != NULL) {
        mSoundPlayer->detail_RemoveSound(this);
    }

    if (mExtSoundPlayer != NULL) {
        mExtSoundPlayer->RemoveSound(this);
    }

    if (mAmbientArgAllocaterCallback != NULL) {
        mAmbientArgAllocaterCallback->detail_FreeAmbientArg(mAmbientArg, this);
        mAmbientArg = NULL;
    }

    mStartedFlag = false;
    mFadeOutFlag = false;
}

void BasicSound::SetPlayerPriority(int priority) {
    mPriority = priority;

    if (mSoundPlayer != NULL) {
        mSoundPlayer->detail_SortPriorityList(this);
    }

    OnUpdatePlayerPriority();
}

void BasicSound::OnUpdatePlayerPriority() {}

void BasicSound::SetInitialVolume(f32 vol) {
    mInitVolume = ut::Clamp(vol, 0.0f, 1.0f);
}

void BasicSound::SetVolume(f32 vol, int frames) {
    f32 target;
    if (vol > 1.0f) {
        target = 1.0f;
    } else if (vol < 0.0f) {
        target = 0.0f;
    } else {
        target = vol;
    }
    mExtMoveVolume.SetTarget(target, frames);
}

void BasicSound::SetPitch(f32 pitch) {
    mExtPitch = pitch;
}

void BasicSound::SetPan(f32 pan) {
    mExtPan = pan;
}

void BasicSound::SetSurroundPan(f32 pan) {
    mExtSurroundPan = pan;
}

void BasicSound::SetLpfFreq(f32 freq) {
    GetBasicPlayer().SetLpfFreq(freq);
}

void BasicSound::SetOutputLine(int flag) {
    mOutputLineFlag = flag;
    mOutputLineFlagEnable = true;
}

bool BasicSound::IsEnabledOutputLine() const {
    return mOutputLineFlagEnable;
}

int BasicSound::GetOutputLine() const {
    return mOutputLineFlag;
}

void BasicSound::SetMainOutVolume(f32 vol) {
    mMainOutVolume = ut::Clamp(vol, 0.0f, 1.0f);
}

void BasicSound::SetRemoteOutVolume(int remote, f32 vol) {
    mRemoteOutVolume[remote] = ut::Clamp(vol, 0.0f, 1.0f);
}

void BasicSound::SetFxSend(AuxBus bus, f32 send) {
    GetBasicPlayer().SetFxSend(bus, send);
}

void BasicSound::SetRemoteFilter(int filter) {
    GetBasicPlayer().SetRemoteFilter(filter);
}

void BasicSound::SetPanMode(PanMode mode) {
    GetBasicPlayer().SetPanMode(mode);
}

void BasicSound::SetPanCurve(PanCurve curve) {
    GetBasicPlayer().SetPanCurve(curve);
}

f32 BasicSound::GetInitialVolume() const {
    return mInitVolume;
}

f32 BasicSound::GetPitch() const {
    return mExtPitch;
}

f32 BasicSound::GetPan() const {
    return mExtPan;
}

f32 BasicSound::GetSurroundPan() const {
    return mExtSurroundPan;
}

f32 BasicSound::GetMainOutVolume() const {
    return mMainOutVolume;
}

f32 BasicSound::GetRemoteOutVolume(int remote) const {
    return mRemoteOutVolume[remote];
}

void BasicSound::SetAmbientParamCallback(
    AmbientParamUpdateCallback* pParamUpdate,
    AmbientArgUpdateCallback* pArgUpdate,
    AmbientArgAllocaterCallback* pArgAlloc, void* pArg) {

    mAmbientParamUpdateCallback = pParamUpdate;
    mAmbientArgUpdateCallback = pArgUpdate;
    mAmbientArgAllocaterCallback = pArgAlloc;
    mAmbientArg = pArg;
}

void BasicSound::AttachPlayerHeap(PlayerHeap* pHeap) {
    mHeap = pHeap;
}

void BasicSound::DetachPlayerHeap(PlayerHeap* pHeap) {
    (void)pHeap;
    mHeap = NULL;
}

void BasicSound::AttachSoundPlayer(SoundPlayer* pPlayer) {
    mSoundPlayer = pPlayer;
}

void BasicSound::DetachSoundPlayer(SoundPlayer* pPlayer) {
    (void)pPlayer;
    mSoundPlayer = NULL;
}

void BasicSound::AttachSoundActor(SoundActor* pActor) {
    mSoundActor = pActor;
}

void BasicSound::AttachExternalSoundPlayer(ExternalSoundPlayer* pExtPlayer) {
    mExtSoundPlayer = pExtPlayer;
}

void BasicSound::DetachExternalSoundPlayer(ExternalSoundPlayer* pExtPlayer) {
    (void)pExtPlayer;
    mExtSoundPlayer = NULL;
}

bool BasicSound::IsAttachedGeneralHandle() {
    return mGeneralHandle != NULL;
}

bool BasicSound::IsAttachedTempGeneralHandle() {
    return mTempGeneralHandle != NULL;
}

void BasicSound::DetachGeneralHandle() {
    mGeneralHandle->DetachSound();
}

void BasicSound::DetachTempGeneralHandle() {
    mTempGeneralHandle->DetachSound();
}

void BasicSound::SetId(u32 id) {
    mId = id;
    GetBasicPlayer().SetId(id);
}

} // namespace detail
} // namespace snd
} // namespace nw4r

namespace nw4r {
namespace snd {
namespace detail {

const nw4r::ut::detail::RuntimeTypeInfo* BasicSound::GetRuntimeTypeInfo() const {
    return &typeInfo;
}

} // namespace detail
} // namespace snd
} // namespace nw4r


namespace nw4r {
namespace snd {
namespace detail {

void BasicSound::SetAmbientInfo(const AmbientInfo& rInfo) {
    void* pArg =
        rInfo.argAllocaterCallback->detail_AllocAmbientArg(rInfo.argSize);
    if (pArg == NULL) {
        return;
    }

    std::memcpy(pArg, rInfo.arg, rInfo.argSize);

    // Retail keeps this store of the raw arg before the callback fields (it is
    // overwritten by pArg below).
    mAmbientArg = rInfo.arg;

    mAmbientParamUpdateCallback = rInfo.paramUpdateCallback;
    mAmbientArgUpdateCallback = rInfo.argUpdateCallback;
    mAmbientArgAllocaterCallback = rInfo.argAllocaterCallback;
    mAmbientArgSize = rInfo.argSize;
    mAmbientArg = pArg;

    if (rInfo.paramUpdateCallback != NULL) {
        int voiceOutCount = static_cast<int>(
            rInfo.paramUpdateCallback->GetVoiceOutCount(pArg, mId));
        if (voiceOutCount > 4) {
            voiceOutCount = 4;
        }
        mVoiceOutCount = static_cast<u8>(voiceOutCount);
    }
}

} // namespace detail
} // namespace snd
} // namespace nw4r
