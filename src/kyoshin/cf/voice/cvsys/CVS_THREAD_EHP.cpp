// CVS_THREAD_EHP: Voice thread for EHP (Emergency HP recovery) sequences.
// Five matched functions: completion callback, voice removal, slot-rotation
// advance, slot-2 play, and the standalone EHP voice selector.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_EHP.hpp"
#include "monolib/math/Random.hpp"

// Init-data tables (3 u32s each: {0, -1, callback}). Each slot state carries
// a {field_0, field_4, callback} triple that the rotation function copies into
// unk0/unk4/unk8 when the rotating index is not at the stop index.
extern "C" u32 lbl_eu_80539B14[3]; // {0, -1, func_802A6408}
extern "C" u32 lbl_eu_80539B20[3]; // {0, -1, func_802A6718}

// Virtual method override: returns the buffer size for this thread type.
// Matches CVS_THREAD::blank1 slot in vtable; EHP subclass returns 0xB4 (180).
int func_802A6818() {
    return 0xB4;
}

// ── Target 1: us-802a8e4c (func_802A6718) ──────────────────────────────────
// Completion callback: if no active voice is playing, invoke the
// playback-start virtual (CVS_THREAD::func_802A3B50, vtable slot 0).
void func_802A6718(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a8e94 (func_802A6760) ──────────────────────────────────
// Remove a voice from all slots by matching its embedded CCharVoice pointer.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void func_802A6760(CVS_THREAD_EHP* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    // Slot 0x20: bias to embedded CCharVoice at +0x3E9C via &handle->voice.
    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle) biased = &handle->voice;
    if (biased == voicePtr) self->field_0x20 = NULL;

    // Slot 0x24
    handle = self->field_0x24;
    biased = (CCharVoice*)handle;
    if (handle) biased = &handle->voice;
    if (biased == voicePtr) self->field_0x24 = NULL;

    // Rotating slots 0x2C..0x34 (count in field_0x3c).
    // Reusing handle/biased from outer scope keeps regalloc closer.
    for (int i = 0; i < self->field_0x3c; i++) {
        handle = self->field_0x2c[i];
        biased = (CCharVoice*)handle;
        if (handle) biased = &handle->voice;
        if (biased == voicePtr) self->field_0x2c[i] = NULL;
    }
}

// ── Target 3: us-802a8cc0 (func_802A658C) ──────────────────────────────────
// Advance the rotating index (field_0x38) forward or backward depending on the
// direction flag (field_0x44), wrapping at the bounds (0..field_0x3c). When the
// index lands on the stop index (field_0x40), the playback-start virtual is
// invoked; otherwise the slot-state triple is reloaded from lbl_eu_80539B14.
void func_802A658C(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Advance the rotating index forward or backward, wrapping at bounds.
    if (self->field_0x44 == 0) {
        // Forward: increment, wrap to 0 if past bound.
        self->field_0x38++;
        if (self->field_0x3c < self->field_0x38) {
            self->field_0x38 = 0;
        }
    } else {
        // Backward: decrement, wrap to bound-1 on underflow.
        self->field_0x38--;
        if (self->field_0x38 < 0) {
            self->field_0x38 = self->field_0x3c - 1;
        }
    }

    // Branch layout in retail: beq to vtable-call, fall-through for lbl copy.
    // To get the same layout we invert the condition so the lbl copy
    // (larger block) is the if-body and the vtable call is the else-body.
    if (self->field_0x38 != self->field_0x40) {
        // Reload slot-state triple {unk0, unk4, unk8} from init table.
        // Retail uses lis+lwzu for the first-element load to form the
        // base address.  We load [0] first into a local so MWCC has
        // the best chance of using the same lis+lwzu pattern.
        u32 v0 = lbl_eu_80539B14[0];
        self->unk4 = lbl_eu_80539B14[1];
        self->unk0 = (u32*)v0;
        self->unk8 = lbl_eu_80539B14[2];
    } else {
        self->func_802A3B50();
    }
}

// ── Target 4: us-802a8d84 (func_802A6650) ──────────────────────────────────
// Play function for slot 2. Reloads the slot-state triple from lbl_eu_80539B20,
// finds a free voice handle (excluding slot 2's own handle), and if that handle
// is inactive plays a random voice ID (mtRand(2) + 0x51D). On any failure the
// playback-start virtual is invoked as a fallback.
void func_802A6650(CVS_THREAD_EHP* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Reload slot-state triple {field_0, field_4, callback}.
    self->unk4 = lbl_eu_80539B20[1];
    self->unk0 = (u32*)lbl_eu_80539B20[0];
    self->unk8 = lbl_eu_80539B20[2];

    CVoiceHandle* handle = func_802A7998(self->field_0x24);
    if (handle != NULL) {
        // is-active check via the handle's vtable (offset 0x2BC).
        typedef int (*IsActiveFunc)(CVoiceHandle*);
        IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
        if (isActive(handle) == 0) {
            // Voice is not active -- play a random voice ID.
            CCharVoice* voicePtr = (CCharVoice*)handle;
            if (handle != NULL) {
                voicePtr = &handle->voice;
            }
            int voiceId = ml::math::mtRand(2) + 0x51D;
            if (func_802A3C44(self, voicePtr, voiceId) != 0) {
                return;
            }
        }
    }

    // Fallback: invoke the playback-start virtual.
    self->func_802A3B50();
}

// ── Target 5: us-802a8f54 (func_802A6820) ──────────────────────────────────
// Standalone EHP voice selector. Given two party-slot indices (a, b), it picks
// a voice ID based on their relationship, allocates a 0xAA-byte buffer, and
// plays the voice on the currently free handle. Returns 0 in all paths.
int func_802A6820(int a, int b) {
    if (a == b) {
        return 0;
    }

    CVoiceHandle* handle = func_802A7998((CVoiceHandle*)0);
    if (handle == NULL) {
        return 0;
    }

    // Skip if the current voice is still active.
    typedef int (*IsActiveFunc)(CVoiceHandle*);
    IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
    if (isActive(handle) != 0) {
        return 0;
    }

    // Read a u32 value from the handle's sub-object (vtable offset 0x30) and
    // gate the selection on a category check (func_80174C98).
    CVSubObj* subobj = handle->field_0x04;
    typedef u32* (*GetPtrFunc)(CVSubObj*);
    GetPtrFunc getPtr = (GetPtrFunc)subobj->vtable[0x30 / 4];
    u32* result = getPtr(subobj);
    u32 value = *result;
    if (func_80174C98(handle, &value, 0x803) == 0) {
        return 0;
    }

    // Choose the voice ID from the relationship between a and b.
    if (b < a && a >= 2) {
        b = ml::math::mtRand(2) + 0x6A5;
    } else if (a < b && a == 1) {
        b = 0x6A7;
    } else {
        return 0;
    }

    // Allocate the 0xAA-byte playback buffer.
    if (func_802A330C(0xAA, 1) == NULL) {
        return 0;
    }

    // Play the selected voice on the (biased) handle.
    CCharVoice* voicePtr = (CCharVoice*)handle;
    if (handle != NULL) {
        voicePtr = &handle->voice;
    }
    func_802A3D54(voicePtr, b, 0xAA);
    return 0;
}

// Stubs for functions not yet decompiled in this TU.
void __ct__802A5ED4() {}
void func_802A617C() {}
void func_802A6408() {}
void func_802A6958() {}
