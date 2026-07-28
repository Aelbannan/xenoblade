// CVS_THREAD_DOWN: Voice thread for "Down" (knocked-down) status effect.
// Five functions: completion callback, voice removal, slot-1 play, slot-2 play, constructor.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_DOWN.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// Forward declarations for sibling TU functions.
extern "C" {
    int func_802A3E88(CVS_THREAD* self);
    void func_802A3BEC(CVS_THREAD* self, CCharVoice* voicePtr);
    int func_802A3C44(CVS_THREAD* self, CCharVoice* voicePtr, int voiceId);
    CVS_THREAD_DOWN* func_802A330C(int size, int align);
    u8* func_802A34E4(int size);
}

// Base constructor: no explicit params — takes self from r3.
extern "C" void __ct__cf_CVS_THREAD();

// Init data tables for slot states (3 u32s each).
extern "C" u32 lbl_eu_80539A68[3];
extern "C" u32 lbl_eu_80539A74[3];
extern "C" u32 lbl_eu_80539A80[3];

// Vtable for CVS_THREAD_DOWN.
extern "C" u32 lbl_eu_80539A8C[7];

// ── Target 1: us-802a8540 (func_802A5E0C) ──────────────────────────────────
// Completion callback: if no active voice, call the playback-start virtual.
void func_802A5E0C(CVS_THREAD_DOWN* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802a8588 (func_802A5E54) ──────────────────────────────────
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void func_802A5E54(CVS_THREAD_DOWN* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);

    // Slot 0x20: load handle, bias if non-null, compare with voicePtr
    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }

    // Slot 0x24: same pattern
    handle = self->field_0x24;
    biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x24 = NULL;
    }
}

// ── Target 3: us-802a83c4 (func_802A5C90) ──────────────────────────────────
// Advance/play function for voice slot 1 (field_0x20).
// Copies init data from lbl_eu_80539A74 to fields 0x00-0x08, checks if
// the voice is still active (vtable method at offset 0x2BC), and if
// inactive, plays a random voice ID (mtRand(2) + 0x70C).
void func_802A5C90(CVS_THREAD_DOWN* self) {
    const u32* base = lbl_eu_80539A74;
    u32 v0 = base[0];
    CVoiceHandle* handle20 = self->field_0x20;
    u32 v1 = base[1];
    u32 v2 = base[2];
    self->unk4 = v1;
    self->unk0 = (u32*)v0;
    self->unk8 = v2;

    if (handle20 != NULL) {
        // Call vtable method at offset 0x2BC (is-active check)
        if (((int (*)(CVoiceHandle*))handle20->vtable[0x2BC / 4])(handle20) == 0) {
            // Voice is not active -- try to play a random voice
            handle20 = self->field_0x20;
            CCharVoice* voicePtr = (CCharVoice*)handle20;
            if (handle20 != NULL) {
                voicePtr = &handle20->voice;
            }
            int voiceId = ml::math::mtRand(2) + 0x70C;
            if (func_802A3C44(self, voicePtr, voiceId) == 0) {
                self->func_802A3B50();
            }
        }
    }
}

// ── Target 4: us-802a8480 (func_802A5D4C) ──────────────────────────────────
// Advance/play function for voice slot 2 (field_0x24).
// Copies init data from lbl_eu_80539A80 to fields 0x00-0x08, checks if
// the voice is still active, and if inactive, plays a voice ID chosen by
// mtRand(2): 0x713 when rand==0, otherwise 0x70E.
void func_802A5D4C(CVS_THREAD_DOWN* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    const u32* base = lbl_eu_80539A80;
    u32 v0 = base[0];
    CVoiceHandle* handle24 = self->field_0x24;
    u32 v1 = base[1];
    u32 v2 = base[2];
    self->unk4 = v1;
    self->unk0 = (u32*)v0;
    self->unk8 = v2;

    if (handle24 != NULL) {
        // Call vtable method at offset 0x2BC (is-active check)
        if (((int (*)(CVoiceHandle*))handle24->vtable[0x2BC / 4])(handle24) == 0) {
            // Voice is not active -- try to play a weighted random voice
            int voiceId = 0x713;
            if (ml::math::mtRand(2) != 0) voiceId = 0x70E;
            handle24 = self->field_0x24;
            CCharVoice* voicePtr = (CCharVoice*)handle24;
            if (handle24 != NULL) {
                voicePtr = &handle24->voice;
            }
            if (func_802A3C44(self, voicePtr, voiceId) == 0) {
                self->func_802A3B50();
            }
        }
    }
}

// ── Target 5: us-802a82bc (__ct__802A5B88) ──────────────────────────────────
// Factory/constructor for CVS_THREAD_DOWN. Takes two owner objects (with a
// field at offset 0x3F00 that must have bit 1 set), allocates a handle
// (0xF0 bytes, discarded) and the object itself (0x28 bytes), calls the
// base constructor, sets vtable/owner fields, and copies init data from
// lbl_eu_80539A68.
CVS_THREAD_DOWN* __ct__802A5B88(CVS_THREAD_DOWN* owner1, CVS_THREAD_DOWN* owner2) try {
    // Both owners must have their 0x3F00 field's bit 1 set
    if (!(((u32*)owner1)[0x3F00 / 4] & 2)) return NULL;
    if (!(((u32*)owner2)[0x3F00 / 4] & 2)) return NULL;

    // Allocate handle buffer (0xF0 bytes, discarded)
    CVS_THREAD_DOWN* handleBuf = func_802A330C(0xF0, 1);
    if (handleBuf == NULL) return NULL;

    // Allocate the actual CVS_THREAD_DOWN object
    CVS_THREAD_DOWN* self = (CVS_THREAD_DOWN*)func_802A34E4(0x28);
    if (self == NULL) return NULL;

    // Base constructor (no params, self in r3)
    __ct__cf_CVS_THREAD();

    // Set vtable at offset 0x1C (right after the 7 CVS_THREAD base fields)
    ((void**)self)[7] = (void**)lbl_eu_80539A8C;
    self->field_0x20 = (CVoiceHandle*)owner1;
    self->field_0x24 = (CVoiceHandle*)owner2;

    // Copy init data from global table using a single base pointer
    const u32* base = lbl_eu_80539A68;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
} catch (...) {
    throw;
}