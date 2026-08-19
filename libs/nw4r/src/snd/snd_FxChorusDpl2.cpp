#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Global scope: MWCC does not mangle file-scope names, so plain extern emits
// the exact retail symbols.
//
// FxChorusDpl2 vtable (shared data pool, nw4r_data.s lbl_eu_8056A798). The
// slot type is only forward-declared: this TU never indexes the table, it
// just stores the label's address into the object's vptr.
struct FxChorusDpl2Vtbl;
extern FxChorusDpl2Vtbl lbl_eu_8056A798[];

namespace nw4r {
namespace snd {
namespace detail {

// The parameter blob consumed by SetParam and stored as mParam. Lives in
// detail so the SetParam mangled name matches retail
// (SetParam__...FRCQ44nw4r3snd6detail13FxChorusParam).
struct FxChorusParam {
    f32 delayTime; // 0x0
    f32 depth;     // 0x4
    f32 rate;      // 0x8
    f32 feedback;  // 0xC
    f32 outGain;   // 0x10
};

}  // namespace detail

// Retail .sdata2 float constants referenced by name so the SDA21 relocations
// match the stripped retail object (MWCC_CASES §1b float pools).
extern "C" const f32 lbl_eu_80669F60; // default delayTime
extern "C" const f32 lbl_eu_80669F64; // default depth
extern "C" const f32 lbl_eu_80669F68; // default rate/outGain; depth+outGain max
extern "C" const f32 lbl_eu_80669F6C; // default feedback; min + sendGain
extern "C" const f32 lbl_eu_80669F70; // delayTime max
extern "C" const f32 lbl_eu_80669F74; // delayTime min / rate min
extern "C" const f32 lbl_eu_80669F78; // rate max
extern "C" const f32 lbl_eu_80669F7C; // feedback max

// __declspec(novtable): the retail vtable is shared data (lbl_eu_8056A798),
// so MWCC must not emit a local __vt__FxChorusDpl2; the ctor stores the
// extern vtable address explicitly (retail's vptr store is in the ctor body).
class __declspec(novtable) FxChorusDpl2 : public FxBase {
public:
    FxChorusDpl2();

    virtual ~FxChorusDpl2() {
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
    bool SetParam(const detail::FxChorusParam& rParam);
    void OnChangeOutputMode();

private:
    // The embedded heap sub-object occupies only {mHeap, mAllocCount};
    // detail::AxfxImpl's mIsActive member is not represented here (this class
    // keeps its own active flag at 0xC) and the AxfxImpl methods called on it
    // (HookAlloc/RestoreAlloc/GetHeapTotalSize/...) only touch those two fields.
    struct WorkHeap {
        MEMiHeapHead* mHeap; // at 0x14
        u32 mAllocCount;     // at 0x18
    };

    bool mIsActive;          // at 0xC
    s32 mOutputMode;         // at 0x10
    WorkHeap mWorkHeap;      // at 0x14
    detail::FxChorusParam mParam; // at 0x1C
    AXFX_CHORUS_EXP mMono;   // at 0x30
    AXFX_CHORUS_EXP_DPL2 mDpl2; // at 0xD0

    detail::AxfxImpl* GetImpl() {
        return reinterpret_cast<detail::AxfxImpl*>(&mWorkHeap);
    }
};

FxChorusDpl2::FxChorusDpl2() {
    // FxBase's implicit ctor zeroes the link node at 0x4/0x8 first; the
    // manual vptr store lands between that and the member stores, matching
    // the retail store order (0x4, 0x8, vptr 0x0, 0xc, 0x14, 0x18, floats).
    *(FxChorusDpl2Vtbl**)this = lbl_eu_8056A798;

    mIsActive = false;         // 0xc
    mWorkHeap.mHeap = nullptr; // 0x14
    mWorkHeap.mAllocCount = 0; // 0x18

    mParam.delayTime = lbl_eu_80669F60;
    mParam.depth     = lbl_eu_80669F64;
    mParam.rate      = lbl_eu_80669F68;
    mParam.feedback  = lbl_eu_80669F6C;
    mParam.outGain   = lbl_eu_80669F68;

    detail::FxChorusParam param;
    param.delayTime = lbl_eu_80669F60;
    param.depth     = lbl_eu_80669F64;
    param.rate      = lbl_eu_80669F68;
    param.feedback  = lbl_eu_80669F6C;
    param.outGain   = lbl_eu_80669F68;
    SetParam(param);
}

u32 FxChorusDpl2::GetRequiredMemSize() {
    u32 memSize = ut::RoundUp(AXFXChorusExpGetMemSize(&mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXChorusExpGetMemSizeDpl2(&mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    if (required < memSize) {
        required = memSize;
    }
    return required;
}

bool FxChorusDpl2::AssignWorkBuffer(void* pBuffer, u32 size) {
    return GetImpl()->CreateHeap(pBuffer, size);
}

void FxChorusDpl2::ReleaseWorkBuffer() {
    GetImpl()->DestroyHeap();
}

bool FxChorusDpl2::StartUp() {
    u32 memSize = ut::RoundUp(AXFXChorusExpGetMemSize(&mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXChorusExpGetMemSizeDpl2(&mDpl2) +
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

        success = AXFXChorusExpInitDpl2(&mDpl2);
        GetImpl()->RestoreAlloc(allocHook, freeHook);

        (void)AXFXChorusExpGetMemSizeDpl2(&mDpl2); // debug leftover
    } else {
        mOutputMode = 0;
        GetImpl()->HookAlloc(&allocHook, &freeHook);

        success = AXFXChorusExpInit(&mMono);
        GetImpl()->RestoreAlloc(allocHook, freeHook);

        (void)AXFXChorusExpGetMemSize(&mMono); // debug leftover
    }

    mIsActive = true;

    return success;
}

void FxChorusDpl2::Shutdown() {
    if (!mIsActive) {
        return;
    }

    mIsActive = false;

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    GetImpl()->HookAlloc(&allocHook, &freeHook);

    if (mOutputMode == 1) {
        AXFXChorusExpShutdownDpl2(&mDpl2);
    } else {
        AXFXChorusExpShutdown(&mMono);
    }

    GetImpl()->RestoreAlloc(allocHook, freeHook);
}

bool FxChorusDpl2::SetParam(const detail::FxChorusParam& rParam) {
    mParam = rParam;

    f32 v = ut::Clamp(rParam.delayTime, lbl_eu_80669F74, lbl_eu_80669F70);
    mDpl2.delayTime = v;
    mMono.delayTime = v;

    v = ut::Clamp(rParam.depth, lbl_eu_80669F6C, lbl_eu_80669F68);
    mDpl2.depth = v;
    mMono.depth = v;

    v = ut::Clamp(rParam.rate, lbl_eu_80669F74, lbl_eu_80669F78);
    mDpl2.rate = v;
    mMono.rate = v;

    v = ut::Clamp(rParam.feedback, lbl_eu_80669F6C, lbl_eu_80669F7C);
    mDpl2.feedback = v;
    mMono.feedback = v;

    v = ut::Clamp(rParam.outGain, lbl_eu_80669F6C, lbl_eu_80669F68);
    mDpl2.outGain = v;
    mMono.outGain = v;

    mMono.busIn = NULL;
    mMono.busOut = NULL;
    mMono.sendGain = lbl_eu_80669F6C;
    mDpl2.busIn = NULL;
    mDpl2.busOut = NULL;
    mDpl2.sendGain = lbl_eu_80669F6C;

    if (!mIsActive) {
        return true;
    }

    u32 memSize = (AXFXChorusExpGetMemSize(&mMono) + 135) & ~31U;
    u32 required = (AXFXChorusExpGetMemSizeDpl2(&mDpl2) + 135) & ~31U;
    if (required < memSize) {
        required = memSize;
    }

    // Available work-buffer space = end offset of the heap (MEMGetHeapTotalSize).
    // Written inline to match retail's register allocation byte-for-byte.
    MEMiHeapHead* heap = mWorkHeap.mHeap;
    u32 total;
    if (heap == NULL) {
        total = 0;
    } else {
        total = (u32)heap->end - (u32)heap;
    }
    if (required > total) {
        return false;
    }

    BOOL result;
    if (mOutputMode == 1) {
        result = AXFXChorusExpSettingsUpdateDpl2(&mDpl2);
    } else {
        result = AXFXChorusExpSettingsUpdate(&mMono);
    }
    return result != 0;
}

void FxChorusDpl2::UpdateBuffer(int channels, void** ppBuffer, u32 size,
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
        AXFXChorusExpCallbackDpl2(&buffer, &mDpl2);
    } else {
        AXFX_BUFFERUPDATE buffer;
        buffer.left = (s32*)ppBuffer[0];
        buffer.right = (s32*)ppBuffer[1];
        buffer.surround = (s32*)ppBuffer[2];
        AXFXChorusExpCallback(&buffer, &mMono);
    }
}

void FxChorusDpl2::OnChangeOutputMode() {
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