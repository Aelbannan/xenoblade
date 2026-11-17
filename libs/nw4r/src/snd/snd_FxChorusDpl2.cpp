#include <nw4r/snd.h>
#include <nw4r/ut.h>

// Global scope: MWCC does not mangle file-scope names, so plain extern emits
// the exact retail symbols.
//
// FxChorusDpl2 vtable (this TU's split .data 0x8056A798, 0x28). The table is
// emitted below as data; the ctor stores its address into the object's vptr.
// Slot layout (retail): {0, 0, dtor, StartUp, Shutdown, UpdateBuffer,
// OnChangeOutputMode, AssignWorkBuffer, ReleaseWorkBuffer, 0}.
struct FxChorusDpl2Vtbl {
    u32 slots[10];
};
extern FxChorusDpl2Vtbl lbl_eu_8056A798;

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

// Standalone layout (no FxBase base): the class reproduces retail's FxBase
// prefix (vptr + intrusive link node) as explicit fields. Inheriting FxBase
// would make MWCC emit the weak __vt__FxBase / weak FxBase+LinkListNode
// dtors into this TU, which retail's split does not carry (retail keeps
// only the 0x28 derived vtable here; the weak base copies link elsewhere).
// __declspec(novtable): MWCC must not emit a local __vt__FxChorusDpl2; the
// ctor stores lbl_eu_8056A798 explicitly (retail's vptr store is in the
// ctor body).
class __declspec(novtable) FxChorusDpl2 {
public:
    FxChorusDpl2();

    // Declared only (virtual: retail slot 0x08 holds the deleting dtor).
    // The dtor lives in the factory TU's linkage (retail __dt__...Dpl2Fv is
    // at 0x801C47F8, inside kyoshin/cf/code_801C2C14.cpp's split, not here).
    virtual ~FxChorusDpl2(); // at 0x8

    virtual bool StartUp();  // at 0xC
    virtual void Shutdown(); // at 0x10

    virtual void UpdateBuffer(int channels, void** ppBuffer, u32 size,
                              SampleFormat format, f32 sampleRate,
                              OutputMode mode); // at 0x14

    virtual void OnChangeOutputMode(); // at 0x18

    virtual bool AssignWorkBuffer(void* pBuffer, u32 size); // at 0x1C
    virtual void ReleaseWorkBuffer();                       // at 0x20

    u32 GetRequiredMemSize();
    bool SetParam(const detail::FxChorusParam& rParam);

private:
    // The embedded heap sub-object occupies only {mHeap, mAllocCount};
    // detail::AxfxImpl's mIsActive member is not represented here (this class
    // keeps its own active flag at 0xC) and the AxfxImpl methods called on it
    // (HookAlloc/RestoreAlloc/GetHeapTotalSize/...) only touch those two fields.
    struct WorkHeap {
        MEMiHeapHead* mHeap; // at 0x14
        u32 mAllocCount;     // at 0x18
    };

    // vptr + intrusive link node (retail FxBase prefix layout, stored
    // explicitly: this TU defines the class standalone so MWCC emits no
    // weak FxBase vtable/dtor support into this TU's split). The vptr itself
    // is the class's implicit vtable pointer at +0x0 (written manually from
    // lbl_eu_8056A798); only the link fields are declared.
    void* linkPrev;          // at 0x4 (intrusive FxBase link node)
    void* linkNext;          // at 0x8

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
    // Retail store order (0x4, 0x8, vptr 0x0, 0xc, 0x14, 0x18, floats): the
    // link node is zeroed explicitly first, then the manual vptr store lands.
    linkPrev = nullptr;      // 0x4
    linkNext = nullptr;      // 0x8
    *(FxChorusDpl2Vtbl**)this = &lbl_eu_8056A798; // 0x0

    mIsActive = false;         // 0xc
    mWorkHeap.mHeap = nullptr; // 0x14
    mWorkHeap.mAllocCount = 0; // 0x18

    mParam.delayTime = 10.0f;
    mParam.depth     = 0.5f;
    mParam.rate      = 1.0f;
    mParam.feedback  = 0.0f;
    mParam.outGain   = 1.0f;

    detail::FxChorusParam param;
    param.delayTime = 10.0f;
    param.depth     = 0.5f;
    param.rate      = 1.0f;
    param.feedback  = 0.0f;
    param.outGain   = 1.0f;
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

    f32 v = ut::Clamp(rParam.delayTime, 0.1f, 50.0f);
    mDpl2.delayTime = v;
    mMono.delayTime = v;

    v = ut::Clamp(rParam.depth, 0.0f, 1.0f);
    mDpl2.depth = v;
    mMono.depth = v;

    v = ut::Clamp(rParam.rate, 0.1f, 2.0f);
    mDpl2.rate = v;
    mMono.rate = v;

    v = ut::Clamp(rParam.feedback, 0.0f, 0.99f);
    mDpl2.feedback = v;
    mMono.feedback = v;

    v = ut::Clamp(rParam.outGain, 0.0f, 1.0f);
    mDpl2.outGain = v;
    mMono.outGain = v;

    mMono.busIn = NULL;
    mMono.busOut = NULL;
    mMono.sendGain = 0.0f;
    mDpl2.busIn = NULL;
    mDpl2.busOut = NULL;
    mDpl2.sendGain = 0.0f;

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

// Slot targets under their exact retail linker names (extern "C" prevents
// re-mangling, so each name resolves to the same TU's member definition;
// the dtor is defined in the factory TU at 0x801C47F8). Referenced only by
// the hand-built vtable below - never called here.
extern "C" void __dt__Q34nw4r3snd12FxChorusDpl2Fv();
extern "C" bool StartUp__Q34nw4r3snd12FxChorusDpl2Fv();
extern "C" void Shutdown__Q34nw4r3snd12FxChorusDpl2Fv();
extern "C" void UpdateBuffer__Q34nw4r3snd12FxChorusDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode();
extern "C" void OnChangeOutputMode__Q34nw4r3snd12FxChorusDpl2Fv();
extern "C" bool AssignWorkBuffer__Q34nw4r3snd12FxChorusDpl2FPvUl();
extern "C" void ReleaseWorkBuffer__Q34nw4r3snd12FxChorusDpl2Fv();

// Retail vtable data (.data @0x8056A798, 0x28); the ctor stores it at +0x0.
__declspec(section ".data") __attribute__((used))
FxChorusDpl2Vtbl lbl_eu_8056A798 = { {
    0,
    0,
    (u32)__dt__Q34nw4r3snd12FxChorusDpl2Fv,
    (u32)StartUp__Q34nw4r3snd12FxChorusDpl2Fv,
    (u32)Shutdown__Q34nw4r3snd12FxChorusDpl2Fv,
    (u32)UpdateBuffer__Q34nw4r3snd12FxChorusDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode,
    (u32)OnChangeOutputMode__Q34nw4r3snd12FxChorusDpl2Fv,
    (u32)AssignWorkBuffer__Q34nw4r3snd12FxChorusDpl2FPvUl,
    (u32)ReleaseWorkBuffer__Q34nw4r3snd12FxChorusDpl2Fv,
    0,
} };