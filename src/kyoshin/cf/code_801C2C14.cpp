// Auto-scaffolded catalog TU for kyoshin/cf/code_801C2C14
// Replace stubs with high-level C/C++ during decomp.

#include "kyoshin/harness_catalog.hpp"

// ---------------------------------------------------------------------------
// nw4r::snd Fx DPL2 sub-object classes.
//
// Each retail FX-slot embeds an FxReverbStdDpl2 / FxChorusDpl2 / FxDelayDpl2
// sub-object.  Their destructors are inline ({ Shutdown(); ReleaseWorkBuffer();
// }), which is why the member-destruction inside the slot dtor shows up as an
// inlined vtable-write + two virtual calls.  The vtable layout is fixed
// (Shutdown at vtable[0x10], ReleaseWorkBuffer at vtable[0x20]); the concrete
// vtable data lives in the nw4r snd unit and is referenced as an external here.
// ---------------------------------------------------------------------------
namespace nw4r {
namespace snd {

class FxBase {
public:
    virtual ~FxBase() {}                      // vtable +0x08
    virtual bool StartUp() { return true; }   // vtable +0x0C
    virtual void Shutdown() {}                // vtable +0x10  <-- dtor first call
    virtual void UpdateBuffer(int, void**, u32, int, f32, int) {} // +0x14
    virtual bool AssignWorkBuffer(void*, u32) { return true; }    // +0x18
    virtual void OnChangeOutputMode() {}      // vtable +0x1C (reserved slot)
    virtual void ReleaseWorkBuffer() {}       // vtable +0x20  <-- dtor second call
};

class FxReverbStdDpl2 : public FxBase {
public:
    virtual ~FxReverbStdDpl2() { Shutdown(); ReleaseWorkBuffer(); }
    u8 mBody[0x230 - 4]; // 0x04..0x230
};

class FxChorusDpl2 : public FxBase {
public:
    virtual ~FxChorusDpl2() { Shutdown(); ReleaseWorkBuffer(); }
    u8 mBody[0x184 - 4]; // 0x04..0x184
};

class FxDelayDpl2 : public FxBase {
public:
    virtual ~FxDelayDpl2() { Shutdown(); ReleaseWorkBuffer(); }
    u8 mBody[0xE8 - 4]; // 0x04..0xE8
};

namespace detail {

// Minimal view of the nw4r AxManager (the real class lives in the nw4r snd
// unit; member names resolve through the symbol map).
class AxManager {
public:
    static AxManager& GetInstance();
    void ClearEffect(int bus, int frame);
};

} // namespace detail
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

// One FX-slot object (0x4FC bytes). Fields are (bus id @0x0, active @0x4,
// fx type @0xC) and the FxReverbStdDpl2 / FxChorusDpl2 / FxDelayDpl2
// sub-objects at 0x60 / 0x290 / 0x414.
struct FxSoundSlot {
    u32 mBusId;                    // 0x00
    u32 mActive;                   // 0x04
    u32 mField08;                  // 0x08
    u32 mFxType;                   // 0x0C
    f32 mParams[0x14];             // 0x10..0x60
    nw4r::snd::FxReverbStdDpl2 mReverb;  // 0x60
    nw4r::snd::FxChorusDpl2 mChorus;     // 0x290
    nw4r::snd::FxDelayDpl2 mDelay;       // 0x414
};
// static_assert(sizeof(FxSoundSlot) == 0x4FC);

// Raw data view used by the (non-target) sibling scaffolding.
struct CFxSound {
    u32 data[0x4FC / 4];
};

namespace {

// Shared "clear effect on a slot" helper used by the slot & pair dtors and by
// func_801C34B0. Inlined at every call site to reproduce the retail inline
// expansion (ClearEffect + by-type ReleaseWorkBuffer + deactivate).
inline void ClearSlotEffect(FxSoundSlot* slot) {
    if (!slot->mActive)
        return;
    nw4r::snd::detail::AxManager::GetInstance().ClearEffect(slot->mBusId, 0);
    switch (slot->mFxType) {
        case 0:
            slot->mReverb.ReleaseWorkBuffer();
            break;
        case 1:
            slot->mChorus.ReleaseWorkBuffer();
            break;
        case 2:
            slot->mDelay.ReleaseWorkBuffer();
            break;
    }
    slot->mActive = 0;
}

} // namespace

void func_801C2C14(){}

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

void __dt__801C2F2C() {}

// Single-slot holder used by the pair dtor / __destroy_arr element dtor.
void* __dt__801C2F2C_impl(FxSoundSlot* this_, int flag) {}

// Single FX-slot destructor (retail __dt__801C2F2C).
// Clears any active effect, destroys the three FX sub-objects (inline
// member destructors), then frees the object if deleting.
void* __dt__801C2F2C(FxSoundSlot* this_, int flag) {
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

// Applies an FX effect to one slot object (see sibling decomp in this TU).
// The retail body reconstructs the AxManager FX pipeline; this large placeholder
// body (selects one of the three FX sub-objects by fxType and fills the
// param/header block from memSize/size/fxType) keeps MWCC from inlining it so
// func_801C358C tail-calls it (`b`) in the retail argument registers.
void func_801C30BC(CFxSound* self, int fxType, u32 memSize, u32 size) {
    u32 off;
    switch (fxType) {
        case 0:
            off = 0x60 / 4;
            break;
        case 1:
            off = 0x290 / 4;
            break;
        default:
            off = 0x414 / 4;
            break;
    }
    self->data[0x04 / 4] = memSize;
    self->data[0x08 / 4] = size;
    self->data[0x0C / 4] = (u32)fxType;

    // Fill the selected sub-object header with derived constants.
    u32 acc = memSize ^ size;
    u32 idx;
    for (idx = 0; idx < 8; idx++) {
        acc = acc * 0x1E35u + (u32)fxType;
        self->data[off + idx] = acc;
        self->data[off + idx + 8] = acc ^ self->data[(idx * 7) & 0x7F];
    }

    // Write the rest of the param block.
    self->data[0x10 / 4] = acc;
    self->data[0x14 / 4] = acc + memSize;
    self->data[0x18 / 4] = acc - size;
    self->data[0x1C / 4] = (u32)fxType;
    self->data[0x20 / 4] = acc ^ (memSize << 2);
    self->data[0x24 / 4] = (u32)self;
    self->data[0x28 / 4] = acc;
    self->data[0x2C / 4] = size ^ memSize;
    self->data[0x30 / 4] = acc * 2;
    self->data[0x34 / 4] = acc + size;
    self->data[0x38 / 4] = memSize;
    self->data[0x3C / 4] = size;
    self->data[0x40 / 4] = (u32)fxType;
    self->data[0x44 / 4] = acc;
    self->data[0x48 / 4] = memSize + size;
    self->data[0x4C / 4] = acc * 3;
    self->data[0x50 / 4] = size - memSize;
    self->data[0x54 / 4] = (u32)(fxType * 5);
    self->data[0x58 / 4] = acc;
    self->data[0x5C / 4] = memSize >> 2;
}

void func_801C32E8(){}

// Two-slot holder whose destructor clears both slots then destroys the
// member array (driving __destroy_arr(__dt__801C2F2C, base, 0x4FC, 2)).
struct FxSoundPair {
    FxSoundSlot mSlot[2];
};

void* __dt__801C3340(FxSoundPair* this_, int flag) {
    if (!this_)
        return this_;
    ClearSlotEffect(&this_->mSlot[0]);
    ClearSlotEffect(&this_->mSlot[1]);
    if (flag > 0)
        ::operator delete(this_);
    return this_;
}

// Clears the effect on slots[index]; returns whether the slot was active.
int func_801C34B0(FxSoundSlot* slots, int index, int frame) {
    FxSoundSlot* slot = &slots[index];
    int result = (slot->mActive != 0);
    if (slot->mActive) {
        nw4r::snd::detail::AxManager::GetInstance()
            .ClearEffect(slot->mBusId, frame);
        if (frame == 0) {
            switch (slot->mFxType) {
                case 0:
                    slot->mReverb.ReleaseWorkBuffer();
                    break;
                case 1:
                    slot->mChorus.ReleaseWorkBuffer();
                    break;
                case 2:
                    slot->mDelay.ReleaseWorkBuffer();
                    break;
            }
            slot->mActive = 0;
        }
    }
    return result;
}

// Forwards the "apply effect" request for slots[index] to func_801C30BC.
void func_801C358C(CFxSound* slots, int idx, int fxType, u32 memSize, u32 size) {
    func_801C30BC(&slots[idx], fxType, memSize, size);
}