#include <nw4r/snd.h>
#include <nw4r/ut.h>

namespace nw4r {
namespace snd {
namespace detail {

// The parameter blob consumed by SetParam and stored as mParam. Lives in
// detail so the SetParam mangled name matches retail
// (SetParam__...FRCQ44nw4r3snd6detail16FxReverbStdParam).
struct FxReverbStdParam {
    f32 preDelayTime;     // 0x0
    f32 fusedTime;        // 0x4
    f32 coloration;       // 0x8
    f32 damping;          // 0xC
    f32 outGain;          // 0x10
    u32 iirType;          // 0x14
    f32 preDelayTimeMax;  // 0x18
    u32 fusedMode;        // 0x1C
    f32 earlyGain;        // 0x20
    f32 fusedGain;        // 0x24
};

} // namespace detail

// Retail .sdata2 float constants referenced by name so the SDA21 relocations
// match the stripped retail object (MWCC_REFERENCE §1b float pools).
extern "C" const f32 lbl_eu_80669F98; // preDelayTime / preDelayTimeMax default
extern "C" const f32 lbl_eu_80669F9C; // fusedTime default
extern "C" const f32 lbl_eu_80669FA0; // coloration default
extern "C" const f32 lbl_eu_80669FA4; // damping default
extern "C" const f32 lbl_eu_80669FA8; // outGain / fusedGain default, upper clamp
extern "C" const f32 lbl_eu_80669FAC; // sendGain, lower clamp bound

class FxReverbStdDpl2 : public FxBase {
public:
    FxReverbStdDpl2();

    virtual ~FxReverbStdDpl2() {
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
    bool SetParam(const detail::FxReverbStdParam& rParam);
    void OnChangeOutputMode();

private:
    // The embedded heap sub-object occupies only {mHeap, mAllocCount};
    // detail::AxfxImpl's mIsActive member is not represented here (this class
    // keeps its own active flag at 0xC) and the AxfxImpl methods called on it
    // (CreateHeap/DestroyHeap/HookAlloc/RestoreAlloc/...) only touch those two
    // fields plus the global shared alloc hook state.
    bool mIsActive;               // at 0xC
    s32 mOutputMode;              // at 0x10
    MEMiHeapHead* mHeap;          // at 0x14
    u32 mAllocCount;              // at 0x18

    detail::FxReverbStdParam mParam; // at 0x1C (0x28 bytes -> 0x44)
    AXFX_REVERBSTD_EXP mMono;    // at 0x44 (0xE8 bytes)
    AXFX_REVERBSTD_EXP_DPL2 mDpl2; // at 0x12C (0x104 bytes)

    detail::AxfxImpl* GetImpl() {
        return reinterpret_cast<detail::AxfxImpl*>(&mHeap);
    }
};

FxReverbStdDpl2::FxReverbStdDpl2()
    : mIsActive(false) {
    mHeap = nullptr; // 0x14
    mAllocCount = 0; // 0x18

    mParam.preDelayTime = lbl_eu_80669F98;
    mParam.fusedTime = lbl_eu_80669F9C;
    mParam.coloration = lbl_eu_80669FA0;
    mParam.damping = lbl_eu_80669FA4;
    mParam.outGain = lbl_eu_80669FA8;
    mParam.iirType = 5;
    mParam.preDelayTimeMax = lbl_eu_80669F98;
    mParam.fusedMode = 0;
    mParam.earlyGain = lbl_eu_80669FAC;
    mParam.fusedGain = lbl_eu_80669FA8;

    detail::FxReverbStdParam param;
    param.preDelayTime = lbl_eu_80669F98;
    param.fusedTime = lbl_eu_80669F9C;
    param.coloration = lbl_eu_80669FA0;
    param.damping = lbl_eu_80669FA4;
    param.outGain = lbl_eu_80669FA8;
    param.iirType = 5;
    param.preDelayTimeMax = lbl_eu_80669F98;
    param.fusedMode = 0;
    param.earlyGain = lbl_eu_80669FAC;
    param.fusedGain = lbl_eu_80669FA8;
    SetParam(param);
}

u32 FxReverbStdDpl2::GetRequiredMemSize() {
    u32 memSize = ut::RoundUp(AXFXReverbStdExpGetMemSize(&mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    if (required < memSize) {
        required = memSize;
    }
    return required;
}

bool FxReverbStdDpl2::AssignWorkBuffer(void* pBuffer, u32 size) {
    return GetImpl()->CreateHeap(pBuffer, size);
}

void FxReverbStdDpl2::ReleaseWorkBuffer() {
    GetImpl()->DestroyHeap();
}

bool FxReverbStdDpl2::StartUp() {
    u32 memSize = ut::RoundUp(AXFXReverbStdExpGetMemSize(&mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 total = required < memSize ? memSize : required;
    if (total > GetImpl()->GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;

    BOOL success;
    if (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2) {
        mOutputMode = 1;
        GetImpl()->HookAlloc(&allocHook, &freeHook);

        success = AXFXReverbStdExpInitDpl2(&mDpl2);
        GetImpl()->RestoreAlloc(allocHook, freeHook);

        (void)AXFXReverbStdExpGetMemSizeDpl2(&mDpl2); // debug leftover
    } else {
        mOutputMode = 0;
        GetImpl()->HookAlloc(&allocHook, &freeHook);

        success = AXFXReverbStdExpInit(&mMono);
        GetImpl()->RestoreAlloc(allocHook, freeHook);

        (void)AXFXReverbStdExpGetMemSize(&mMono); // debug leftover
    }

    mIsActive = true;

    return success;
}

void FxReverbStdDpl2::Shutdown() {
    if (!mIsActive) {
        return;
    }

    mIsActive = false;

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    GetImpl()->HookAlloc(&allocHook, &freeHook);

    if (mOutputMode == 1) {
        AXFXReverbStdExpShutdownDpl2(&mDpl2);
    } else {
        AXFXReverbStdExpShutdown(&mMono);
    }

    GetImpl()->RestoreAlloc(allocHook, freeHook);
}

bool FxReverbStdDpl2::SetParam(const detail::FxReverbStdParam& rParam) {
    mParam = rParam;

    f32 preDelayTimeMax =
        ut::Max(rParam.preDelayTimeMax, lbl_eu_80669FAC);
    bool changed = (preDelayTimeMax != mMono.preDelayTimeMax);

    mDpl2.earlyMode = rParam.iirType;
    mMono.earlyMode = rParam.iirType;
    mDpl2.preDelayTimeMax = preDelayTimeMax;
    mMono.preDelayTimeMax = preDelayTimeMax;

    f32 preDelayTime =
        ut::Clamp(rParam.preDelayTime, lbl_eu_80669FAC, rParam.preDelayTimeMax);
    mDpl2.preDelayTime = preDelayTime;
    mMono.preDelayTime = preDelayTime;

    mDpl2.fusedMode = rParam.fusedMode;
    mMono.fusedMode = rParam.fusedMode;

    // Retail clamps fusedTime only on the lower side (ut::Max), unlike the
    // other gain/time fields below which use a full upper/lower Clamp.
    f32 fusedTime = ut::Max(rParam.fusedTime, lbl_eu_80669FAC);
    mDpl2.fusedTime = fusedTime;
    mMono.fusedTime = fusedTime;

    f32 coloration =
        ut::Clamp(rParam.coloration, lbl_eu_80669FAC, lbl_eu_80669FA8);
    mDpl2.coloration = coloration;
    mMono.coloration = coloration;

    f32 damping = ut::Clamp(rParam.damping, lbl_eu_80669FAC, lbl_eu_80669FA8);
    mDpl2.damping = damping;
    mMono.damping = damping;

    f32 earlyGain =
        ut::Clamp(rParam.earlyGain, lbl_eu_80669FAC, lbl_eu_80669FA8);
    mDpl2.earlyGain = earlyGain;
    mMono.earlyGain = earlyGain;

    f32 fusedGain =
        ut::Clamp(rParam.fusedGain, lbl_eu_80669FAC, lbl_eu_80669FA8);
    mDpl2.fusedGain = fusedGain;
    mMono.fusedGain = fusedGain;

    f32 outGain = ut::Clamp(rParam.outGain, lbl_eu_80669FAC, lbl_eu_80669FA8);
    mDpl2.outGain = outGain;
    mMono.outGain = outGain;

    mMono.busIn = NULL;
    mMono.busOut = NULL;
    mMono.sendGain = lbl_eu_80669FAC;
    mDpl2.busIn = NULL;
    mDpl2.busOut = NULL;
    mDpl2.sendGain = lbl_eu_80669FAC;

    if (!mIsActive) {
        return true;
    }

    u32 memSize = ut::RoundUp(AXFXReverbStdExpGetMemSize(&mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    if (required < memSize) {
        required = memSize;
    }

    // Available work-buffer space = end offset of the heap.
    MEMiHeapHead* heap = mHeap;
    u32 total;
    if (heap == NULL) {
        total = 0;
    } else {
        total = (u32)heap->end - (u32)heap;
    }
    if (required > total) {
        return false;
    }

    BOOL success;
    if (changed) {
        AXFXAllocHook allocHook;
        AXFXFreeHook freeHook;
        GetImpl()->HookAlloc(&allocHook, &freeHook);

        if (mOutputMode == 1) {
            success = AXFXReverbStdExpSettingsDpl2(&mDpl2);
        } else {
            success = AXFXReverbStdExpSettings(&mMono);
        }

        GetImpl()->RestoreAlloc(allocHook, freeHook);
    } else if (mOutputMode == 1) {
        success = AXFXReverbStdExpSettingsUpdateDpl2(&mDpl2);
    } else {
        success = AXFXReverbStdExpSettingsUpdate(&mMono);
    }

    return success != 0;
}

void FxReverbStdDpl2::UpdateBuffer(int channels, void** ppBuffer, u32 size,
                                   SampleFormat format, f32 sampleRate,
                                   OutputMode mode) {
#pragma unused(channels)
#pragma unused(size)
#pragma unused(format)
#pragma unused(sampleRate)

    if (!mIsActive) {
        return;
    }

    if ((mode == OUTPUT_MODE_DPL2) != mOutputMode) {
        return;
    }

    if (mOutputMode == 1) {
        AXFX_BUFFERUPDATE_DPL2 buffer;
        buffer.left = (s32*)ppBuffer[0];
        buffer.right = (s32*)ppBuffer[1];
        buffer.left_surround = (s32*)ppBuffer[2];
        buffer.right_surround = (s32*)ppBuffer[3];
        AXFXReverbStdExpCallbackDpl2(&buffer, &mDpl2);
    } else {
        AXFX_BUFFERUPDATE buffer;
        buffer.left = (s32*)ppBuffer[0];
        buffer.right = (s32*)ppBuffer[1];
        buffer.surround = (s32*)ppBuffer[2];
        AXFXReverbStdExpCallback(&buffer, &mMono);
    }
}

void FxReverbStdDpl2::OnChangeOutputMode() {
    bool dpl2 =
        (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2);
    if (mOutputMode == dpl2) {
        return;
    }

    Shutdown();
    StartUp();
}

} // namespace snd
} // namespace nw4r