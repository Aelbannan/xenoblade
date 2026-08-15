#include "kyoshin/cf/CfSoundMan.hpp"
#include "monolib/device/CDeviceSC.hpp"
#include "monolib/util/MemManager.hpp"

using namespace cf;

// Same-unit callees (sound-slot record ops); defined below. C linkage so
// relocs emit the unmangled retail name.
extern "C" void func_801C0A14(CfSoundRecord* rec);
extern "C" bool func_801C0A98(CfSoundRecord* rec, u32 a, u32 b, u32 c);
extern "C" bool func_801C0B90(CfSoundRecord* rec, u32 a, u32 b, u32 c);
extern "C" void func_801C1318(CfSoundRecord* rec, u32 soundId, u32 fadeFrames, float volume);
extern "C" void func_801C150C(CfSoundRecord* rec, u32 soundId, s32 stopFlag);
extern "C" void func_801C171C(CfSoundRecord* rec, u32 soundId, float pan);
extern "C" void func_801C17CC(CfSoundRecord* rec, u32 a, u32 b);
extern "C" void func_801C0948(s32 stopFlag);
extern "C" u32 func_801C0FCC(CfSoundRecord* rec, u32 a, CfSoundSlotParam* param, float f1, float f2);
extern "C" u32 func_801C10C0(CfSoundRecord* rec, s32 a, u32 b, float f1, float f2);

// Starts playback on the record's player (defined below; forward-declared so
// the func_801BFB34 dispatch can call it with the retail 4-arg ABI). C
// linkage so the call reloc binds to the retail-unmangled name.
extern "C" u32 func_801C0F5C(u32 a, s32 userParam, float f1, u32 b);
extern "C" void func_801C0C88(CfSoundRecord* rec, nw4r::snd::AuxBus bus, float volume);
extern "C" void func_801C1218(CfSoundRecord* rec, bool pause, int fade);
extern "C" void func_801C1618(CfSoundRecord* rec);
extern "C" void func_801C12A0(CfSoundRecord* rec, s32 mode);
extern "C" void func_801C13D8(CfSoundRecord* rec, s32 mode, u32 fadeFrames);
extern "C" void func_eu_801C22F0(u32 soundId, s32 stopFlag);

// Record constructor (retail func_801C09BC); defined below, forward-declared
// so the manager ctor can pass its address to __construct_array.
extern "C" CfSoundRecord* func_801C09BC(CfSoundRecord* _this);

// Record destructor (retail __dt__801BF80C); defined below, forward-declared
// so the manager ctor can pass its address to __construct_array.
extern "C" void* __dt__801BF80C(CfSoundRecord* _this, int flags);

// Constructor for the sound-manager singleton (retail __ct__801BF76C): zeroes
// the header word, constructs the two FX slots (func_801C32E8), constructs the
// 13-record array (func_801C09BC / __dt__801BF80C), publishes the pointer, and
// initializes the nw4r sound system on first use (output mode depends on the
// console's sound mode).
CfSoundManGlobal* __ct__801BF76C(CfSoundManGlobal* self) {
    self->field_0x00 = 0;
    func_801C32E8(self->mFxSlots);
    __construct_array(self->mRecords, (void*)func_801C09BC,
                      (void*)__dt__801BF80C, 0x268, 0xD);
    lbl_eu_80664430 = self;
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        lbl_eu_8066443A = 0;
        nw4r::snd::SoundSystem::InitSoundSystem(4, 3);
        nw4r::snd::OutputMode mode = nw4r::snd::OUTPUT_MODE_STEREO;
        if (CDeviceSC::isSoundModeMono()) {
            mode = nw4r::snd::OUTPUT_MODE_MONO;
        }
        nw4r::snd::detail::AxManager::GetInstance().SetOutputMode(mode);
    }
    return self;
}

// Deleting destructor for the sound-slot record (CfSoundRecord): destroys the
// embedded SoundArchivePlayer and MemorySoundArchive (non-deleting, flag -1),
// then frees the record when the delete flag is set.
void* __dt__801BF80C(CfSoundRecord* _this, int flags) {
    if (_this != NULL) {
        __dt__Q34nw4r3snd18SoundArchivePlayerFv(&_this->mArchivePlayer, -1);
        __dt__Q34nw4r3snd18MemorySoundArchiveFv(&_this->mArchive, -1);
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// Deleting destructor for the sound-manager singleton (retail
// __dt__801BF874), mirror of __ct__801BF76C: shuts down every record via
// func_801C0A14 plus the nw4r sound system, clears the global manager
// pointer, destroys the record array (__destroy_arr) and the FX-slot pair
// (__dt__801C3340), then frees the object when the delete flag is set. The
// double beq after the manager compare reuses one CR0 result (early skip +
// ?: record-base resolver), same shape as func_801BF93C.
void* __dt__801BF874(CfSoundManGlobal* self, int flags) {
    if (self != 0) {
        if (lbl_eu_80664430 != 0) {
            CfSoundRecord* rec =
                lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
            for (u32 i = 0; i < 13; i++) {
                func_801C0A14(rec + i);
            }
            nw4r::snd::SoundSystem::ShutdownSoundSystem();
        }
        lbl_eu_80664430 = 0;
        __destroy_arr(self->mRecords, (void*)__dt__801BF80C, 0x268, 0xD);
        __dt__801C3340(self->mFxSlots, -1);
        if (flags > 0) {
            operator delete(self);
        }
    }
    return self;
}

// Shuts down every sound-slot record (13 records, 0x268 stride) via
// func_801C0A14, then shuts down the nw4r sound system itself. The double
// null-check shape (one cmpwi, two beq targets) comes from the early return
// on the manager pointer plus the ?: record-base resolver reusing the same
// CR0 result.
void func_801BF93C() {
    if (lbl_eu_80664430 == 0) {
        return;
    }
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    for (u32 i = 0; i < 13; i++) {
        func_801C0A14(rec + i);
    }
    nw4r::snd::SoundSystem::ShutdownSoundSystem();
}

void func_801BF9A4(){}

void func_801BFA08(u32 idx, u32 p1, u32 p2, u32 p3) {
    CfSoundRecord* rec = (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    func_801C0A98(rec, p1, p2, p3);
    rec->field_0x1C = idx;
}

// Resolve sound slot idx (record array at manager+0x9FC, 0x268-byte stride)
// and delegate to func_801C0A14.
void func_801BFA64(u32 idx) {
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    func_801C0A14(rec + idx);
}

void func_801BFA88(u32 idx, u32 p1, u32 p2, u32 p3) {
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    func_801C0B90(rec + idx, p2, p3, p1);
}

u32 func_801BFABC(u32 idx) {
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    return rec[idx].mFlag & 1;
}

extern "C" CfSoundSlot* func_801C087C(u32 id);
extern "C" void func_801BFAE4(u32 id) { func_801C087C(id); }

// Resolve the sound slot for id, then copy the 12-byte parameter block
// into the slot record at +0x08.
void func_801BFAE8(u32 id, const CfSoundSlotParam* param) {
    CfSoundSlot* slot = func_801C087C(id);
    if (slot != 0) {
        slot->field_0x08 = param->field_0x00;
        slot->field_0x0C = param->field_0x04;
        slot->field_0x10 = param->field_0x08;
    }
}

// Sound-start dispatch gate: while the game is in a cutscene/presentation
// state (lbl_eu_80663E24 bit 9) with no blocking UI, or in the pause/event
// state (bit 11 without bit 7 of lbl_eu_80663E28), refuses the request and
// returns 0xFFFF. Otherwise resolves the record slot and starts playback via
// func_801C0F5C, returning its result.
u32 func_801BFB34(u32 a, u32 b, u32 c, float volume) {
    // While a cutscene is running (lbl_eu_80663E24 bit 22) with no blocking
    // UI, refuse the request. A non-zero gate-function result routes straight
    // to the record lookup, skipping the pause-state check below (retail
    // branch layout).
    if ((lbl_eu_80663E24 & 0x400000) != 0 &&
        func_8008585C__Q22cf13CfGameManagerFv() != 0) {
        if (func_80294624() != 0 || func_8028E440() != 0 ||
            func_802B22E0() != 0 || isInitialized__10CMenuPauseFv() != 0) {
            goto lookup;
        }
        return 0xFFFF;
    }
    if ((lbl_eu_80663E28 & 0x1000000) == 0 &&
        (lbl_eu_80663E24 & 0x100000) != 0) {
        return 0xFFFF;
    }
lookup:
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + a;
    if (rec == 0) {
        return 0xFFFF;
    }
    return func_801C0F5C((u32)rec, b, volume, c);
}

void cf::CfSoundMan::func_801BFC38(u32 r3, u32 r4, u32 r5, u32 r6, float f1) {}

// Slot-resolver wrappers (see func_801BFA64): resolve manager->mRecords[idx]
// (0x268-byte stride) with a null guard, then delegate. The trailing a/b
// params are part of the retail signature - forwarded to the callee - and
// keep r4/r5 live so the record base allocates into r6 (byte-identity key).
// The two floats pass through untouched to func_801C0FCC (retail never
// touches f1/f2 in the tail call).
u32 func_801BFDE8(u32 idx, u32 a, u32 b, float f1, float f2) {
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return 0xFFFF;
    }
    return func_801C0FCC(rec, a, (CfSoundSlotParam*)b, f1, f2);
}

u32 func_801BFE20(u32 idx, u32 a, u32 b) {
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return 0xFFFF;
    }
    // Retail calls func_801C10C0 with only the three GPR args (the float
    // params are part of the callee's ABI but the caller leaves them
    // undefined). Cast to a 3-arg pointer so the call site emits no float
    // setup, keeping the matched byte shape.
    return ((u32 (*)(CfSoundRecord*, u32, u32))func_801C10C0)(rec, a, b);
}

void func_801BFE58(s32 idx, u32 a, u32 b, float volume) {
    if (idx < 0) {
        return;
    }
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    func_801C1318(rec, a, b, volume);
}

// Slot-resolver dispatcher: validates the slot, then routes to
// func_801C12A0 (mode == -1, with a zeroed second arg) or func_801C13D8
// (mode + fadeFrames passed through). Both tail calls; fadeFrames rides in
// r5 so the record-base temp must allocate to r6 (retail byte identity).
void func_801BFE8C(s32 idx, s32 mode, u32 fadeFrames) {
    if (idx < 0) {
        return;
    }
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    if (mode == -1) {
        func_801C12A0(rec, 0);
    } else {
        func_801C13D8(rec, mode, fadeFrames);
    }
}

void func_801BFED0(s32 idx, u32 a, u32 b) {
    if (idx < 0) {
        return;
    }
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    func_801C150C(rec, a, b);
}

// Slot-resolver + stop dispatcher: validates the record slot exists, then
// forwards (soundId, stopFlag) to the 64-entry table sweeper (tail call).
void func_eu_801C17FC(u32 idx, u32 soundId, s32 stopFlag) {
    if (soundId == 0) {
        return;
    }
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    func_eu_801C22F0(soundId, stopFlag);
}

// Slot-resolver dispatcher: validates the slot, then routes to
// func_801C1218 (mode == -1, three args) or func_801C1618 (slot only).
void func_801BFF04(u32 idx, s32 mode, bool a, u32 b) {
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    if (mode == -1) {
        func_801C1218(rec, a, b);
    } else {
        func_801C1618(rec);
    }
}

// Same as func_801BFDE8 but one pass-through param (retail base in r5) and a
// pass-through float (f1 rides through untouched to func_801C171C).
void func_801BFF44(s32 idx, u32 a, float pan) {
    if (idx < 0) {
        return;
    }
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    func_801C171C(rec, a, pan);
}

void func_801BFF78(s32 idx, u32 a, u32 b) {
    if (idx < 0) {
        return;
    }
    CfSoundRecord* rec =
        (lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0) + idx;
    if (rec == 0) {
        return;
    }
    func_801C17CC(rec, a, b);
}

// Records the requested master volume in the global backing store, then
// applies it to the nw4r AxManager with the frame count scaled by the ramp
// multiplier.
void func_801BFFAC(float f1, float f2) {
    lbl_eu_80662628 = f1;
    nw4r::snd::detail::AxManager::GetInstance().SetMasterVolume(
        f1, (int)(lbl_eu_80667E8C * f2));
}

// Returns the current master volume from the nw4r AxManager. The inline
// MoveValue::GetValue interpolates between the origin and target volumes
// while the ramp is in flight (mCounter < mFrame); once finished it returns
// the target directly (retail inlines the whole ramp evaluation).
float func_801C0014__Fv() {
    nw4r::snd::detail::AxManager& ax =
        nw4r::snd::detail::AxManager::GetInstance();
    return ax.GetMasterVolume();
}

// Stops every sound slot (mode) when the sound system is initialized:
// sweeps the 13 records, then resets the whole 64-entry slot table.
void func_801C0094(s32 mode) {
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        return;
    }
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    for (u32 i = 0; i < 13; i++) {
        func_801C12A0(rec + i, mode);
    }
    func_801C0948(mode);
}

extern "C" void func_801C07F8(u32 mask, int stopFlag);
void func_801C0118(u32 mask, int stopFlag) { func_801C07F8(mask, stopFlag); }

// Pauses/resumes all 13 sound-slot records when the sound system is
// initialized, delegating each to func_801C1218 (record stride 0x268).
void func_801C011C(bool pause, int fade) {
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        return;
    }
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    for (u32 i = 0; i < 13; i++) {
        func_801C1218(rec + i, pause, fade);
    }
}

// Applies an FX effect to manager FX-slot `idx` and sweeps all 13 records
// with the new FX-send volume. The work-buffer size comes from
// func_800A9E50; the whole op is guarded on the sound system being
// initialized (checked twice, around the effect application) and the
// FX-slot array being present.
void func_801C01A8(int idx, int fxType, float volume) {
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        return;
    }
    FxSoundSlot* fxSlots = lbl_eu_80664430 ? lbl_eu_80664430->mFxSlots : 0;
    if (fxSlots == 0) {
        return;
    }
    u32 memSize = func_800A9E50(idx);
    if (func_801C358C(fxSlots, idx, fxType, memSize, 0x20000) == 0) {
        return;
    }
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        return;
    }
    CfSoundRecord* rec = lbl_eu_80664430 ? lbl_eu_80664430->mRecords : 0;
    nw4r::snd::AuxBus bus = (nw4r::snd::AuxBus)idx;
    for (u32 i = 0; i < 13; i++) {
        func_801C0C88(rec + i, bus, volume);
    }
}

// Clears the FX effect on manager FX-slot `index` with the given fade frame
// count (retail func_801C34B0), guarded on the sound system being initialized
// and the FX-slot array being present.
void func_801C028C(u32 index, u32 frame) {
    if (!IsInitializedSoundSystem__Q34nw4r3snd11SoundSystemFv()) {
        return;
    }
    FxSoundSlot* slots = lbl_eu_80664430 ? lbl_eu_80664430->mFxSlots : 0;
    if (slots == 0) {
        return;
    }
    func_801C34B0(slots, index, frame);
}

// Initializes a sound-slot record: clears the playing sound, marks the id
// invalid, and applies the default volume/gain constants.
void func_801C02F8(CfSoundSlot* slot) {
    slot->mSound = 0;
    slot->mId = 0xFFFF;
    slot->mSoundId = 0;
    slot->field_0x18 = lbl_eu_80667E98;
    slot->field_0x1C = lbl_eu_80667E9C;
    slot->field_0x2A = 0;
    slot->field_0x2C = 0;
    slot->field_0x2E = -1;
}

// Deleting destructor for the sound-slot record (retail symbol is
// address-named, so this is a plain free function rather than a member dtor).
void* __dt__801C0334(CfSoundSlot* _this, int flags) {
    if (_this != NULL && flags > 0) {
        operator delete(_this);
    }
    return _this;
}

// Deleting dtor for the 4-byte SoundHandle wrapper (retail symbol is
// address-named, so a plain free function): detach any attached sound,
// then free when requested.
void* __dt__801C0374(CfSoundHandle* _this, int flags) {
    if (_this != NULL) {
        _this->mHandle.DetachSound();
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

// Writes the sound slot's current position into out (3 words) and reports
// success. When the slot has an active voice handle (field_0x2C != 0 and a
// sound-id buffer), the position comes from the voice source's vf73 query;
// otherwise it comes from the source's sub-position object (field_0x98) or
// its vf41 position block, or finally the slot's own position fields.
// The nested `obj != 0 && obj->field_0x98 != 0` re-check reproduces the
// retail double-beq on the same compare (the second beq is dead) - same
// artifact as func_801C07F8's outer-&&-plus-inner-recheck.
extern "C" s32 func_801C03C8(CfSoundSlot* slot, CfSoundActorPos* out) {
    s32 result = 1;
    if (slot->field_0x2C != 0 && slot->mSoundId != 0) {
        slot->field_0x2A |= 4;
        CfSoundActorSrc* obj =
            (CfSoundActorSrc*)func_800B708C((int)slot->mSoundId);
        if (obj != 0) {
            CfSoundActorPos3* res =
                (CfSoundActorPos3*)obj->vf73(slot->field_0x2C);
            if (res != 0) {
                // Retail loads the three floats in reverse (z, y, x) and
                // stores them to the stack in forward (x, y, z) order.
                f32 fx = res->field_0x0C;
                f32 fy = res->field_0x1C;
                f32 fz = res->field_0x2C;
                out->field_0x00 = *(u32*)&fx;
                out->field_0x04 = *(u32*)&fy;
                out->field_0x08 = *(u32*)&fz;
                return 1;
            }
        }
        result = 0;
        goto fallback;
    }
    if (slot->mSoundId != 0) {
        slot->field_0x2A |= 2;
        CfSoundActorSrc* obj =
            (CfSoundActorSrc*)func_800B708C((int)slot->mSoundId);
        if (obj != 0) {
            if (obj != 0 && obj->field_0x98 != 0) {
                CfSoundActorSub98* sub = (CfSoundActorSub98*)obj->field_0x98;
                f32 fx = sub->field_0x2DC;
                f32 fy = sub->field_0x2E0;
                f32 fz = sub->field_0x2E4;
                out->field_0x00 = *(u32*)&fx;
                out->field_0x04 = *(u32*)&fy;
                out->field_0x08 = *(u32*)&fz;
            } else {
                CfSoundActorPos* pos = obj->getPosition();
                u32 v1 = pos->field_0x04;
                u32 v0 = pos->field_0x00;
                out->field_0x00 = v0;
                out->field_0x04 = v1;
                out->field_0x08 = pos->field_0x08;
            }
            return 1;
        }
        result = 0;
        goto fallback;
    }
    slot->field_0x2A |= 8;
fallback:
    out->field_0x00 = slot->field_0x08;
    out->field_0x04 = slot->field_0x0C;
    out->field_0x08 = slot->field_0x10;
    return result;
}

// Kept out-of-line (auto_inline off) so the retail call from func_801C08BC
// survives; the full teardown body is a separate target.
#pragma push
#pragma auto_inline off
extern "C" void func_801C055C(CfSoundSlot* p) {}
#pragma pop

#pragma push
#pragma auto_inline off
// Stops every sound slot whose flag word has any bit set in `mask`. The
// double null-check (outer && guard plus inner re-check) reuses one CR1
// compare - MWCC keeps the first compare in CR1 because the mask test
// clobbers CR0 in between (retail byte identity).
extern "C" void func_801C07F8(u32 mask, int stopFlag) {
    u16 mask16 = (u16)mask;
    for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
        if (p->mSound != 0 && (p->field_0x2A & mask16) != 0) {
            if (p->mSound != 0) {
                p->mSound->Stop(stopFlag);
            }
        }
    }
}

// Look up a sound-slot record by its 16-bit id in the 64-entry table.
// Returns the matching record or null (kept out-of-line for the tail call
// from func_801BFAE4).
CfSoundSlot* func_801C087C(u32 id) {
    if (id != 0xFFFF) {
        for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
            if (id == p->mId) {
                return p;
            }
        }
    }
    return 0;
}
#pragma pop

// Sweeps the 64-entry sound-slot table, tearing down (func_801C055C) every
// slot that is in use - either a live sound or a valid id with the in-use
// flag (bit 4 of field_0x2A) set - and resetting idle slots to the unused
// state (id 0xFFFF, cleared flags).
void func_801C08BC() {
    for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
        if (p->mSound != 0 ||
            (p->mId != 0xFFFF && (p->field_0x2A & 0x10) != 0)) {
            func_801C055C(p);
        } else {
            p->mId = 0xFFFF;
            p->field_0x2A = 0;
            p->field_0x2E = -1;
        }
    }
}

// Sweeps the 64-entry sound-slot table: stops any playing sound with the
// given fade frames, then resets each slot's id/flag state to idle.
void func_801C0948(s32 stopFlag) {
    for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
        if (p->mSound != 0) {
            p->mSound->Stop(stopFlag);
        }
        p->mId = 0xFFFF;
        p->field_0x2A = 0;
        p->field_0x2E = -1;
    }
}

#pragma push
#pragma auto_inline off
// Stops the sound whose id matches `soundId` (if any slot is playing it),
// then resets the slot's id/flag state to idle. The double null-check (outer
// && guard plus inner re-check) reuses one CR1 compare - MWCC keeps the first
// compare in CR1 because the mSoundId test clobbers CR0 in between.
void func_eu_801C22F0(u32 soundId, s32 stopFlag) {
    if (soundId == 0) {
        return;
    }
    for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
        if (p->mSound != 0 && p->mSoundId == soundId) {
            if (p->mSound != 0) {
                p->mSound->Stop(stopFlag);
            }
            p->mId = 0xFFFF;
            p->field_0x2A = 0;
            p->field_0x2E = -1;
        }
    }
}
#pragma pop

// Constructor for a sound-slot record (CfSoundRecord): clears the control
// words, marks the record id invalid, and constructs the embedded
// MemorySoundArchive and SoundArchivePlayer objects. The nw4r ctors are
// called directly (see header) - placement new would add a null check.
extern "C" CfSoundRecord* func_801C09BC(CfSoundRecord* _this) {
    _this->mFlag = 0;
    _this->field_0x10 = 0;
    _this->field_0x20 = 0;
    _this->field_0x24 = 0;
    _this->field_0x28 = -1;
    __ct__Q34nw4r3snd18MemorySoundArchiveFv(&_this->mArchive);
    __ct__Q34nw4r3snd18SoundArchivePlayerFv(&_this->mArchivePlayer);
    return _this;
}

// Empty stubs called by matched functions; auto_inline off keeps the calls
// (MWCC would otherwise inline the empty bodies and delete the call sites).
#pragma push
#pragma auto_inline off
// Shuts down an active record: stops its archive player and archive, frees
// any sound-data buffer, then resets the record's id/flag state.
extern "C" void func_801C0A14(CfSoundRecord* rec) {
    if ((rec->mFlag & 1) != 0) {
        rec->mArchivePlayer.Shutdown();
        rec->mArchive.Shutdown();
    }
    // Unsigned -1 compare: MWCC expands (u32)x != 0xFFFFFFFF into the
    // addis +1 / cmplwi 0xFFFF idiom (retail byte identity).
    if ((u32)rec->field_0x28 != 0xFFFFFFFF && rec->field_0x14 != 0) {
        mtl::MemManager::deallocate(rec->field_0x14);
        rec->field_0x14 = 0;
    }
    rec->field_0x28 = -1;
    rec->field_0x14 = 0;
    rec->field_0x18 = 0;
    rec->mFlag = 0;
}

extern "C" bool func_801C0A98(CfSoundRecord* rec, u32 data, u32 size, u32 bufSize) {
    // Shut down the embedded player/archive while the record is active.
    if ((rec->mFlag & 1) != 0) {
        rec->mArchivePlayer.Shutdown();
        rec->mArchive.Shutdown();
    }
    // Free the previous sound-data buffer once the record id is invalidated.
    if ((u32)rec->field_0x28 != 0xFFFFFFFF && rec->field_0x14 != 0) {
        mtl::MemManager::deallocate(rec->field_0x14);
        rec->field_0x14 = 0;
    }
    // Re-arm the record with the caller's buffer: store the buffer base,
    // 32-byte-aligned data size, and work-buffer size; the tail window
    // [data+aligned, bufSize) is recorded for the player's work area.
    rec->field_0x28 = -1;
    rec->field_0x14 = 0;
    rec->field_0x18 = 0;
    rec->mFlag = 0;
    rec->field_0x04 = data;
    rec->field_0x08 = (size + 0x1F) & ~0x1F;
    rec->field_0x0C = bufSize;
    rec->field_0x20 = data + ((size + 0x1F) & ~0x1F);
    rec->field_0x24 = bufSize - ((size + 0x1F) & ~0x1F);
    bool ok = rec->mArchive.Setup((const void*)data);
    rec->field_0x10 =
        (rec->mArchivePlayer.GetRequiredMemSize(&rec->mArchive) + 0x1F) & ~0x1F;
    return ok;
}

extern "C" bool func_801C0B90(CfSoundRecord* rec, u32 a, u32 b, u32 c) {
    rec->field_0x14 = (void*)a;
    rec->field_0x18 = b;
    if (a == 0 || b == 0) {
        // Re-arm from the existing buffer: the current end becomes the new
        // base, and the used size (field_0x10) is consumed from the tail.
        rec->field_0x14 = (void*)rec->field_0x20;
        rec->field_0x20 = rec->field_0x20 + rec->field_0x10;
        rec->field_0x24 = rec->field_0x24 - rec->field_0x10;
    }
    u32 i;
    bool ok = rec->mArchivePlayer.Setup(&rec->mArchive, rec->field_0x14,
                                        rec->field_0x10, NULL, 0);
    if (ok) {
        rec->mFlag |= 1;
    }
    if (c != 0 && (rec->mFlag & 1) != 0) {
        for (i = 0; i < rec->mArchive.GetPlayerCount(); i++) {
            rec->mArchivePlayer.GetSoundPlayer(i).SetPlayableSoundCount((int)c);
        }
    }
    return ok;
}
#pragma pop

#pragma push
#pragma auto_inline off
// Applies the FX-send volume `volume` to every player of an active record.
// The player count is re-queried every iteration (retail keeps the
// GetPlayerCount call in the loop test). Kept out-of-line so the retail call
// from func_801C01A8 survives.
extern "C" void func_801C0C88(CfSoundRecord* rec, nw4r::snd::AuxBus bus, float volume) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    for (u32 i = 0; i < rec->mArchive.GetPlayerCount(); i++) {
        rec->mArchivePlayer.GetSoundPlayer(i).SetFxSend(bus, volume);
    }
}
#pragma pop

// If the sound slot is active (mFlag bit 0), advance its embedded
// SoundArchivePlayer. Tail-called into nw4r Update.
void func_801C0D10(CfSoundRecord* rec) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    rec->mArchivePlayer.Update();
}

#pragma push
#pragma auto_inline off
// Binary search over the record's sound archive by user parameter: returns
// the sound id whose user param equals `target`, or -1. Guarded on the record
// being active (mFlag bit 0) and a non-sentinel target.
extern "C" int func_801C0D28(CfSoundRecord* rec, s32 target) {
    if ((rec->mFlag & 1) != 0 && target != -1) {
        // lo declared first (allocates r31) but assigned after the count call
        // so MWCC emits li r31,0 after it (retail scheduling/alloc).
        int lo;
        int hi = GetSoundCount__Q34nw4r3snd12SoundArchiveCFv(&rec->mArchive);
        lo = 0;
        while (lo <= hi) {
            int mid = (lo + hi) >> 1;
            int param = (int)rec->mArchive.GetSoundUserParam(mid);
            // Single target - param evaluation feeds all three tests (retail
            // keeps the subf result in r0 and reuses its sign for the
            // lo/hi updates).
            int diff = target - param;
            if (diff == 0) {
                return mid;
            }
            if (diff > 0) {
                lo = mid + 1;
            } else {
                hi = mid - 1;
            }
        }
    }
    return -1;
}
#pragma pop

// No-op: called when nw4r sound playback starts successfully.
// Retained as an empty hook in the retail binary (single blr). Kept
// out-of-line (auto_inline off) so the retail call from func_801C0DC4's
// failure path survives.
#pragma push
#pragma auto_inline off
void func_801C0DC0() {}
#pragma pop

// Scan the 64-entry sound-slot table for a free slot (mId == 0xFFFF).
// Kept as a static helper so MWCC inlines it (same rationale as
// findSlotById): the inlined `return p` / `return 0` reproduces the retail
// scan shape without inverting the match branch.
static inline CfSoundSlot* findFreeSlot() {
    for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
        if (p->mId == 0xFFFF) {
            return p;
        }
    }
    return 0;
}

#pragma push
#pragma auto_inline off
// Starts playback on the record's player using a sound slot: resolves the
// slot (a caller-provided one, or a free table slot scanned for by id),
// calls detail_StartSound through the player's SoundStartable, and on
// success fills in the slot record (fresh id from the counter, volume
// defaults, caller id) and applies the volume/fade. Returns the slot's
// 16-bit id, or 0xFFFF on any failure. The id != -1 test is materialized as
// a boolean early (the subfic/or/srwi idiom) because the retail keeps it in
// r5 across the free-slot scan.
extern "C" u32 func_801C0DC4(CfSoundRecord* rec, s32 id, float volume,
                             u32 fadeFrames, CfSoundSlot* slotParam) {
    u32 slotId = 0xFFFF;
    // Materialized as a 0/1 boolean early (retail subfic/addi/or/srwi idiom)
    // because the retail keeps it in r5 across the free-slot scan.
    bool idValid = (id != -1);
    if ((rec->mFlag & 1) == 0) {
        return slotId;
    }
    CfSoundSlot* slot = slotParam;
    if (slotParam == 0) {
        slot = findFreeSlot();
    }
    if (idValid && slot != 0) {
        if (rec->mArchivePlayer.StartSound((nw4r::snd::SoundHandle*)slot,
                                           (u32)id)) {
            if (slotParam != 0) {
                slotId = slotParam->mId;
            } else {
                u32 counter = lbl_eu_80664438 + 1;
                lbl_eu_80664438 = (u16)counter;
                if ((counter & 0xFFFF) == 0xFFFF) {
                    counter = (counter & 0xFFFF) + 1;
                    lbl_eu_80664438 = (u16)counter;
                }
                slotId = counter & 0xFFFF;
                slot->field_0x2A = 0;
                slot->field_0x18 = lbl_eu_80667E98;
                slot->field_0x2C = 0;
                slot->mSoundId = 0;
                slot->field_0x2E = -1;
                slot->mId = (u16)counter;
                slot->field_0x20 = (u32)rec;
                slot->field_0x24 = (u32)id;
                if (rec != 0) {
                    slot->field_0x2E = rec->field_0x1C;
                }
            }
            if (fadeFrames != 0 && slot->mSound != 0) {
                slot->mSound->SetVolume(lbl_eu_80667EA0, 0);
            }
            if (slot->mSound != 0) {
                slot->mSound->SetVolume(volume, (int)fadeFrames);
            }
        } else {
            func_801C0DC0();
        }
    }
    return slotId;
}
#pragma pop

#pragma push
#pragma auto_inline off
// Starts playback on the record's player: resolves the sound slot
// (func_801C0D28, which also consumes the retail pass-through r4 user param)
// and on failure returns 0xFFFF, otherwise forwards the request to
// func_801C0DC4 with the resolved slot id and volume and returns its result.
// The retail ABI is (r3 record, r4 search param, f1 volume, r5 forwarded
// value) - the 4th param b is the value forwarded to func_801C0DC4.
// Kept out-of-line (auto_inline off) so the retail call from func_801BFB34
// survives; C linkage binds the reloc to the retail-unmangled name.
extern "C" u32 func_801C0F5C(u32 a, s32 userParam, float f1, u32 b) {
    int result = func_801C0D28((CfSoundRecord*)a, userParam);
    if (result == -1) {
        return 0xFFFF;
    }
    return func_801C0DC4((CfSoundRecord*)a, result, f1, b, 0);
}
#pragma pop

#pragma push
#pragma auto_inline off
// Starts playback on the record's player and links the resulting sound id
// to a 64-entry table slot: resolves the sound slot (func_801C0D28) and on
// failure returns 0xFFFF; otherwise forwards the request to func_801C0DC4
// with the default volume and, if the returned id is valid (16-bit !=
// 0xFFFF), copies the caller's parameter block + gain floats into the
// matching table slot (field_0x2A in-use bit set). No null guard on the
// lookup result: retail dereferences slot unconditionally.
extern "C" u32 func_801C0FCC(CfSoundRecord* rec, u32 a, CfSoundSlotParam* param,
                             float f1, float f2) {
    int result = func_801C0D28(rec, (s32)a);
    u32 res;
    if (result == -1) {
        res = 0xFFFF;
    } else {
        res = func_801C0DC4(rec, result, lbl_eu_80667EA0, 0, 0);
    }
    if ((res & 0xFFFF) != 0xFFFF) {
        // p doubles as the slot: the found path leaves p at the match, and
        // the not-found reset (p = 0) sits on the loop-exit edge - both in
        // one register (retail r6).
        CfSoundSlot* end = lbl_eu_80575928 + 64;
        CfSoundSlot* p = lbl_eu_80575928;
        while (p != end) {
            if ((res & 0xFFFF) == p->mId) {
                goto found;
            }
            p++;
        }
        p = 0;
    found:
        p->field_0x2A |= 1;
        p->field_0x08 = param->field_0x00;
        p->field_0x0C = param->field_0x04;
        p->field_0x10 = param->field_0x08;
        p->field_0x18 = f2;
        p->field_0x1C = f1;
    }
    return res;
}

// Starts a sound on the record's player and links the resulting sound id to
// a 64-entry table slot. Resolves the sound slot by user param (func_801C0D28)
// and on failure returns 0xFFFF; otherwise forwards the request to
// func_801C0DC4 at the default volume and, if the returned id is valid
// (16-bit != 0xFFFF), finds the matching table slot and fills it from the
// voice source's position block plus the caller's gain floats. The retail
// ABI is (rec, search param, b, f1, f2) - f1/f2 are the gain floats stored at
// slot +0x1C/+0x18; func_801BFE20 calls with only the GPR args.
// Scan the 64-entry sound-slot table for a slot whose 16-bit id matches.
// Kept as a static helper so MWCC inlines it into func_801C10C0: the
// inlined `return p` / `return 0` reproduces the retail scan shape (bne out
// of the body on the match, li p, 0 on the not-found edge) - a goto-found /
// break + p = 0 form inverts the branch and loses the li.
static inline CfSoundSlot* findSlotById(u32 id) {
    for (CfSoundSlot* p = lbl_eu_80575928; p != lbl_eu_80575928 + 64; p++) {
        if (id == p->mId) {
            return p;
        }
    }
    return 0;
}

#pragma push
#pragma auto_inline off
extern "C" u32 func_801C10C0(CfSoundRecord* rec, s32 a, u32 b, float f1, float f2) {
    int result;
    u32 soundId;
    CfSoundActorSrc* src;
    src = (CfSoundActorSrc*)func_800B708C((int)b);
    if (src == 0) {
        return 0xFFFF;
    }
    result = func_801C0D28(rec, a);
    if (result == -1) {
        soundId = 0xFFFF;
    } else {
        soundId = func_801C0DC4(rec, result, lbl_eu_80667EA0, 0, 0);
    }
    u32 sid = soundId & 0xFFFF;
    if (sid != 0xFFFF) {
        // The inlined findSlotById scan leaves p as the found slot (or 0
        // when the id is absent); the found code then runs unconditionally -
        // latent not-found null deref in retail. The if-wrapped block keeps
        // the end-pointer materialization as a register addi (no addend reloc).
        CfSoundSlot* p = findSlotById(sid);
        p->field_0x2A |= 1;
        p->mSoundId = b;
        CfSoundActorPos* pos = src->getPosition();
        // Load the first two words in the retail order (4 before 0) so MWCC
        // pairs them ahead of the stores.
        u32 v1 = pos->field_0x04;
        u32 v0 = pos->field_0x00;
        p->field_0x08 = v0;
        p->field_0x0C = v1;
        p->field_0x10 = pos->field_0x08;
        p->field_0x18 = f2;
        p->field_0x1C = f1;
        func_801C055C(p);
    }
    return soundId;
}
#pragma pop

#pragma push
#pragma auto_inline off
// Pauses or resumes every player of an active record. The player count is
// re-queried every iteration (retail keeps GetPlayerCount in the loop test).
void func_801C1218(CfSoundRecord* rec, bool pause, int fade) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    for (u32 i = 0; i < rec->mArchive.GetPlayerCount(); i++) {
        rec->mArchivePlayer.GetSoundPlayer(i).PauseAllSound(pause, fade);
    }
}
#pragma pop

#pragma push
#pragma auto_inline off
// Stops every player in the record's SoundArchivePlayer with the given fade
// frames. The player count is re-queried every iteration (retail keeps the
// GetPlayerCount call in the loop test).
void func_801C12A0(CfSoundRecord* rec, s32 mode) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    for (u32 i = 0; i < rec->mArchive.GetPlayerCount(); i++) {
        rec->mArchivePlayer.GetSoundPlayer(i).StopAllSound(mode);
    }
}

// Sets the volume of every sound slot whose 16-bit id matches `soundId`
// (only while the record is active, mFlag bit 0). The volume float is kept
// in f31 across the loop because the SetVolume call clobbers f1. The
// searching flag + nested while reproduce the retail loop rotation (r0
// toggles 1->0->1 per slot).
extern "C" void func_801C1318(CfSoundRecord* rec, u32 soundId, u32 fadeFrames,
                              float volume) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    // end declared before p so MWCC allocates end=r31, p=r30 (retail); the
    // table base is loaded first and end derived from it (addi r31, r30, 0xC00).
    CfSoundSlot* end = lbl_eu_80575928 + 64;
    CfSoundSlot* p = lbl_eu_80575928;
    int searching = 1;
    while (searching && p != end) {
        searching = 0;
        while (searching == 0) {
            if (soundId == p->mId && p->mSound != 0) {
                p->mSound->SetVolume(volume, fadeFrames);
            }
            searching = 1;
        }
        p++;
    }
}

extern "C" void func_801C13D8(CfSoundRecord* rec, s32 mode, u32 fadeFrames){}
#pragma pop

#pragma push
#pragma auto_inline off
// Stops every sound slot whose 16-bit id matches `soundId` and resets the
// slot's id/flag state to idle (id 0xFFFF, cleared flags, -1 end state). The
// 0xFFFF materialization (lis 1 / subi -1) is recomputed per outer iteration
// while the lis part stays hoisted; the searching flag + nested while
// reproduce the retail loop rotation (r0 toggles 1->0->1 per slot).
extern "C" void func_801C150C(CfSoundRecord* rec, u32 soundId, s32 stopFlag) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    // searching flag + nested while reproduce the retail loop rotation (r0
    // toggles 1->0->1 per slot); the redundant if (searching) guard keeps
    // MWCC's dead second beq before the p++ (retail byte identity).
    CfSoundSlot* end = lbl_eu_80575928 + 64;
    CfSoundSlot* p = lbl_eu_80575928;
    int searching = 1;
    while (searching && p != end) {
        searching = 0;
        while (searching == 0) {
            if (soundId == p->mId) {
                if (p->mSound != 0) {
                    p->mSound->Stop(stopFlag);
                }
                p->mId = 0xFFFF;
                p->field_0x2A = 0;
                p->field_0x2E = -1;
            }
            searching = 1;
        }
        // The increment rides a second always-true while so MWCC reuses the
        // inner loop's cmpwi and keeps the dead second beq (retail shape).
        while (searching) {
            p++;
            break;
        }
    }
}

// Pause request: pause the slot's sound when its id matches the global
// pause target (or the target is the "all sounds" sentinel -1), using the
// recorded fade frames.
void func_801C15C0(CfSoundSlot* slot) {
    nw4r::snd::detail::BasicSound* sound = slot->mSound;
    CfSoundPauseParam* g = &lbl_eu_80576528;
    u32 soundId = (sound != 0) ? sound->GetId() : 0xFFFFFFFF;
    s32 target = g->field_0x00;
    if (soundId == target || target == -1) {
        // Pre-compute the Pause args so MWCC hoists the loads before the
        // sound-null check (retail scheduling: loads + bool conv then beqlr).
        bool pause = g->field_0x08 != 0;
        int fade = g->field_0x04;
        if (sound != 0) {
            sound->Pause(pause, fade);
        }
    }
}

void func_801C1618(CfSoundRecord* rec){}

// Sets the pan of every slot whose 16-bit id matches `soundId` (only while
// the record is active, mFlag bit 0). The pan float is kept in f31 across the
// loop because the SetPan call clobbers f1. The searching flag + nested
// while reproduce the retail loop rotation (r0 toggles 1->0->1 per slot).
extern "C" void func_801C171C(CfSoundRecord* rec, u32 soundId, float pan) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    // end declared before p so MWCC allocates end=r31, p=r30 (retail); the
    // table base is loaded first and end derived from it (addi r31, r30, 0xC00).
    // searching flag + nested while reproduce the retail loop rotation
    // (r0 toggles 1->0->1 per slot).
    CfSoundSlot* end = lbl_eu_80575928 + 64;
    CfSoundSlot* p = lbl_eu_80575928;
    int searching = 1;
    while (searching && p != end) {
        searching = 0;
        while (searching == 0) {
            if (soundId == p->mId && p->mSound != 0) {
                p->mSound->SetPan(pan);
            }
            searching = 1;
        }
        p++;
    }
}

#pragma push
#pragma optimize_for_size on
// ORs `b` into the flag word of the first sound slot whose 16-bit id matches
// `a`; only runs while the record is active (mFlag bit 0).
// searching flag + nested while reproduce the retail loop rotation
// (r0 toggles 1->0->1 per slot) — same shape as the SetPan sibling above.
void func_801C17CC(CfSoundRecord* rec, u32 a, u32 b) {
    if ((rec->mFlag & 1) == 0) {
        return;
    }
    CfSoundSlot* end = lbl_eu_80575928 + 64;
    CfSoundSlot* p = lbl_eu_80575928;
    int searching = 1;
    while (searching && p != end) {
        searching = 0;
        while (searching == 0) {
            if (p->mId == a) {
                p->field_0x2A |= (u16)b;
                return;
            }
            searching = 1;
            p++;
        }
    }
}
#pragma pop

// Deleting dtor for a sound slot's embedded SoundHandle (element dtor of the
// 64-entry global slot array; retail symbol address-named). The handle sits at
// offset 0 of each 0x30-byte slot, so destroying it covers the whole record.
// Retail keeps TWO null checks around the detach call (nested if, MWCC reuses
// the CR0 result of the outer compare), unlike __dt__801C0374's single check.
// Deleting dtor for a sound slot's embedded SoundHandle (element dtor of the
// 64-entry global slot array; retail symbol address-named). The handle sits at
// offset 0 of each 0x30-byte slot, so destroying it covers the whole record.
// Retail keeps TWO null checks around the detach call (nested if, MWCC reuses
// the CR0 result of the outer compare), unlike __dt__801C0374's single check.
// `self` (pre-call checks + detach arg) stays in r3, while the post-call
// delete/return use the param `_this`, which MWCC colours into r31 (retail
// this->r31 allocation; the D2-inline copy keeps the values distinct).
// Deleting dtor for a sound slot's embedded SoundHandle (element dtor of the
// 64-entry global slot array; retail symbol address-named). The handle sits at
// offset 0 of each 0x30-byte slot, so destroying it covers the whole record.
// Retail keeps TWO null checks around the detach call (nested if — the
// D2-inlined-into-D1 MWCC artifact; MWCC reuses the CR0 result of the outer
// compare), unlike __dt__801C0374's single check.
void* __dt__801C189C(CfSoundHandle* _this, int flags) {
    if (_this != NULL) {
        if (_this != NULL) {
            _this->mHandle.DetachSound();
        }
        if (flags > 0) {
            operator delete(_this);
        }
    }
    return _this;
}

void __dt__801C18F4(){}
// Static initializer for the 64-entry sound-slot table: construct the elements
// (func_801C02F8, 0x30-byte stride), then register the whole-array destructor
// (__dt__801C18F4) as a global cleanup via the link cell before the table.
void sinit_801C183C() {
    __construct_array((void*)lbl_eu_80575928, (void*)func_801C02F8,
                      (void*)__dt__801C189C, 0x30, 0x40);
    __register_global_object((void*)lbl_eu_80575928, (void*)__dt__801C18F4,
                             (void*)lbl_eu_80575918);
}
