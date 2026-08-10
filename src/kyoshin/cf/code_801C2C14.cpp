// Auto-scaffolded catalog TU for kyoshin/cf/code_801C2C14
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

#include <new>

#include <nw4r/db/db_assert.h>

// ---------------------------------------------------------------------------
// nw4r::snd Fx DPL2 sub-object classes.
//
// Each retail FX-slot embeds an FxReverbStdDpl2 / FxChorusDpl2 / FxDelayDpl2
// sub-object.  Their destructors are inline ({ Shutdown(); ReleaseWorkBuffer();
// }), which is why the member-destruction inside the slot dtor shows up as an
// inlined vtable-write + two virtual calls.  The vtable layout is fixed
// (Shutdown at vtable[0x10], ReleaseWorkBuffer at vtable[0x20]); the concrete
// vtable data lives in the nw4r snd unit and is referenced as an external here
// (retail lbl_eu_8056A7E8 / 8056A798 / 8056A7C0 via the reloc map).
// ---------------------------------------------------------------------------

// MWCC ABI array-construction/destruction helpers.  C linkage keeps the
// call reloc names unmangled (same pattern as CMenuQstCnt.hpp).
extern "C" void __construct_array(void*, void*, void*, int, int);
extern "C" void __destroy_arr(void*, void*, int, int);

namespace nw4r {
namespace snd {

enum AuxBus { AUX_A, AUX_B, AUX_C, AUX_BUS_NUM };

class FxBase {
public:
    virtual ~FxBase() {}                   // vtable +0x08
    virtual bool StartUp() { return true; }   // vtable +0x0C
    virtual void Shutdown() {}             // vtable +0x10  <-- dtor first call
    virtual void UpdateBuffer(int, void**, u32, int, f32, int) {} // vtable +0x14
    virtual bool AssignWorkBuffer(void*, u32) { return true; }    // vtable +0x18
    // Retail slot +0x1C is invoked with (memSize, size) when an effect is
    // applied to a slot (func_801C30BC).  Exact upstream name unknown.
    virtual void StartUp(u32, u32) {}      // vtable +0x1C
    virtual void ReleaseWorkBuffer() {}    // vtable +0x20  <-- dtor second call
};

namespace detail {

// Parameter blobs consumed by the DPL2 SetParam calls.  Sizes must match the
// nw4r snd detail structs (FxReverbStdParam is 0x28 bytes, the chorus/delay
// params 0x14 each) so they overlay the slot parameter block at 0x10.
struct FxReverbStdParam {
    f32 preDelayTime;     // 0x00
    f32 fusedTime;        // 0x04
    f32 coloration;       // 0x08
    f32 damping;          // 0x0C
    f32 outGain;          // 0x10
    u32 iirType;          // 0x14
    f32 preDelayTimeMax;  // 0x18
    u32 fusedMode;        // 0x1C
    f32 earlyGain;        // 0x20
    f32 fusedGain;        // 0x24
};

struct FxChorusParam {
    f32 delayTime;        // 0x00
    f32 depth;            // 0x04
    f32 rate;             // 0x08
    f32 feedback;         // 0x0C
    f32 outGain;          // 0x10
};

struct FxDelayParam {
    f32 delay;            // 0x00
    f32 feedback;         // 0x04
    f32 outGain;          // 0x08
    f32 delayTimeMax;     // 0x0C
    f32 iir;              // 0x10
};

// Minimal view of the nw4r AxManager (the real class lives in the nw4r snd
// unit; member names resolve through the symbol map).  The per-bus fade
// volume array at 0x54 (16-byte MoveValue entries) is read directly by the
// effect-apply code to refuse starting an effect while a bus fade is running.
class AxManager {
public:
    struct FadeVolume {
        f32 mOrigin;   // 0x00
        f32 mTarget;   // 0x04
        int mFrame;    // 0x08
        int mCounter;  // 0x0C

        bool IsFinished() const { return mCounter >= mFrame; }
    };

    static AxManager& GetInstance();
    void ClearEffect(AuxBus bus, int frame);
    bool AppendEffect(AuxBus bus, FxBase* pFx);

private:
    int mOutputMode;         // 0x00
    u8* mZeroBufferAddress;  // 0x04
    u8 mPad08[0x10];         // 0x08..0x17
    u8 mPad18[0x38];         // 0x18..0x4F
    s32 mResetReadyCounter;  // 0x50

public:
    FadeVolume mAuxFadeVolume[3]; // 0x54
};

} // namespace detail

class FxReverbStdDpl2 : public FxBase {
public:
    FxReverbStdDpl2();                     // import: __ct__Q34nw4r3snd15FxReverbStdDpl2Fv
    virtual ~FxReverbStdDpl2() { Shutdown(); ReleaseWorkBuffer(); }
    u32 GetRequiredMemSize();
    bool SetParam(const detail::FxReverbStdParam& rParam);
    u8 mBody[0x230 - 4]; // 0x04..0x230
};

class FxChorusDpl2 : public FxBase {
public:
    FxChorusDpl2();
    virtual ~FxChorusDpl2() { Shutdown(); ReleaseWorkBuffer(); }
    u32 GetRequiredMemSize();
    bool SetParam(const detail::FxChorusParam& rParam);
    u8 mBody[0x184 - 4]; // 0x04..0x184
};

class FxDelayDpl2 : public FxBase {
public:
    FxDelayDpl2();
    virtual ~FxDelayDpl2() { Shutdown(); ReleaseWorkBuffer(); }
    u32 GetRequiredMemSize();
    bool SetParam(const detail::FxDelayParam& rParam);
    u8 mBody[0xE8 - 4]; // 0x04..0xE8
};

} // namespace snd
} // namespace nw4r

// SDA2 default-FX parameter constants (this TU's .sdata2 pool). Referenced by
// name so the SDA21 relocations match the stripped retail object.
extern const f32 lbl_eu_80667EC0;
extern const f32 lbl_eu_80667EC4;
extern const f32 lbl_eu_80667EC8;
extern const f32 lbl_eu_80667ECC;
extern const f32 lbl_eu_80667ED0;
extern const f32 lbl_eu_80667ED4;
extern const f32 lbl_eu_80667ED8;
extern const f32 lbl_eu_80667EDC;
extern const f32 lbl_eu_80667EE0;
extern const f32 lbl_eu_80667EE4;
extern const f32 lbl_eu_80667EE8;
extern const f32 lbl_eu_80667EEC;
extern const f32 lbl_eu_80667EF0;
extern const f32 lbl_eu_80667EF4;
extern const f32 lbl_eu_80667EF8;
extern const f32 lbl_eu_80667EFC;
extern const f32 lbl_eu_80667F00;
extern const f32 lbl_eu_80667F04;

// Panic() arguments (retail .data string literals, external to this TU).
extern const char lbl_eu_80533D48[];
extern const char lbl_eu_80533D80[];

// One FX-slot object (0x4FC bytes). Fields are (bus id @0x0, active @0x4,
// fx type @0xC) and the FxReverbStdDpl2 / FxChorusDpl2 / FxDelayDpl2
// sub-objects at 0x60 / 0x290 / 0x414.
struct FxSoundSlot {
    u32 mBusId;      // 0x00
    u32 mActive;     // 0x04
    u32 mField08;    // 0x08
    u32 mFxType;     // 0x0C
    // Per-type parameter block (0x10..0x60): the three nw4r param structs sit
    // back-to-back; SetParam is called with the one matching the FX type.
    nw4r::snd::detail::FxReverbStdParam mReverbParam; // 0x10 (0x28 bytes)
    nw4r::snd::detail::FxChorusParam mChorusParam;    // 0x38 (0x14 bytes)
    nw4r::snd::detail::FxDelayParam mDelayParam;      // 0x4C (0x14 bytes)
    nw4r::snd::FxReverbStdDpl2 mReverb;  // 0x60
    nw4r::snd::FxChorusDpl2 mChorus;     // 0x290
    nw4r::snd::FxDelayDpl2 mDelay;       // 0x414
};
// static_assert(sizeof(FxSoundSlot) == 0x4FC);

namespace {

// Shared "clear effect on a slot" helper used by the slot & pair dtors and by
// func_801C34B0. Inlined at every call site to reproduce the retail inline
// expansion (ClearEffect + by-type ReleaseWorkBuffer + deactivate).
inline void ClearSlotEffect(FxSoundSlot* slot) {
    if (!slot->mActive)
        return;
    nw4r::snd::detail::AxManager::GetInstance()
        .ClearEffect((nw4r::snd::AuxBus)slot->mBusId, 0);
    if (slot->mFxType == 0)
        slot->mReverb.ReleaseWorkBuffer();
    else if (slot->mFxType == 1)
        slot->mChorus.ReleaseWorkBuffer();
    else if (slot->mFxType == 2)
        slot->mDelay.ReleaseWorkBuffer();
    slot->mActive = 0;
}

} // namespace

// Element constructor (retail func_801C2C14).  Writes the default param block,
// constructs the three FX sub-objects (out-of-line nw4r ctors), clears the
// active/type header, then overwrites the params with the final values.
// C-linkage so the __construct_array address relocs match retail.
extern "C" void func_801C2C14(FxSoundSlot* self) {
    // Wave 1: default params (retail store order, ascending).
    self->mReverbParam.preDelayTime = lbl_eu_80667EC0;
    self->mReverbParam.fusedTime = lbl_eu_80667EC4;
    self->mReverbParam.coloration = lbl_eu_80667EC8;
    self->mReverbParam.damping = lbl_eu_80667ECC;
    self->mReverbParam.outGain = lbl_eu_80667ED0;
    self->mReverbParam.iirType = 5;
    self->mReverbParam.preDelayTimeMax = lbl_eu_80667EC0;
    self->mReverbParam.fusedMode = 0;
    self->mReverbParam.earlyGain = lbl_eu_80667ED4;
    self->mReverbParam.fusedGain = lbl_eu_80667ED0;
    self->mChorusParam.delayTime = lbl_eu_80667ED8;
    self->mChorusParam.depth = lbl_eu_80667EDC;
    self->mChorusParam.rate = lbl_eu_80667ED0;
    self->mChorusParam.feedback = lbl_eu_80667ED4;
    self->mChorusParam.outGain = lbl_eu_80667ED0;
    self->mDelayParam.delay = lbl_eu_80667EE0;
    self->mDelayParam.feedback = lbl_eu_80667ECC;
    self->mDelayParam.outGain = lbl_eu_80667ED0;
    self->mDelayParam.delayTimeMax = lbl_eu_80667EE0;
    self->mDelayParam.iir = lbl_eu_80667ED0;

    // Construct the three FX sub-objects (retail __ct__Q34nw4r3snd...Fv).
    new (&self->mReverb) nw4r::snd::FxReverbStdDpl2();
    new (&self->mChorus) nw4r::snd::FxChorusDpl2();
    new (&self->mDelay) nw4r::snd::FxDelayDpl2();

    // Clear the active/type header (retail memset of the 0x04..0x10 block).
    memset(&self->mActive, 0, 0xC);

    // Wave 2: final param values (retail store order).
    self->mReverbParam.iirType = 5;
    self->mReverbParam.preDelayTimeMax = lbl_eu_80667EE4;
    self->mReverbParam.preDelayTime = lbl_eu_80667EC0;
    self->mReverbParam.fusedMode = 3;
    self->mReverbParam.fusedTime = lbl_eu_80667EE8;
    self->mReverbParam.coloration = lbl_eu_80667EC8;
    self->mReverbParam.damping = lbl_eu_80667EC8;
    self->mReverbParam.earlyGain = lbl_eu_80667EEC;
    self->mReverbParam.fusedGain = lbl_eu_80667EF0;
    self->mReverbParam.outGain = lbl_eu_80667EF4;
    self->mChorusParam.delayTime = lbl_eu_80667EF8;
    self->mChorusParam.depth = lbl_eu_80667EDC;
    self->mChorusParam.rate = lbl_eu_80667ED0;
    self->mChorusParam.feedback = lbl_eu_80667ED4;
    self->mChorusParam.outGain = lbl_eu_80667ED0;
    self->mDelayParam.delayTimeMax = lbl_eu_80667EFC;
    self->mDelayParam.delay = lbl_eu_80667F00;
    self->mDelayParam.feedback = lbl_eu_80667EC8;
    self->mDelayParam.outGain = lbl_eu_80667F04;
    self->mDelayParam.iir = lbl_eu_80667ED0;
}

void __dt__Q34nw4r3snd15FxReverbStdDpl2Fv(){}

// Base-class dtor: only releases the memory (no vtable write). Array/derived
// destructors of this TU dispatch here as DESTROYBASE.
void* __dt__801C2DF4(void* this_, int deletingFlag) {
    if (this_ && deletingFlag > 0)
        ::operator delete(this_);
    return this_;
}

void __dt__Q34nw4r3snd12FxChorusDpl2Fv(){}

void __dt__Q34nw4r3snd11FxDelayDpl2Fv(){}

// Single FX-slot destructor (retail __dt__801C2F2C).
// Clears any active effect, destroys the three FX sub-objects (inline member
// destructors), then frees the object if deleting.  C-linkage so the
// __construct_array / __destroy_arr address relocs match retail.
extern "C" void* __dt__801C2F2C(FxSoundSlot* this_, int flag) {
    if (!this_)
        return this_;
    ClearSlotEffect(this_);
    this_->mDelay.~FxDelayDpl2();
    this_->mChorus.~FxChorusDpl2();
    this_->mReverb.~FxReverbStdDpl2();
    if (flag > 0)
        ::operator delete(this_);
    return this_;
}

// Applies an FX effect to one slot object (retail func_801C30BC).
//
// 1. If the slot already has an active effect, clears it (ClearEffect plus the
//    matching DPL2 sub-object's ReleaseWorkBuffer) and deactivates the slot.
// 2. Refuses to run while the AxManager's per-bus fade volume is mid-fade
//    (IsFinished() false) - the retail returns 0 in that case.
// 3. Pushes the stored parameter block into the chosen DPL2 sub-object, then
//    starts it with (memSize, size) and appends it to the bus effect chain.
bool func_801C30BC(FxSoundSlot* self, u32 fxType, u32 memSize, u32 size) {
    // The retail builds a {&mReverb, &mChorus, &mDelay} address array on the
    // stack and indexes it by fxType (lwzx); kept as an explicit local array
    // to reproduce that shape.  `pFx` is the pointer handed to AppendEffect.
    nw4r::snd::FxBase* pFxList[3] = { &self->mReverb, &self->mChorus, &self->mDelay };
    nw4r::snd::FxBase* pFx = pFxList[fxType];

    if (self->mActive) {
        nw4r::snd::detail::AxManager::GetInstance()
            .ClearEffect((nw4r::snd::AuxBus)self->mBusId, 0);
        if (self->mFxType == 0) {
            self->mReverb.ReleaseWorkBuffer();
        } else if (self->mFxType == 1) {
            self->mChorus.ReleaseWorkBuffer();
        } else if (self->mFxType == 2) {
            self->mDelay.ReleaseWorkBuffer();
        }
        self->mActive = 0;
    }

    u32 busId = self->mBusId;
    nw4r::snd::detail::AxManager& mgr = nw4r::snd::detail::AxManager::GetInstance();
    int busOk = busId <= 2;
    if (!busOk) {
        nw4r::db::Panic(lbl_eu_80533D80, 0x6e, lbl_eu_80533D48, busId, 0, 3);
    }
    if (!mgr.mAuxFadeVolume[busId].IsFinished()) {
        return 0;
    }

    if (fxType == 0) {
        self->mReverb.SetParam(self->mReverbParam);
        self->mReverb.GetRequiredMemSize();
        self->mActive = memSize;
        self->mField08 = size;
        self->mFxType = fxType;
        self->mReverb.StartUp(memSize, size);
    } else if (fxType == 1) {
        self->mChorus.SetParam(self->mChorusParam);
        self->mChorus.GetRequiredMemSize();
        self->mActive = memSize;
        self->mField08 = size;
        self->mFxType = fxType;
        self->mChorus.StartUp(memSize, size);
    } else if (fxType == 2) {
        self->mDelay.SetParam(self->mDelayParam);
        self->mDelay.GetRequiredMemSize();
        self->mActive = memSize;
        self->mField08 = size;
        self->mFxType = fxType;
        self->mDelay.StartUp(memSize, size);
    }

    return nw4r::snd::detail::AxManager::GetInstance()
        .AppendEffect((nw4r::snd::AuxBus)self->mBusId, pFx);
}

// Two-slot holder whose destructor clears both slots then destroys the
// member array (driving __destroy_arr(__dt__801C2F2C, base, 0x4FC, 2)).
struct FxSoundPair {
    FxSoundSlot mSlot[2];
};

// Two-slot holder constructor (retail func_801C32E8): constructs the slot
// array via __construct_array(func_801C2C14, __dt__801C2F2C, 0x4FC, 2), then
// assigns the two bus ids (0 and 1).  Returns this (retail mr r3, r31).
FxSoundPair* func_801C32E8(FxSoundPair* self) {
    __construct_array(self, (void*)func_801C2C14, (void*)__dt__801C2F2C, 0x4FC, 2);
    self->mSlot[0].mBusId = 0;
    self->mSlot[1].mBusId = 1;
    return self;
}

// Two-slot holder destructor (retail __dt__801C3340): clears both slots'
// active effects, destroys the member array via __destroy_arr, then frees the
// object if deleting.
void* __dt__801C3340(FxSoundPair* this_, int flag) {
    if (!this_)
        return this_;
    ClearSlotEffect(&this_->mSlot[0]);
    ClearSlotEffect(&this_->mSlot[1]);
    __destroy_arr(this_, (void*)__dt__801C2F2C, 0x4FC, 2);
    if (flag > 0)
        ::operator delete(this_);
    return this_;
}

// Clears the effect on slots[index]; returns whether the slot was active.
int func_801C34B0(FxSoundSlot* slots, int index, int frame) {
    // result declared before slot so MWCC keeps it in r31 (retail rlwinm).
    int result = (slots[index].mActive != 0);
    FxSoundSlot* slot = &slots[index];
    if (slot->mActive) {
        nw4r::snd::detail::AxManager::GetInstance()
            .ClearEffect((nw4r::snd::AuxBus)slot->mBusId, frame);
        if (frame == 0) {
            if (slot->mFxType == 0)
                slot->mReverb.ReleaseWorkBuffer();
            else if (slot->mFxType == 1)
                slot->mChorus.ReleaseWorkBuffer();
            else if (slot->mFxType == 2)
                slot->mDelay.ReleaseWorkBuffer();
            slot->mActive = 0;
        }
    }
    return result;
}

// Forwards the "apply effect" request for slots[index] to func_801C30BC.
void func_801C358C(FxSoundSlot* slots, int idx, int fxType, u32 memSize, u32 size) {
    func_801C30BC(&slots[idx], fxType, memSize, size);
}
