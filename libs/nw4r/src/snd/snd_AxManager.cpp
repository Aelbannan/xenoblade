// The biquad filter preset classes are declared in snd_BiquadFilterPreset.h,
// but retail emits their destructors from this translation unit (the last five
// symbols of the AxManager .text split, right after __sinit_). snd.h pulls
// that header in, so skip it here and declare the classes locally so MWCC
// emits the __dt__ symbols from this TU. Nothing else in this TU (or in any
// header it includes) references the BiquadFilter* types.
#define NW4R_SND_BIQUAD_FILTER_PRESET_H
#include <nw4r/snd.h>

#include <revolution/AXFX.h>
#include <revolution/DVD.h>
#include <revolution/OS.h>

#include <cstring>

namespace nw4r {
namespace snd {
namespace detail {

NW4R_UT_LINKLIST_TYPEDEF_FORCE(FxBase);

// Retail .sdata2 float constants referenced by name so the SDA21 relocations
// match the stripped retail object (MWCC_CASES §1b float pools).
extern "C" const f32 lbl_eu_80669E7C; // 0.0f

// The AxManager header ends its layout at 0xF0 (u8 mAuxCallbackWaitCounter[3]).
// Retail's object carries three more u32 words at 0xF4 (the ctor zeroes them;
// ShutdownEffect clears them per bus; AuxCallbackFunc stores OSGetTick deltas;
// nothing in this TU reads them). The header is outside this session's
// writable scope, so access them through a tail-view struct that mirrors the
// real layout instead of raw pointer arithmetic.
struct AxManagerTailView {
    u8 field_0xF0[4];  // 0xF0: waitCounter bytes + 1 pad byte
    u32 field_0xF4[3]; // 0xF4: per-bus tick/state words
};

u8 AxManager::sZeroBuffer[AxManager::ZERO_BUFFER_SIZE];

// NOTE (matching residual): retail constructs the six MoveValue members from
// the ctor init list with 1.0f, emitting the 1.0 member-init stores and the
// fade/user array loops BEFORE the mFxList __construct_array. Reproducing
// that requires a MoveValue value ctor (MoveValue(TValue)), which
// snd_MoveValue.h does not declare (header outside writable scope), so the
// 1.0 member-init phase cannot be emitted; the body below reproduces the
// retail's second pass (the 0.0f InitValue pass + callback zeroing) which
// sets the same final object state as retail.
AxManager::AxManager()
    : mOutputMode(OUTPUT_MODE_STEREO),
      mZeroBufferAddress(NULL),
      mInitialized(false),
      mUpdateVoicePrioFlag(true),
      mOldAidCallback(NULL),
      mResetReadyCounter(-1) {

    mMainOutVolume.InitValue(lbl_eu_80669E7C);
    mMasterVolume.InitValue(lbl_eu_80669E7C);
    mVolumeForReset.InitValue(lbl_eu_80669E7C);

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mAuxFadeVolume[i].InitValue(lbl_eu_80669E7C);
        mAuxUserVolume[i].InitValue(lbl_eu_80669E7C);
        mAuxCallback[i] = NULL;
        mAuxCallbackContext[i] = NULL;
        reinterpret_cast<AxManagerTailView*>(this)->field_0xF4[i] = 0;
    }
}

AxManager& AxManager::GetInstance() {
    static AxManager instance;
    return instance;
}

void AxManager::Init() {
    if (mInitialized) {
        return;
    }

    std::memset(sZeroBuffer, 0, sizeof(sZeroBuffer));
    DCFlushRange(sZeroBuffer, sizeof(sZeroBuffer));
    mZeroBufferAddress = sZeroBuffer;

    ut::AutoInterruptLock lock;

    AXGetAuxACallback(&mAuxCallback[AUX_A], &mAuxCallbackContext[AUX_A]);
    AXGetAuxBCallback(&mAuxCallback[AUX_B], &mAuxCallbackContext[AUX_B]);
    AXGetAuxCCallback(&mAuxCallback[AUX_C], &mAuxCallbackContext[AUX_C]);

    AXRegisterAuxACallback(NULL, NULL);
    AXRegisterAuxBCallback(NULL, NULL);
    AXRegisterAuxCCallback(NULL, NULL);

    mNextAxRegisterCallback = AXRegisterCallback(AxCallbackFunc);

    mInitialized = true;
}

void AxManager::Shutdown() {
    if (!mInitialized) {
        return;
    }

    AXRegisterCallback(mNextAxRegisterCallback);

    ShutdownEffect(AUX_A);
    ShutdownEffect(AUX_B);
    ShutdownEffect(AUX_C);

    AXRegisterAuxACallback(mAuxCallback[AUX_A], mAuxCallbackContext[AUX_A]);
    AXRegisterAuxBCallback(mAuxCallback[AUX_B], mAuxCallbackContext[AUX_B]);
    AXRegisterAuxCCallback(mAuxCallback[AUX_C], mAuxCallbackContext[AUX_C]);

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        mAuxCallback[i] = NULL;
        mAuxCallbackContext[i] = NULL;
    }

    mZeroBufferAddress = NULL;

    mInitialized = false;
}

f32 AxManager::GetOutputVolume() const {
    return mMasterVolume.GetValue();
}

void AxManager::Update() {
    s32 status = DVDGetDriveStatus();

    if (status == DVD_STATE_IDLE || status == DVD_STATE_BUSY) {
        if (mDiskError) {
            mDiskError = false;
            VoiceManager::GetInstance().UpdateAllVoicesSync(
                Voice::SYNC_AX_VOICE);
        }
    } else {
        if (!mDiskError) {
            mDiskError = true;
            VoiceManager::GetInstance().UpdateAllVoicesSync(
                Voice::SYNC_AX_VOICE);
        }
    }

    for (int i = 0; i < AUX_BUS_NUM; i++) {
        bool update = false;

        if (!mAuxUserVolume[i].IsFinished()) {
            mAuxUserVolume[i].Update();
            update = true;
        }

        if (!mAuxFadeVolume[i].IsFinished()) {
            mAuxFadeVolume[i].Update();

            if (mAuxFadeVolume[i].IsFinished()) {
                ShutdownEffect(static_cast<AuxBus>(i));
            }

            update = true;
        }

        if (update) {
            f32 ratio = 1.0f;
            ratio *= ut::Clamp(mAuxUserVolume[i].GetValue(), 0.0f, 1.0f);
            ratio *= ut::Clamp(mAuxFadeVolume[i].GetValue(), 0.0f, 1.0f);
            u16 volume = static_cast<u16>(AX_MAX_VOLUME * ratio);

            switch (i) {
            case AUX_A: {
                AXSetAuxAReturnVolume(volume);
                break;
            }

            case AUX_B: {
                AXSetAuxBReturnVolume(volume);
                break;
            }

            case AUX_C: {
                AXSetAuxCReturnVolume(volume);
                break;
            }
            }
        }
    }

    if (!mMasterVolume.IsFinished()) {
        mMasterVolume.Update();
        VoiceManager::GetInstance().UpdateAllVoicesSync(Voice::SYNC_AX_VE);
    }

    if (!mVolumeForReset.IsFinished()) {
        mVolumeForReset.Update();
    }

    if (!mMainOutVolume.IsFinished()) {
        mMainOutVolume.Update();
    }

    f32 masterRatio = mMainOutVolume.GetValue();
    masterRatio *= mVolumeForReset.GetValue();
    masterRatio = ut::Clamp(masterRatio, 0.0f, 1.0f);
    AXSetMasterVolume(static_cast<u16>(AX_MAX_VOLUME * masterRatio));
}

void* AxManager::GetZeroBufferAddress() {
    return mZeroBufferAddress;
}

void AxManager::RegisterCallback(CallbackListNode* pNode,
                                 AXOutCallback pCallback) {
    ut::AutoInterruptLock lock;
    pNode->callback = pCallback;
    mCallbackList.PushBack(pNode);
}

void AxManager::UnregisterCallback(CallbackListNode* pNode) {
    ut::AutoInterruptLock lock;
    mCallbackList.Erase(pNode);
}

void AxManager::SetOutputMode(OutputMode mode) {
    mOutputMode = mode;

    switch (mode) {
    case OUTPUT_MODE_STEREO: {
        AXSetMode(AX_OUTPUT_STEREO);
        break;
    }

    case OUTPUT_MODE_SURROUND: {
        AXSetMode(AX_OUTPUT_SURROUND);
        break;
    }

    case OUTPUT_MODE_DPL2: {
        AXSetMode(AX_OUTPUT_DPL2);
        break;
    }

    case OUTPUT_MODE_MONO: {
        AXSetMode(AX_OUTPUT_STEREO);
        break;
    }
    }

    VoiceManager::GetInstance().UpdateAllVoicesSync(Voice::SYNC_AX_MIX);
}

OutputMode AxManager::GetOutputMode() {
    return mOutputMode;
}

void AxManager::SetMasterVolume(f32 volume, int frame) {
    mMasterVolume.SetTarget(ut::Clamp(volume, 0.0f, 1.0f), (frame + 2) / 3);

    if (frame == 0) {
        VoiceManager::GetInstance().UpdateAllVoicesSync(Voice::SYNC_AX_VE);
    }
}

void AxManager::AxCallbackFunc() {
    NW4R_UT_LINKLIST_FOREACH_SAFE (it, GetInstance().mCallbackList,
                                   { it->callback(); })

    if (GetInstance().mNextAxRegisterCallback != NULL) {
        GetInstance().mNextAxRegisterCallback();
    }
}

bool AxManager::AppendEffect(AuxBus bus, FxBase* pFx) {
    if (!mAuxFadeVolume[bus].IsFinished()) {
        ShutdownEffect(bus);
    }

    mAuxFadeVolume[bus].SetTarget(1.0f, 0);

    switch (bus) {
    case AUX_A: {
        AXSetAuxAReturnVolume(AX_MAX_VOLUME);
        break;
    }

    case AUX_B: {
        AXSetAuxBReturnVolume(AX_MAX_VOLUME);
        break;
    }

    case AUX_C: {
        AXSetAuxCReturnVolume(AX_MAX_VOLUME);
        break;
    }

    default: {
        break;
    }
    }

    if (!pFx->StartUp()) {
        return false;
    }

    ut::AutoInterruptLock lock;

    if (GetEffectList(bus).IsEmpty()) {
        switch (bus) {
        case AUX_A: {
            AXRegisterAuxACallback(AuxCallbackFunc,
                                   reinterpret_cast<void*>(bus));
            break;
        }

        case AUX_B: {
            AXRegisterAuxBCallback(AuxCallbackFunc,
                                   reinterpret_cast<void*>(bus));
            break;
        }

        case AUX_C: {
            AXRegisterAuxCCallback(AuxCallbackFunc,
                                   reinterpret_cast<void*>(bus));
            break;
        }

        default: {
            break;
        }
        }

        mAuxCallbackWaitCounter[bus] = 2;
    }

    GetEffectList(bus).PushBack(pFx);
    return true;
}

void AxManager::ClearEffect(AuxBus bus, int frame) {
    if (frame == 0) {
        ShutdownEffect(bus);
        return;
    }

    mAuxFadeVolume[bus].SetTarget(0.0f, (frame + 2) / 3);
}

void AxManager::ShutdownEffect(AuxBus bus) {
    ut::AutoInterruptLock lock;

    if (GetEffectList(bus).IsEmpty()) {
        return;
    }

    for (FxBaseList::Iterator it = GetEffectList(bus).GetBeginIter();
         it != GetEffectList(bus).GetEndIter(); ++it) {
        it->Shutdown();
    }

    GetEffectList(bus).Clear();

    switch (bus) {
    case AUX_A: {
        AXRegisterAuxACallback(NULL, NULL);
        break;
    }

    case AUX_B: {
        AXRegisterAuxBCallback(NULL, NULL);
        break;
    }

    case AUX_C: {
        AXRegisterAuxCCallback(NULL, NULL);
        break;
    }

    default: {
        break;
    }
    }
}

void AxManager::AuxCallbackFunc(void* pChans, void* pContext) {
    int num;
    void* buffer[AX_DPL2_MAX];

    void** ppChans = static_cast<void**>(pChans);
    AuxBus bus = static_cast<AuxBus>(reinterpret_cast<u32>(pContext));

    if (GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2) {
        num = AX_DPL2_MAX;

        buffer[AX_DPL2_L] = ppChans[AX_DPL2_L];
        buffer[AX_DPL2_R] = ppChans[AX_DPL2_R];
        buffer[AX_DPL2_LS] = ppChans[AX_DPL2_LS];
        buffer[AX_DPL2_RS] = ppChans[AX_DPL2_RS];
    } else {
        num = AX_STEREO_MAX;

        buffer[AX_STEREO_L] = ppChans[AX_STEREO_L];
        buffer[AX_STEREO_R] = ppChans[AX_STEREO_R];
        buffer[AX_STEREO_S] = ppChans[AX_STEREO_S];
    }

    if (GetInstance().mAuxCallbackWaitCounter[bus] > 0) {
        GetInstance().mAuxCallbackWaitCounter[bus]--;

        for (int i = 0; i < num; i++) {
            std::memset(buffer[i], 0, FX_BUFFER_SIZE);
        }
    } else if (GetInstance().GetEffectList(bus).IsEmpty()) {
        for (int i = 0; i < num; i++) {
            std::memset(buffer[i], 0, FX_BUFFER_SIZE);
        }
    } else {
        for (FxBaseList::Iterator it =
                 GetInstance().GetEffectList(bus).GetBeginIter();
             it != GetInstance().GetEffectList(bus).GetEndIter(); ++it) {

            it->UpdateBuffer(num, buffer, FX_BUFFER_SIZE, FX_SAMPLE_FORMAT,
                             FX_SAMPLE_RATE, GetInstance().GetOutputMode());
        }
    }
}

void AxManager::PrepareReset() {
    if (mOldAidCallback != NULL) {
        return;
    }

    mVolumeForReset.SetTarget(0.0f, 3);
    mResetReadyCounter = -1;
    mOldAidCallback = AIRegisterDMACallback(AiDmaCallbackFunc);
}

void AxManager::AiDmaCallbackFunc() {
    static bool finishedFlag = false;

    AxManager& r = GetInstance();
    r.mOldAidCallback();

    if (finishedFlag) {
        if (r.mResetReadyCounter < 0) {
            AXSetMaxDspCycles(0);
            r.mResetReadyCounter = AUX_CALLBACK_WAIT_FRAME;
        }
    } else if (r.mVolumeForReset.GetValue() == 0.0f) {
        finishedFlag = true;
    }

    if (r.mResetReadyCounter > 0) {
        r.mResetReadyCounter--;
    }
}

// ---------------------------------------------------------------------------
// Biquad filter preset destructors (see the note at the top of this file).
// The filter classes are stateless, so each destructor body is empty; MWCC
// emits the standard "delete when this != 0 and flag != 0" epilogue calling
// __dl__FPv, matching the retail __dt__ bodies.
// ---------------------------------------------------------------------------
class BiquadFilterLpf {
public:
    ~BiquadFilterLpf();
};

class BiquadFilterHpf {
public:
    ~BiquadFilterHpf();
};

class BiquadFilterBpf512 {
public:
    ~BiquadFilterBpf512();
};

class BiquadFilterBpf1024 {
public:
    ~BiquadFilterBpf1024();
};

class BiquadFilterBpf2048 {
public:
    ~BiquadFilterBpf2048();
};

BiquadFilterLpf::~BiquadFilterLpf() {}
BiquadFilterHpf::~BiquadFilterHpf() {}
BiquadFilterBpf512::~BiquadFilterBpf512() {}
BiquadFilterBpf1024::~BiquadFilterBpf1024() {}
BiquadFilterBpf2048::~BiquadFilterBpf2048() {}

} // namespace detail
} // namespace snd
} // namespace nw4r
