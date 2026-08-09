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
// Factory/constructor for CVS_THREAD_VISION_TELL. Takes a source CVoiceHandle
// and a second handle, allocates a throwaway 0x32-byte buffer and the object
// itself (0x28 bytes), runs the base constructor and sets the derived vtable
// and slot handles inside the try block, then copies the init-state triple.
CVS_THREAD_VISION_TELL* __ct__802A96C0(CVoiceHandle* h1, CVoiceHandle* h2) {
    // Allocate handle buffer (discarded).
    if (func_802A330C(0x32, 1) == NULL) {
        return NULL;
    }

    // Allocate the actual CVS_THREAD_VISION_TELL object.
    CVS_THREAD_VISION_TELL* self = (CVS_THREAD_VISION_TELL*)func_802A34E4(0x28);
    if (self == NULL) {
        return NULL;
    }

    // Base-initialise the object (vtable, IDs). Wrap in try so a base-ctor
    // failure unwinds via __throw -- the retail catch rethrows.
    try {
        __ct__cf_CVS_THREAD(self);

        // Override the vtable at offset 0x1C (index 7) with the derived one.
        ((void**)self)[7] = (void**)lbl_eu_80539DD4;
        self->field_0x20 = h1;
        self->field_0x24 = h2;
    } catch (...) {
        throw;
    }

    // Copy init data from the global table using a single base pointer.
    const u32* base = lbl_eu_80539DB0;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];

    return self;
}

// ── Target 4: us-802abed8 (func_802A97A0) ──────────────────────────────────
// Advance/play function for voice slot 1 (field_0x20).
void func_802A97A0(CVS_THREAD_VISION_TELL* self) {
    // Copy init data -- read index 0 first (lwzu pointer-increment) so MWCC
    // emits the lis @ha + lwzu @l base-load form, then the handle, then the
    // remaining elements. Match the HAGE pattern.
    const u32* p = lbl_eu_80539DBC;
    u32 v0 = *p++;
    CVoiceHandle* handle20 = self->field_0x20;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    // Both slots must be populated
    if (handle20 == NULL) return;
    if (self->field_0x24 == NULL) return;

    // Check if slot 1 voice is still active (inline cast, matches retail r12
    // dispatch and avoids pinning the function pointer into a callee-saved reg).
    if (((int (*)(CVoiceHandle*))handle20->vtable[0x2BC / 4])(handle20) != 0) return;

    // Get iterator from slot 2 handle
    if (func_802A7850(func_802A77E8(self->field_0x24)) == 0) return;

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

    // Get voice pointer from slot 1 handle. Retail re-reads field_0x20 from
    // memory here (does not reuse the init-time handle cache).
    CCharVoice* voicePtr = (CCharVoice*)self->field_0x20;
    if (self->field_0x20 != NULL) {
        voicePtr = &self->field_0x20->voice;
    }

    // Try to play the selected voice
    if (func_802A3C44(self, voicePtr, voiceId) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 5: us-802ac05c (func_802A9924) ──────────────────────────────────
// Advance/play function for voice slot 2 (field_0x24). Retail re-reads
// self->field_0x24/field_0x20 from memory at each use (no long-lived caches).
void func_802A9924(CVS_THREAD_VISION_TELL* self) {
    if (func_802A3E88(self) != 0) {
        return;
    }

    // Copy init data -- index 0 first (lwzu pointer-increment) to force the
    // retail lis @ha + lwzu @l base-load form.
    const u32* p = lbl_eu_80539DC8;
    u32 v0 = *p++;
    CVoiceHandle* handle24 = self->field_0x24;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    // Initial active-state gate on the slot-2 handle (read once here).
    if (handle24 == NULL) return;
    if (((int (*)(CVoiceHandle*))handle24->vtable[0x2BC / 4])(handle24) != 0) return;

    // Iterator active?
    if (func_802A7850(func_802A77E8(self->field_0x24)) != 0) {
        // Slot-1 voice is in an active state (1): play the scripted voice.
        if (self->field_0x20 != NULL && func_802A77E8(self->field_0x20) == 1) {
            CCharVoice* voicePtr = (CCharVoice*)self->field_0x24;
            if (self->field_0x24 != NULL) {
                voicePtr = &self->field_0x24->voice;
            }
            if (func_802A3C44(self, voicePtr, 0x12D) == 0) {
                self->func_802A3B50();
            }
            return;
        }

        // Random slot-2 voice from the first table.
        CCharVoice* voicePtr = (CCharVoice*)self->field_0x24;
        if (self->field_0x24 != NULL) {
            voicePtr = &self->field_0x24->voice;
        }
        if (func_802A3C44(self, voicePtr, lbl_eu_80662D58[ml::math::mtRand(4)]) == 0) {
            self->func_802A3B50();
        }
        return;
    }

    // Iterator inactive: allow only slot-2 states 8 or 10. Retail re-reads
    // field_0x24 and re-calls func_802A77E8 for EACH state test (two calls),
    // so use a short-circuit || (not a cached local) to reproduce the second
    // lwz + bl pair.
    if (func_802A77E8(self->field_0x24) == 8 ||
        func_802A77E8(self->field_0x24) == 10) {
        // Random slot-2 voice from the second table.
        CCharVoice* voicePtr = (CCharVoice*)self->field_0x24;
        if (self->field_0x24 != NULL) {
            voicePtr = &self->field_0x24->voice;
        }
        if (func_802A3C44(self, voicePtr, lbl_eu_80662D60[ml::math::mtRand(3)]) == 0) {
            self->func_802A3B50();
        }
    }
}

// Virtual method override: returns the buffer size for this thread type.
int CVS_THREAD_VISION_TELL::blank1() {
    return BUFFER_SIZE;
}