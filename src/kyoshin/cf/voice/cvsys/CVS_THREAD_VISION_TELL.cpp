// CVS_THREAD_VISION_TELL: Voice thread for "Vision Tell" audio events.
// Five functions: completion callback, voice removal, constructor,
// slot-1 advance/play, slot-2 advance/play.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// ── Target 1: us-802ac1fc (func_802A9AC4) ──────────────────────────────────
// Completion callback: if no active voice, call the playback-start virtual.
void func_802A9AC4(CVS_THREAD_VISION_TELL* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802ac244 (func_802A9B0C) ──────────────────────────────────
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void func_802A9B0C(CVS_THREAD_VISION_TELL* self, CCharVoice* voicePtr) {
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

// ── Target 3: us-802abdf8 (__ct__802A96C0) ──────────────────────────────────
// Factory/constructor for CVS_THREAD_VISION_TELL. Takes two owner objects,
// allocates a handle (0x32 bytes, discarded) and the object itself (0x28 bytes),
// calls the base constructor, sets vtable/owner fields, and copies init data.
CVS_THREAD_VISION_TELL* __ct__802A96C0(CVS_THREAD_VISION_TELL* owner1, CVS_THREAD_VISION_TELL* owner2) try {
    // Allocate handle buffer (discarded)
    CVoiceHandle* handleBuf = func_802A330C(0x32, 1);
    if (handleBuf == NULL) return NULL;

    // Allocate the actual CVS_THREAD_VISION_TELL object
    CVS_THREAD_VISION_TELL* self = (CVS_THREAD_VISION_TELL*)func_802A34E4(0x28);
    if (self == NULL) return NULL;

    // Base constructor (self already in r3).
    __ct__cf_CVS_THREAD();

    // Set vtable at offset 0x1C (right after the 7 CVS_THREAD base words).
    ((void**)self)[7] = (void**)lbl_eu_80539DD4;
    self->field_0x20 = (CVoiceHandle*)owner1;
    self->field_0x24 = (CVoiceHandle*)owner2;

    // Copy init data from global table using a single base pointer
    const u32* base = lbl_eu_80539DB0;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
} catch (...) {
    throw;
}

// ── Target 4: us-802abed8 (func_802A97A0) ──────────────────────────────────
// Advance/play function for voice slot 1 (field_0x20).
void func_802A97A0(CVS_THREAD_VISION_TELL* self) {
    // Copy init data -- read index 0 first so MWCC emits the lwzu base-load
    // form (lis @ha + lwzu @l), then the handle, then the remaining elements.
    self->unk0 = (u32*)lbl_eu_80539DBC[0];
    CVoiceHandle* handle20 = self->field_0x20;
    self->unk4 = lbl_eu_80539DBC[1];
    self->unk8 = lbl_eu_80539DBC[2];

    // Both slots must be populated
    if (handle20 == NULL) return;
    if (self->field_0x24 == NULL) return;

    // Check if slot 1 voice is still active
    typedef int (*CheckFunc)(CVoiceHandle*);
    CheckFunc checkFunc = (CheckFunc)handle20->vtable[0x2BC / 4];
    if (checkFunc(handle20) != 0) return;

    // Get iterator from slot 2 handle
    int iter = func_802A77E8(self->field_0x24);
    if (func_802A7850(iter) == 0) return;

    // Select voice ID based on iterator value
    int voiceId;
    switch (func_802A77E8(self->field_0x24)) {
    case 2:
        voiceId = 0xC9;
        break;
    case 5:
        voiceId = 0xCA;
        break;
    case 4:
        voiceId = 0xCB;
        break;
    case 6:
        if (func_802A7B90(self->field_0x20, self->field_0x24) != 0) {
            voiceId = 0x76F;
        } else {
            voiceId = 0xCC;
        }
        break;
    case 7:
        voiceId = 0xCD;
        break;
    case 3:
        voiceId = 0xCE;
        break;
    case 1:
        if (func_802A7EB0(self->field_0x20, self->field_0x24) != 0) {
            voiceId = 0x51A;
        } else {
            voiceId = 0xCF;
        }
        break;
    default:
        self->func_802A3B50();
        return;
    }

    // Get voice pointer from slot 1 handle
    CCharVoice* voicePtr = (CCharVoice*)handle20;
    if (handle20 != NULL) {
        voicePtr = &handle20->voice;
    }

    // Try to play the selected voice
    if (func_802A3C44(self, voicePtr, voiceId) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 5: us-802ac05c (func_802A9924) ──────────────────────────────────
// Advance/play function for voice slot 2 (field_0x24).
void func_802A9924(CVS_THREAD_VISION_TELL* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Copy init data -- read index 0 first so MWCC emits the lwzu base-load
    // form, then the handle, then the remaining elements.
    self->unk0 = (u32*)lbl_eu_80539DC8[0];
    CVoiceHandle* handle24 = self->field_0x24;
    self->unk4 = lbl_eu_80539DC8[1];
    self->unk8 = lbl_eu_80539DC8[2];

    if (handle24 == NULL) return;

    // Call vtable method at offset 0x2BC (is-active check)
    typedef int (*CheckFunc)(CVoiceHandle*);
    CheckFunc checkFunc = (CheckFunc)handle24->vtable[0x2BC / 4];
    if (checkFunc(handle24) != 0) return;

    // Get voice iterator from slot 2 handle
    int iter = func_802A77E8(self->field_0x24);
    if (func_802A7850(iter) != 0) {
        // Iterator is active -- try slot 1 voice or random selection
        CVoiceHandle* handle20 = self->field_0x20;
        if (handle20 != NULL) {
            if (func_802A77E8(handle20) == 1) {
                // Slot 1 voice is in state 1 -- play a specific voice
                CCharVoice* voicePtr = (CCharVoice*)handle24;
                if (handle24 != NULL) {
                    voicePtr = &handle24->voice;
                }
                if (func_802A3C44(self, voicePtr, 0x12D) == 0) {
                    self->func_802A3B50();
                }
                return;
            }
        }

        // Random selection from lbl_eu_80662D58 table
        CCharVoice* voicePtr = (CCharVoice*)handle24;
        if (handle24 != NULL) {
            voicePtr = &handle24->voice;
        }
        int voiceId = lbl_eu_80662D58[ml::math::mtRand(4)];
        if (func_802A3C44(self, voicePtr, voiceId) == 0) {
            self->func_802A3B50();
        }
        return;
    }

    // Iterator is inactive -- check if slot 2 voice is in state 8 or 10
    int state = func_802A77E8(self->field_0x24);
    if (state != 8 && state != 10) return;

    // Random selection from lbl_eu_80662D60 table
    CCharVoice* voicePtr = (CCharVoice*)handle24;
    if (handle24 != NULL) {
        voicePtr = &handle24->voice;
    }
    int voiceId = lbl_eu_80662D60[ml::math::mtRand(3)];
    if (func_802A3C44(self, voicePtr, voiceId) == 0) {
        self->func_802A3B50();
    }
}

// Virtual method override: returns the buffer size for this thread type.
int CVS_THREAD_VISION_TELL::blank1() {
    return BUFFER_SIZE;
}