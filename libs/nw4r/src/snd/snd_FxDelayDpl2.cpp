#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {

namespace detail {

struct FxDelayParam {
    f32 delay;        // at 0x0 => delayTime
    f32 feedback;     // at 0x4
    f32 outGain;      // at 0x8
    f32 delayTimeMax; // at 0xC => delayTimeMax
    f32 iir;          // at 0x10
};

} // namespace detail

class FxDelayDpl2 : public FxBase {
public:
    FxDelayDpl2();

    virtual ~FxDelayDpl2() {
        Shutdown();
        ReleaseWorkBuffer();
    } // at 0x8

    virtual bool StartUp();  // at 0xC
    virtual void Shutdown(); // at 0x10

    virtual void UpdateBuffer(int channels, void** ppBuffer, u32 size,
                              SampleFormat format, f32 sampleRate,
                              OutputMode mode); // at 0x14

    virtual bool AssignWorkBuffer(void* pBuffer, u32 size); // at 0x18
    virtual void ReleaseWorkBuffer();                       // at 0x1C

    u32 GetRequiredMemSize();
    bool SetParam(const detail::FxDelayParam& rParam);
    void OnChangeOutputMode();

private:
    // Work-buffer memory state, kept flat so the param mirrors at +0x1C can
    // overlap where a nested AxfxImpl would place mIsActive.
    bool mIsActive;  // at 0xC
    int mOutputMode; // at 0x10
    MEMiHeapHead* mHeap; // at 0x14
    u32 mAllocCount;     // at 0x18

    f32 mDelay;        // at 0x1C
    f32 mFeedback;     // at 0x20
    f32 mOutGain;      // at 0x24
    f32 mDelayTimeMax; // at 0x28
    f32 mIir;          // at 0x2C

    AXFX_DELAY_EXP mDelayExp;       // at 0x30
    AXFX_DELAY_EXP_DPL2 mDelayExpDpl2; // at 0x88

    detail::AxfxImpl* GetAxfxImpl() {
        return reinterpret_cast<detail::AxfxImpl*>(&mHeap);
    }
};

FxDelayDpl2::FxDelayDpl2()
    : mIsActive(false), mHeap(NULL), mAllocCount(0),
      mDelay(160.0f), mFeedback(0.4f), mOutGain(1.0f), mDelayTimeMax(160.0f),
      mIir(1.0f) {
    detail::FxDelayParam param;
    param.delay = 160.0f;       // at 0x0
    param.feedback = 0.4f;      // at 0x4
    param.outGain = 1.0f;       // at 0x8
    param.delayTimeMax = 160.0f; // at 0xC
    param.iir = 1.0f;            // at 0x10
    SetParam(param);
}

u32 FxDelayDpl2::GetRequiredMemSize() {
    u32 mono = (AXFXDelayExpGetMemSize(&mDelayExp) + 135) & ~31U;
    u32 dpl2 = (AXFXDelayExpGetMemSizeDpl2(&mDelayExpDpl2) + 135) & ~31U;
    return (dpl2 < mono) ? mono : dpl2;
}

bool FxDelayDpl2::AssignWorkBuffer(void* pBuffer, u32 size) {
    return GetAxfxImpl()->CreateHeap(pBuffer, size);
}

void FxDelayDpl2::ReleaseWorkBuffer() {
    GetAxfxImpl()->DestroyHeap();
}

bool FxDelayDpl2::StartUp() {
    u32 memSize = (AXFXDelayExpGetMemSize(&mDelayExp) + 135) & ~31U;
    u32 memSizeDpl2 =
        (AXFXDelayExpGetMemSizeDpl2(&mDelayExpDpl2) + 135) & ~31U;
    u32 required =
        (memSizeDpl2 < memSize) ? memSize : memSizeDpl2;

    if (required > GetAxfxImpl()->GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;

    BOOL success;
    if (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2) {
        mOutputMode = 1;
        GetAxfxImpl()->HookAlloc(&allocHook, &freeHook);
        success = AXFXDelayExpInitDpl2(&mDelayExpDpl2);
        GetAxfxImpl()->RestoreAlloc(allocHook, freeHook);
        (void)AXFXDelayExpGetMemSizeDpl2(&mDelayExpDpl2); // debug leftover
    } else {
        mOutputMode = 0;
        GetAxfxImpl()->HookAlloc(&allocHook, &freeHook);
        success = AXFXDelayExpInit(&mDelayExp);
        GetAxfxImpl()->RestoreAlloc(allocHook, freeHook);
        (void)AXFXDelayExpGetMemSize(&mDelayExp); // debug leftover
    }

    mIsActive = true;
    return success;
}

void FxDelayDpl2::Shutdown() {
    if (!mIsActive) {
        return;
    }

    mIsActive = false;

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    GetAxfxImpl()->HookAlloc(&allocHook, &freeHook);

    if (mOutputMode == 1) {
        AXFXDelayExpShutdownDpl2(&mDelayExpDpl2);
    } else {
        AXFXDelayExpShutdown(&mDelayExp);
    }

    GetAxfxImpl()->RestoreAlloc(allocHook, freeHook);
}

// SetParam clamp constants (values at retail sdata2 lbl_eu_80669F8C / F90).
static const f32 cMinDelay = 0.0f;    // delay / delayTimeMax lower bound
static const f32 cMaxFeedback = 0.9f; // feedback upper bound

bool FxDelayDpl2::SetParam(const detail::FxDelayParam& rParam) {
    // Declared before `changed` so MWCC's reverse declaration-order callee-saved
    // allocation puts affected (mono memsize) in r31 and `changed` in r30.
    u32 memSize;
    u32 required;
    u32 heapTotal;

    mDelay = rParam.delay;
    mFeedback = rParam.feedback;
    mOutGain = rParam.outGain;
    mDelayTimeMax = rParam.delayTimeMax;
    mIir = rParam.iir;

    // Clamp delayTimeMax: max(x, minDelay)
    f32 clampedMax = ut::Max(rParam.delayTimeMax, cMinDelay);
    bool changed = (clampedMax != mDelayExp.delayTimeMax);
    mDelayExpDpl2.delayTimeMax = clampedMax;
    mDelayExp.delayTimeMax = clampedMax;

    // Clamp delayTime into [minDelay, delayTimeMax]
    f32 clampedDelay = ut::Clamp(rParam.delay, cMinDelay, rParam.delayTimeMax);
    mDelayExpDpl2.delayTime = clampedDelay;
    mDelayExp.delayTime = clampedDelay;

    // Clamp feedback to [0, maxFeedback]
    f32 clampedFeedback = ut::Clamp(rParam.feedback, 0.0f, cMaxFeedback);
    mDelayExpDpl2.feedback = clampedFeedback;
    mDelayExp.feedback = clampedFeedback;

    // Clamp iir to [0, 1]
    f32 clampedIir = ut::Clamp(rParam.iir, 0.0f, 1.0f);
    mDelayExpDpl2.iir = clampedIir;
    mDelayExp.iir = clampedIir;

    // Clamp outGain to [0, 1]
    f32 clampedOutGain = ut::Clamp(rParam.outGain, 0.0f, 1.0f);
    mDelayExpDpl2.outGain = clampedOutGain;
    mDelayExp.outGain = clampedOutGain;

    mDelayExp.busIn = NULL;
    mDelayExp.busOut = NULL;
    mDelayExp.sendGain = 0.0f;
    mDelayExpDpl2.busIn = NULL;
    mDelayExpDpl2.busOut = NULL;
    mDelayExpDpl2.sendGain = 0.0f;

    if (!mIsActive) {
        return true;
    }

    memSize = (AXFXDelayExpGetMemSize(&mDelayExp) + 135) & ~31U;
    required =
        (AXFXDelayExpGetMemSizeDpl2(&mDelayExpDpl2) + 135) & ~31U;
    if (required < memSize) {
        required = memSize;
    }
    heapTotal = (u32)GetAxfxImpl()->GetHeapTotalSize();
    if (required > heapTotal) {
        return false;
    }

    BOOL success;
    if (changed) {
        AXFXAllocHook allocHook;
        AXFXFreeHook freeHook;
        GetAxfxImpl()->HookAlloc(&allocHook, &freeHook);

        if (mOutputMode == 1) {
            success = AXFXDelayExpSettingsDpl2(&mDelayExpDpl2);
        } else {
            success = AXFXDelayExpSettings(&mDelayExp);
        }

        GetAxfxImpl()->RestoreAlloc(allocHook, freeHook);
    } else if (mOutputMode == 1) {
        success = AXFXDelayExpSettingsUpdateDpl2(&mDelayExpDpl2);
    } else {
        success = AXFXDelayExpSettingsUpdate(&mDelayExp);
    }

    return success;
}

void FxDelayDpl2::UpdateBuffer(int channels, void** ppBuffer, u32 size,
                               SampleFormat format, f32 sampleRate,
                               OutputMode mode) {
#pragma unused(channels)
#pragma unused(size)
#pragma unused(format)
#pragma unused(sampleRate)

    if (!mIsActive) {
        return;
    }

    bool isDpl2 = (mode == OUTPUT_MODE_DPL2);
    if (isDpl2 != mOutputMode) {
        return;
    }

    if (mOutputMode == 1) {
        AXFX_BUFFERUPDATE_DPL2 buf;
        buf.left = (s32*)ppBuffer[0];
        buf.right = (s32*)ppBuffer[1];
        buf.left_surround = (s32*)ppBuffer[2];
        buf.right_surround = (s32*)ppBuffer[3];
        AXFXDelayExpCallbackDpl2(&buf, &mDelayExpDpl2);
    } else {
        AXFX_BUFFERUPDATE buf;
        buf.left = (s32*)ppBuffer[0];
        buf.right = (s32*)ppBuffer[1];
        buf.surround = (s32*)ppBuffer[2];
        AXFXDelayExpCallback(&buf, &mDelayExp);
    }
}

void FxDelayDpl2::OnChangeOutputMode() {
    int isDpl2 =
        detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2;
    if (mOutputMode == isDpl2) {
        return;
    }

    Shutdown();
    StartUp();
}

} // namespace snd
} // namespace nw4r