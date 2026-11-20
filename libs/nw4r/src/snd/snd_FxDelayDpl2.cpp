#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Retail-owned data referenced by name so the ctor's relocations match the
// stripped retail object (MWCC_CASES 1b float pools / retail vtables).
// Global scope: MWCC does not mangle file-scope names, so plain extern emits
// the exact retail symbols.
//
// FxDelayDpl2's vtable is emitted by MWCC from the real class below.

extern const f32 lbl_eu_80669F90; // feedback clamp bound

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
    } // at 0x8 (inline; unused here so discarded, like retail)

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

FxDelayDpl2::FxDelayDpl2() {
    mIsActive = false;    // 0xc
    mHeap = NULL;         // 0x14
    mAllocCount = 0;      // 0x18

    mDelay = 160.0f;        // 0x1c, 160.0f
    mFeedback = 0.4f;     // 0x20, 0.4f
    mOutGain = 1.0f;      // 0x24, 1.0f
    mDelayTimeMax = 160.0f; // 0x28, 160.0f
    mIir = 1.0f;          // 0x2c, 1.0f

    detail::FxDelayParam param;
    param.delay = 160.0f;
    param.feedback = 0.4f;
    param.outGain = 1.0f;
    param.delayTimeMax = 160.0f;
    param.iir = 1.0f;
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
    // Retail inlines GetRequiredMemSize() here (the two AXFX bls appear in
    // the StartUp body), so call it and let MWCC inline the same shape; the
    // max value then flows from the inlined return in r3 like retail.
    u32 memSize = GetRequiredMemSize();

    if (memSize > GetAxfxImpl()->GetHeapTotalSize()) {
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

// SetParam clamp constants. Retail keeps each clamp bound as its own sdata2
// object: the delay/delayTimeMax lower bound is lbl_eu_80669F8C (0.0f), the
// feedback cap is lbl_eu_80669F90 (0.9f). Referencing the blob labels
// directly stops MWCC from value-merging them with the plain 0.0f/1.0f
// literals (which retail pools separately as 0.0f / 80669F88).
extern "C" const f32 lbl_eu_80669F8C; // 0.0f
extern "C" const f32 lbl_eu_80669F90; // 0.9f

bool FxDelayDpl2::SetParam(const detail::FxDelayParam& rParam) {
    // Register-allocation-sensitive: MWCC assigns callee-saved locals in reverse
    // declaration order (last-declared live local -> lowest r30/r31), so `memSize`
    // (mono) is declared before `changed` to land mono in r31 and changed in r30.
    // The heap-total is read through named `pHeap`/`total` locals so the DPL2
    // memsize (`required`) keeps the call-return register r3 (matches retail).
    u32 memSize;
    u32 required;

    mDelay = rParam.delay;
    mFeedback = rParam.feedback;
    mOutGain = rParam.outGain;
    mDelayTimeMax = rParam.delayTimeMax;
    mIir = rParam.iir;

    // Clamp delayTimeMax: max(x, minDelay)
    f32 clampedMax = ut::Max(rParam.delayTimeMax, lbl_eu_80669F8C);
    bool changed = (clampedMax != mDelayExp.delayTimeMax);
    mDelayExpDpl2.delayTimeMax = clampedMax;
    mDelayExp.delayTimeMax = clampedMax;

    // Clamp delayTime into [minDelay, delayTimeMax]
    f32 clampedDelay = ut::Clamp(rParam.delay, lbl_eu_80669F8C, rParam.delayTimeMax);
    mDelayExpDpl2.delayTime = clampedDelay;
    mDelayExp.delayTime = clampedDelay;

    // Clamp feedback to [maxFeedback]
    f32 clampedFeedback = ut::Clamp(rParam.feedback, lbl_eu_80669F90, 0.0f);
    mDelayExpDpl2.feedback = clampedFeedback;
    mDelayExp.feedback = clampedFeedback;

    // Clamp iir to [1, max]
    f32 clampedIir = ut::Clamp(rParam.iir, 1.0f, 0.0f);
    mDelayExpDpl2.iir = clampedIir;
    mDelayExp.iir = clampedIir;

    // Clamp outGain to [1, max]
    f32 clampedOutGain = ut::Clamp(rParam.outGain, 1.0f, 0.0f);
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

    u32 total;
    MEMiHeapHead* pHeap = mHeap;
    if (pHeap == NULL) {
        total = 0;
    } else {
        total = (u32)MEMGetHeapTotalSize(pHeap);
    }
    if (required > total) {
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