#include <nw4r/snd.h>

#include <climits>

// Retail .sdata2 pools (US).
extern "C" {
extern const f32 lbl_eu_80669EE8; // 1.0f
extern const f32 lbl_eu_80669EEC; // 0.0f
extern const f64 lbl_eu_80669EF0; // int→double magic
}

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_RTTI_DEF_BASE(BasicSound);

BasicSound::BasicSound(int priority, int arg)
    : mHeap(NULL),
      mGeneralHandle(NULL),
      mTempGeneralHandle(NULL),
      mSoundPlayer(NULL),
      mExtSoundPlayer(NULL),
      mAmbientParamUpdateCallback(NULL) {
    mAmbientParam.volume = lbl_eu_80669EE8;
    mAmbientParam.pitch = lbl_eu_80669EE8;
    mAmbientParam.pan = lbl_eu_80669EEC;
    mAmbientParam.surroundPan = lbl_eu_80669EEC;
    mAmbientParam.fxSend = lbl_eu_80669EEC;
    mAmbientParam.lpf = lbl_eu_80669EEC;
    mAmbientParam.unk18 = lbl_eu_80669EEC;
    mAmbientParam.priority = 0;

    mUnk0x54 = lbl_eu_80669EE8;
    mUnk0x58 = lbl_eu_80669EE8;
    mUnk0x5C = lbl_eu_80669EEC;

    mFadeVolume.InitValue(lbl_eu_80669EEC);
    mPauseFadeVolume.InitValue(lbl_eu_80669EEC);

    mId = INVALID_ID;
    mExtMoveVolume.InitValue(lbl_eu_80669EEC);

    // Link-list nodes (retail zeros after ExtMoveVolume).
    nodePrio.Init();
    nodePlayerPlay.Init();
    nodePlayerPrio.Init();
    nodeExtPlay.Init();

    mAmbientArgUpdateCallback = NULL;
    mAmbientArgAllocaterCallback = NULL;
    mAmbientArg = NULL;
    mUnk0x28 = 0;
    mUnk0x2C = 0;

    mUnk0x95 = 1;
    mPriority = priority;
    mUnk0x50 = arg;
}

void BasicSound::UpdateMoveValue() {
    mFadeVolume.Update();
    mExtMoveVolume.Update();
}

void BasicSound::UpdateParam() {}

void BasicSound::InitParam() {
    mPauseState = 0;
    mPauseFadeFlag = false;
    mStartFlag = false;
    mStartedFlag = false;
    mAutoStopFlag = false;
    mFadeOutFlag = false;

    mAutoStopCounter = 0;
    mUpdateCounter = 0;

    mFadeVolume.InitValue(lbl_eu_80669EEC);
    mPauseFadeVolume.InitValue(lbl_eu_80669EE8);
    mFadeVolume.SetTarget(lbl_eu_80669EE8, 1);

    mInitVolume = lbl_eu_80669EE8;
    mExtPitch = lbl_eu_80669EE8;
    mExtPan = lbl_eu_80669EEC;
    mExtSurroundPan = lbl_eu_80669EEC;
    mExtMoveVolume.InitValue(lbl_eu_80669EE8);

    mUnk0xBC = lbl_eu_80669EEC;
    mOutputLineFlagEnable = false;
    mUnk0xC0 = lbl_eu_80669EEC;

    mOutputLineFlag =
        (mSoundPlayer != NULL) ? mSoundPlayer->detail_GetOutputLine()
                               : OUTPUT_LINE_MAIN;

    mMainOutVolume = lbl_eu_80669EE8;
    for (int i = 0; i < 4; i++) {
        mUnk0xCC[i] = lbl_eu_80669EEC;
    }
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        mRemoteOutVolume[i] = lbl_eu_80669EE8;
    }

    mAmbientParam.volume = lbl_eu_80669EE8;
    mAmbientParam.pitch = lbl_eu_80669EE8;
    mAmbientParam.pan = lbl_eu_80669EEC;
    mAmbientParam.surroundPan = lbl_eu_80669EEC;
    mAmbientParam.fxSend = lbl_eu_80669EEC;
    mAmbientParam.lpf = lbl_eu_80669EEC;
    mAmbientParam.unk18 = lbl_eu_80669EEC;
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
    mFadeVolume.SetTarget(lbl_eu_80669EEC, t);

    // Inlined SetPlayerPriority(0) — retail does not bl.
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
        int state = mPauseState;
        if (state == 2) {
            return;
        }
        // Retail: bge to >=2 block, else cmpwi 0 / bge work.
        if (state < 2) {
            if (state < 0) {
                return;
            }
        } else {
            if (state >= 4) {
                return;
            }
        }

        int t = frames * mPauseFadeVolume.GetValue();
        if (t <= 0) {
            t = 1;
        }
        mPauseFadeVolume.SetTarget(lbl_eu_80669EEC, t);
        mPauseState = 1;
        mPauseFadeFlag = false;
    } else {
        switch (mPauseState) {
        case 0:
            return;
        case 1:
        case 2:
        case 3: {
            int t = frames * (lbl_eu_80669EE8 - mPauseFadeVolume.GetValue());
            if (t <= 0) {
                t = 1;
            }
            mPauseFadeVolume.SetTarget(lbl_eu_80669EE8, t);
            mPauseState = 3;
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
            Stop(0);
            return;
        }

        mAutoStopCounter--;
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

    if (rPlayer.IsPause()) {
        return;
    }

    if (mPauseFadeFlag) {
        mPauseFadeVolume.Update();
    } else {
        mFadeVolume.Update();
        mExtMoveVolume.Update();
    }

    if (mAmbientArgUpdateCallback != NULL) {
        mAmbientArgUpdateCallback->detail_Update(mAmbientArg, this);
    }

    if (mAmbientParamUpdateCallback != NULL) {
        mAmbientParamUpdateCallback->detail_Update(&mAmbientParam, mId, this,
                                                   mAmbientArg, 0xFFFFFFFF);
    }

    f32 volume;
    f32 pan;
    f32 surroundPan;
    f32 pitch;
    f32 mainOutVol;

    volume = 1.0f;
    volume *= GetInitialVolume();
    volume *= mSoundPlayer->GetVolume();
    if (mExtSoundPlayer != NULL) {
        volume *= mExtSoundPlayer->detail_GetVolume();
    }
    volume *= GetMoveVolume();
    volume *= mFadeVolume.GetValue();
    volume *= mPauseFadeVolume.GetValue();
    volume *= GetAmbientParam().volume;

    pan = 0.0f;
    pan += GetPan();
    pan += GetAmbientParam().pan;

    pitch = 1.0f;
    pitch *= GetPitch();

    surroundPan = 0.0f;
    surroundPan += GetSurroundPan();
    surroundPan += GetAmbientParam().surroundPan;

    mainOutVol = 1.0f;
    mainOutVol *= mSoundPlayer->detail_GetMainOutVolume();
    mainOutVol *= GetMainOutVolume();

    int outputLine = OUTPUT_LINE_MAIN;
    if (mSoundPlayer->detail_IsEnabledOutputLine()) {
        outputLine = mSoundPlayer->detail_GetOutputLine();
    }
    if (mOutputLineFlagEnable) {
        outputLine = GetOutputLine();
    }

    f32 remoteOutVol[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVol[i] = 1.0f;
        remoteOutVol[i] *= mSoundPlayer->GetRemoteOutVolume(i);
        remoteOutVol[i] *= GetRemoteOutVolume(i);
    }

    rPlayer.SetVolume(volume);
    rPlayer.SetPan(pan);
    rPlayer.SetSurroundPan(surroundPan);
    rPlayer.SetPitch(pitch);
    rPlayer.SetOutputLine(outputLine);
    rPlayer.SetMainOutVolume(mainOutVol);

    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        rPlayer.SetRemoteOutVolume(i, remoteOutVol[i]);
    }

    if (mFadeOutFlag && mFadeVolume.IsFinished()) {
        mFadeOutFlag = false;
        Shutdown();
        return;
    }

    if (mPauseFadeFlag && mPauseFadeVolume.IsFinished()) {
        mPauseFadeFlag = false;

        if (mPauseFlag) {
            rPlayer.Pause(mPauseFlag);
        }
    }

    if (startPlayer && rPlayer.Start()) {
        mStartedFlag = true;
        mStartFlag = false;
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

    mSoundPlayer->detail_RemovePriorityList(this);
    mSoundPlayer->detail_RemoveSoundList(this);

    if (mExtSoundPlayer != NULL) {
        mExtSoundPlayer->RemoveSoundList(this);
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
    mInitVolume = ut::Clamp(vol, lbl_eu_80669EEC, lbl_eu_80669EE8);
}

void BasicSound::SetVolume(f32 vol, int frames) {
    f32 target;
    if (vol > lbl_eu_80669EE8) {
        target = lbl_eu_80669EE8;
    } else if (vol < lbl_eu_80669EEC) {
        target = lbl_eu_80669EEC;
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
// LLM-HARNESS-BEGIN: us-80415e64
extern "C" int GetRuntimeTypeInfo__Q44nw4r3snd6detail10BasicSoundCFv() { return 0; }
// LLM-HARNESS-END: us-80415e64
