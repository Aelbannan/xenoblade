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
// match the stripped retail object (MWCC_CASES §1b float pools).
extern "C" const f32 lbl_eu_80669F98; // preDelayTime / preDelayTimeMax default
extern "C" const f32 lbl_eu_80669F9C; // fusedTime default
extern "C" const f32 lbl_eu_80669FA0; // coloration default
extern "C" const f32 lbl_eu_80669FA4; // damping default
extern "C" const f32 lbl_eu_80669FA8; // outGain / fusedGain default, upper clamp
extern "C" const f32 lbl_eu_80669FAC; // sendGain, lower clamp bound

// Retail vtable (shared data split .data 0x8056A7E8). The class is defined as
// a plain struct so MWCC emits no local __vt__/RTTI and no weak vtable-support
// stubs or base-class dtors (retail snd_FxReverbStdDpl2.o is text-only and its
// split budget is exact: every emitted helper would overflow it).
extern "C" u8 lbl_eu_8056A7E8[];

// ---------------------------------------------------------------------------
// Plain-layout view of the FxReverbStdDpl2 object (retail offsets):
//   0x00 vptr (set by the ctor from lbl_eu_8056A7E8)
//   0x04 ut::LinkListNode (prev/next, zeroed by the ctor)
//   0x0C mIsActive, 0x10 mOutputMode, 0x14 mHeap, 0x18 mAllocCount
//   0x1C detail::FxReverbStdParam mParam (0x28 bytes)
//   0x44 AXFX_REVERBSTD_EXP mMono (0xE8 bytes)
//   0x12C AXFX_REVERBSTD_EXP_DPL2 mDpl2 (0x104 bytes)
struct FxReverbStdDpl2 {
    void* vtbl;                       // 0x0
    void* linkPrev;                   // 0x4
    void* linkNext;                   // 0x8
    bool mIsActive;                   // 0xC
    s32 mOutputMode;                  // 0x10
    MEMiHeapHead* mHeap;              // 0x14
    u32 mAllocCount;                  // 0x18
    detail::FxReverbStdParam mParam;  // 0x1C (0x28 bytes -> 0x44)
    AXFX_REVERBSTD_EXP mMono;         // 0x44 (0xE8 bytes)
    AXFX_REVERBSTD_EXP_DPL2 mDpl2;    // 0x12C (0x104 bytes)

    detail::AxfxImpl* GetImpl() {
        return reinterpret_cast<detail::AxfxImpl*>(&mHeap);
    }
};

// ---------------------------------------------------------------------------
// All methods defined under their exact retail mangled names as extern "C"
// free functions (no class members => no vtable/key-function emission).
extern "C" bool SetParam__Q34nw4r3snd15FxReverbStdDpl2FRCQ44nw4r3snd6detail16FxReverbStdParam(FxReverbStdDpl2* self, const detail::FxReverbStdParam& rParam);
extern "C" void Shutdown__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self);
extern "C" bool StartUp__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self);

extern "C" FxReverbStdDpl2* __ct__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self) {
    self->linkPrev = nullptr;
    self->linkNext = nullptr;
    self->vtbl = (void*)lbl_eu_8056A7E8;
    self->mIsActive = false;
    self->mHeap = nullptr; // 0x14
    self->mAllocCount = 0; // 0x18

    self->mParam.preDelayTime = lbl_eu_80669F98;
    self->mParam.fusedTime = lbl_eu_80669F9C;
    self->mParam.coloration = lbl_eu_80669FA0;
    self->mParam.damping = lbl_eu_80669FA4;
    self->mParam.outGain = lbl_eu_80669FA8;
    self->mParam.iirType = 5;
    self->mParam.preDelayTimeMax = lbl_eu_80669F98;
    self->mParam.fusedMode = 0;
    self->mParam.earlyGain = lbl_eu_80669FAC;
    self->mParam.fusedGain = lbl_eu_80669FA8;

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
    SetParam__Q34nw4r3snd15FxReverbStdDpl2FRCQ44nw4r3snd6detail16FxReverbStdParam(self, param);
    return self;
}

extern "C" u32 GetRequiredMemSize__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self) {
    u32 memSize = ut::RoundUp(AXFXReverbStdExpGetMemSize(&self->mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&self->mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    if (required < memSize) {
        required = memSize;
    }
    return required;
}

extern "C" bool AssignWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2FPvUl(FxReverbStdDpl2* self, void* pBuffer, u32 size) {
    return self->GetImpl()->CreateHeap(pBuffer, size);
}

extern "C" void ReleaseWorkBuffer__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self) {
    self->GetImpl()->DestroyHeap();
}

extern "C" bool StartUp__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self) {
    u32 memSize = ut::RoundUp(AXFXReverbStdExpGetMemSize(&self->mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&self->mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 total = required < memSize ? memSize : required;
    if (total > self->GetImpl()->GetHeapTotalSize()) {
        return false;
    }

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;

    BOOL success;
    if (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2) {
        self->mOutputMode = 1;
        self->GetImpl()->HookAlloc(&allocHook, &freeHook);

        success = AXFXReverbStdExpInitDpl2(&self->mDpl2);
        self->GetImpl()->RestoreAlloc(allocHook, freeHook);

        (void)AXFXReverbStdExpGetMemSizeDpl2(&self->mDpl2); // debug leftover
    } else {
        self->mOutputMode = 0;
        self->GetImpl()->HookAlloc(&allocHook, &freeHook);

        success = AXFXReverbStdExpInit(&self->mMono);
        self->GetImpl()->RestoreAlloc(allocHook, freeHook);

        (void)AXFXReverbStdExpGetMemSize(&self->mMono); // debug leftover
    }

    self->mIsActive = true;

    return success;
}

extern "C" void Shutdown__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self) {
    if (!self->mIsActive) {
        return;
    }

    self->mIsActive = false;

    AXFXAllocHook allocHook;
    AXFXFreeHook freeHook;
    self->GetImpl()->HookAlloc(&allocHook, &freeHook);

    if (self->mOutputMode == 1) {
        AXFXReverbStdExpShutdownDpl2(&self->mDpl2);
    } else {
        AXFXReverbStdExpShutdown(&self->mMono);
    }

    self->GetImpl()->RestoreAlloc(allocHook, freeHook);
}

extern "C" bool SetParam__Q34nw4r3snd15FxReverbStdDpl2FRCQ44nw4r3snd6detail16FxReverbStdParam(FxReverbStdDpl2* self, const detail::FxReverbStdParam& rParam) {
    self->mParam = rParam;

    f32 preDelayTimeMax =
        ut::Max(rParam.preDelayTimeMax, lbl_eu_80669FAC);
    bool changed = (preDelayTimeMax != self->mMono.preDelayTimeMax);

    self->mDpl2.earlyMode = rParam.iirType;
    self->mMono.earlyMode = rParam.iirType;
    self->mDpl2.preDelayTimeMax = preDelayTimeMax;
    self->mMono.preDelayTimeMax = preDelayTimeMax;

    f32 preDelayTime =
        ut::Clamp(rParam.preDelayTime, lbl_eu_80669FAC, rParam.preDelayTimeMax);
    self->mDpl2.preDelayTime = preDelayTime;
    self->mMono.preDelayTime = preDelayTime;

    self->mDpl2.fusedMode = rParam.fusedMode;
    self->mMono.fusedMode = rParam.fusedMode;

    // Retail clamps fusedTime only on the lower side (ut::Max), unlike the
    // other gain/time fields below which use a full upper/lower Clamp.
    f32 fusedTime = ut::Max(rParam.fusedTime, lbl_eu_80669FAC);
    self->mDpl2.fusedTime = fusedTime;
    self->mMono.fusedTime = fusedTime;

    f32 coloration =
        ut::Clamp(rParam.coloration, lbl_eu_80669FAC, lbl_eu_80669FA8);
    self->mDpl2.coloration = coloration;
    self->mMono.coloration = coloration;

    f32 damping = ut::Clamp(rParam.damping, lbl_eu_80669FAC, lbl_eu_80669FA8);
    self->mDpl2.damping = damping;
    self->mMono.damping = damping;

    f32 earlyGain =
        ut::Clamp(rParam.earlyGain, lbl_eu_80669FAC, lbl_eu_80669FA8);
    self->mDpl2.earlyGain = earlyGain;
    self->mMono.earlyGain = earlyGain;

    f32 fusedGain =
        ut::Clamp(rParam.fusedGain, lbl_eu_80669FAC, lbl_eu_80669FA8);
    self->mDpl2.fusedGain = fusedGain;
    self->mMono.fusedGain = fusedGain;

    f32 outGain = ut::Clamp(rParam.outGain, lbl_eu_80669FAC, lbl_eu_80669FA8);
    self->mDpl2.outGain = outGain;
    self->mMono.outGain = outGain;

    self->mMono.busIn = NULL;
    self->mMono.busOut = NULL;
    self->mMono.sendGain = lbl_eu_80669FAC;
    self->mDpl2.busIn = NULL;
    self->mDpl2.busOut = NULL;
    self->mDpl2.sendGain = lbl_eu_80669FAC;

    if (!self->mIsActive) {
        return true;
    }

    u32 memSize = ut::RoundUp(AXFXReverbStdExpGetMemSize(&self->mMono) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    u32 required = ut::RoundUp(AXFXReverbStdExpGetMemSizeDpl2(&self->mDpl2) +
                                  detail::AxfxImpl::HEAP_SIZE_MIN,
                              32);
    if (required < memSize) {
        required = memSize;
    }

    // Available work-buffer space = end offset of the heap.
    MEMiHeapHead* heap = self->mHeap;
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
        self->GetImpl()->HookAlloc(&allocHook, &freeHook);

        if (self->mOutputMode == 1) {
            success = AXFXReverbStdExpSettingsDpl2(&self->mDpl2);
        } else {
            success = AXFXReverbStdExpSettings(&self->mMono);
        }

        self->GetImpl()->RestoreAlloc(allocHook, freeHook);
    } else if (self->mOutputMode == 1) {
        success = AXFXReverbStdExpSettingsUpdateDpl2(&self->mDpl2);
    } else {
        success = AXFXReverbStdExpSettingsUpdate(&self->mMono);
    }

    return success != 0;
}

extern "C" void UpdateBuffer__Q34nw4r3snd15FxReverbStdDpl2FiPPvUlQ34nw4r3snd12SampleFormatfQ34nw4r3snd10OutputMode(FxReverbStdDpl2* self, int channels, void** ppBuffer, u32 size,
                                   SampleFormat format, f32 sampleRate,
                                   OutputMode mode) {
#pragma unused(channels)
#pragma unused(size)
#pragma unused(format)
#pragma unused(sampleRate)

    if (!self->mIsActive) {
        return;
    }

    if ((mode == OUTPUT_MODE_DPL2) != self->mOutputMode) {
        return;
    }

    if (self->mOutputMode == 1) {
        AXFX_BUFFERUPDATE_DPL2 buffer;
        buffer.left = (s32*)ppBuffer[0];
        buffer.right = (s32*)ppBuffer[1];
        buffer.left_surround = (s32*)ppBuffer[2];
        buffer.right_surround = (s32*)ppBuffer[3];
        AXFXReverbStdExpCallbackDpl2(&buffer, &self->mDpl2);
    } else {
        AXFX_BUFFERUPDATE buffer;
        buffer.left = (s32*)ppBuffer[0];
        buffer.right = (s32*)ppBuffer[1];
        buffer.surround = (s32*)ppBuffer[2];
        AXFXReverbStdExpCallback(&buffer, &self->mMono);
    }
}

// Fake SI interface for the virtual dispatch in OnChangeOutputMode: retail
// dispatches Shutdown/StartUp through the object's vtable (slots 0x10/0xC),
// which a plain-struct call cannot express.
struct FxReverbStdDpl2If {
    virtual void v0();
    virtual void v1();
    virtual void v2();
    virtual bool v3();
    virtual void v4();
};

extern "C" void OnChangeOutputMode__Q34nw4r3snd15FxReverbStdDpl2Fv(FxReverbStdDpl2* self) {
    bool dpl2 =
        (detail::AxManager::GetInstance().GetOutputMode() == OUTPUT_MODE_DPL2);
    if (self->mOutputMode == dpl2) {
        return;
    }

    reinterpret_cast<FxReverbStdDpl2If*>(self)->v2();  // slot 0x10: Shutdown
    reinterpret_cast<FxReverbStdDpl2If*>(self)->v1();  // slot 0xC: StartUp
}

} // namespace snd
} // namespace nw4r

// Absorb: .data 0x28 via pad_data_section (retail zeros)
extern "C" void* __ct__Q34nw4r3snd15FxReverbStdDpl2Fv;
__declspec(section ".data") __attribute__((aligned(8), used)) const void* snd_FxReverbStdDpl2_data_anchor[4] = { (const void*)&__ct__Q34nw4r3snd15FxReverbStdDpl2Fv, nullptr, nullptr, nullptr };