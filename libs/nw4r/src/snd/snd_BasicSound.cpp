#include <nw4r/snd.h>

#include <climits>
#include <cstring>

namespace nw4r {
namespace snd {
namespace detail {

extern "C" nw4r::ut::detail::RuntimeTypeInfo lbl_eu_806654D0(NULL);

namespace {

// Retail AmbientParamUpdateCallback carries a 4th virtual (GetVoiceOutCount)
// at vtable+0x14 that snd_BasicSound.h does not declare (header is outside the
// writable scope). Mirror it via derivation so SetAmbientInfo dispatches at the
// correct slot.
struct AmbientParamUpdateCallbackV4
    : public BasicSound::AmbientParamUpdateCallback {
    virtual u32 GetVoiceOutCount(void* arg, u32 param) = 0; // at 0x14
};

// Retail AmbientArgAllocaterCallback declares a virtual dtor (vtable: RTTI,
// dtor, dtor, Alloc@0xC, Free@0x10); the header's version without an explicit
// dtor compiles Alloc at 0x8. Mirror the retail layout.
struct AmbientArgAllocaterCallbackRetail {
    virtual ~AmbientArgAllocaterCallbackRetail() {} // at 0x4, 0x8
    virtual void* detail_AllocAmbientArg(u32 size) = 0; // at 0xC
    virtual void detail_FreeAmbientArg(void* pArg,
                                       const BasicSound* pSound) = 0; // at 0x10
};

// SoundPlayer fields at 0x30/0x3C/0x40/0x54/0x58 that UpdateParam reads are
// private in snd_SoundPlayer.h; mirror the layout to read them directly.
struct SoundPlayerParamView {
    u8 _pad0x00[0x30];        // 0x00..0x2F
    f32 unk30;                // 0x30
    u8 _pad0x34[0x08];        // 0x34..0x3B
    int outputLineFlagEnable; // 0x3C
    f32 unk40;                // 0x40
    u8 _pad0x44[0x10];        // 0x44..0x53
    f32 unk54;                // 0x54
    f32 fxSend[AUX_BUS_NUM];  // 0x58..0x63
};

// BasicSound fields that SetAmbientInfo writes are private in snd_BasicSound.h
// (which also does not declare SetAmbientInfo - header is read-only this
// session); mirror the layout to implement the retail symbol as a free
// function.
struct BasicSoundAmbientView {
    u8 _pad0x00[0x1C];             // 0x00..0x1B
    void* paramUpdateCallback;     // 0x1C
    void* argUpdateCallback;       // 0x20
    void* argAllocaterCallback;    // 0x24
    void* ambientArg;              // 0x28
    u32 unk2C;                     // 0x2C
    u8 _pad0x30[0x95 - 0x30];      // 0x30..0x94
    u8 unk95;                      // 0x95
    u8 _pad0x96[0x98 - 0x96];      // 0x96..0x97
    u32 id;                        // 0x98
};

} // namespace

// BasicPlayer::SetBiquadFilter is not declared in snd_BasicPlayer.h (header is
// read-only this session); the retail symbol is referenced verbatim so
// UpdateParam can emit the `bl`.
extern "C" void SetBiquadFilter__Q44nw4r3snd6detail11BasicPlayerFif(
    BasicPlayer* pPlayer, int filter, f32 value);

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

int BasicSound::GetVoiceOutCount() const {
    return mUnk0x95;
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
    f32 volume = lbl_eu_80669EE8;
    volume *= mInitVolume;
    volume *= mSoundPlayer->GetVolume();
    volume *= mExtMoveVolume.GetValue();
    volume *= mFadeVolume.GetValue();
    volume *= mPauseFadeVolume.GetValue();
    volume *= mAmbientParam.volume;
    volume *= mUnk0x54;

    f32 pan = lbl_eu_80669EEC;
    pan += mExtPan;
    pan += mAmbientParam.pan;
    pan += mUnk0x5C;

    f32 surroundPan = lbl_eu_80669EEC;
    surroundPan += mExtSurroundPan;
    surroundPan += mAmbientParam.surroundPan;

    f32 pitch = lbl_eu_80669EE8;
    pitch *= mExtPitch;
    pitch *= mAmbientParam.pitch;
    pitch *= mUnk0x58;

    f32 lpfFreq = mUnk0xBC;
    lpfFreq += mAmbientParam.lpf;
    lpfFreq += reinterpret_cast<const SoundPlayerParamView*>(mSoundPlayer)->unk30;

    // Biquad filter id/value: the sound's own, else the player's, else the
    // ambient info's (retail reads the player fields directly).
    u32 filter = mOutputLineFlagEnable;
    f32 filterValue = mUnk0xC0;
    if (filter == 0) {
        const SoundPlayerParamView* pPlayer =
            reinterpret_cast<const SoundPlayerParamView*>(mSoundPlayer);
        filter = pPlayer->outputLineFlagEnable;
        filterValue = pPlayer->unk40;
        if (filter == 0) {
            filter = mAmbientParam.priority;
            filterValue = mAmbientParam.unk18;
        }
    }

    f32 mainOutVol = lbl_eu_80669EE8;
    mainOutVol *= mMainOutVolume;
    mainOutVol *= mSoundPlayer->detail_GetMainOutVolume();

    // Retail loads the output-line flag before the loops and reuses it after.
    int outputLine = mOutputLineFlag;

    f32 remoteOutVol[WPAD_MAX_CONTROLLERS];
    for (int i = 0; i < WPAD_MAX_CONTROLLERS; i++) {
        remoteOutVol[i] = lbl_eu_80669EE8;
        remoteOutVol[i] *= mSoundPlayer->GetRemoteOutVolume(i);
        remoteOutVol[i] *= mRemoteOutVolume[i];
    }

    // Main send + aux sends: sound's own (mUnk0xCC) + player sends + ambient.
    const SoundPlayerParamView* pPlayer =
        reinterpret_cast<const SoundPlayerParamView*>(mSoundPlayer);
    f32 mainSend = lbl_eu_80669EEC;
    f32 fxSend[AUX_BUS_NUM] = { lbl_eu_80669EEC, lbl_eu_80669EEC,
                                lbl_eu_80669EEC };
    mainSend += mUnk0xCC[0];
    fxSend[0] += mUnk0xCC[1];
    fxSend[1] += mUnk0xCC[2];
    fxSend[2] += mUnk0xCC[3];
    mainSend += pPlayer->unk54;
    fxSend[0] += pPlayer->fxSend[0];
    fxSend[1] += pPlayer->fxSend[1];
    fxSend[2] += pPlayer->fxSend[2];
    fxSend[0] += mAmbientParam.fxSend;

    // Retail fetches the player only once all values are computed.
    BasicPlayer& rPlayer = GetBasicPlayer();

    rPlayer.SetVolume(volume);
    rPlayer.SetPan(pan);
    rPlayer.SetSurroundPan(surroundPan);
    rPlayer.SetPitch(pitch);
    rPlayer.SetLpfFreq(lpfFreq);
    SetBiquadFilter__Q44nw4r3snd6detail11BasicPlayerFif(
        &rPlayer, static_cast<int>(filter), filterValue);
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

void BasicSound::DetachExternalSoundPlayer(ExternalSoundPlayer* pExtPlayer) {
    (void)pExtPlayer;
    *(ExternalSoundPlayer**)((u8*)this + 0x18) = NULL;
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
    return &lbl_eu_806654D0;
}

} // namespace detail
} // namespace snd
} // namespace nw4r

// Retail symbol for BasicSound::SetAmbientInfo(const AmbientInfo&); the header
// does not declare it (read-only this session), so define the mangled name as
// a C-linkage free function with an explicit self pointer.
// Absorb: pad .sbss 0x4->0x8
unsigned int snd_BasicSound_sbss_pad;

extern "C" void
SetAmbientInfo__Q44nw4r3snd6detail10BasicSoundFRCQ54nw4r3snd6detail10BasicSound11AmbientInfo(
    nw4r::snd::detail::BasicSound* self,
    const nw4r::snd::detail::BasicSound::AmbientInfo& info) {
    using namespace nw4r::snd::detail;

    BasicSoundAmbientView* pSound =
        reinterpret_cast<BasicSoundAmbientView*>(self);

    AmbientArgAllocaterCallbackRetail* pAlloc =
        reinterpret_cast<AmbientArgAllocaterCallbackRetail*>(
            info.argAllocaterCallback);
    void* pArg = pAlloc->detail_AllocAmbientArg(info.argSize);
    if (pArg != NULL) {
        std::memcpy(pArg, info.arg, info.argSize);

        // Retail keeps a dead store of info.arg into mAmbientArg before it is
        // overwritten by pArg.
        // Evaluate the NULL check before the stores: the store-side reload of
        // info.paramUpdateCallback then happens after the (possibly aliasing)
        // member stores, matching retail.
        // Retail keeps a dead store of info.arg into mAmbientArg before it is
        // overwritten by pArg.
        pSound->ambientArg = info.arg;

        pSound->paramUpdateCallback = info.paramUpdateCallback;
        pSound->argUpdateCallback = info.argUpdateCallback;
        pSound->argAllocaterCallback = info.argAllocaterCallback;
        pSound->unk2C = info.argSize;
        pSound->ambientArg = pArg;

        // Second textual use of info.paramUpdateCallback.
        if (info.paramUpdateCallback != NULL) {
            AmbientParamUpdateCallbackV4* pCallback =
                static_cast<AmbientParamUpdateCallbackV4*>(
                    info.paramUpdateCallback);
            int voiceOut =
                static_cast<int>(pCallback->GetVoiceOutCount(pArg, pSound->id));
            if (voiceOut > 4) {
                voiceOut = 4;
            }
            pSound->unk95 = static_cast<u8>(voiceOut);
        }
    }
}

