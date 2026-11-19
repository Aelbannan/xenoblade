// CVS_THREAD_VISION_TELL: Voice thread for "Vision Tell" audio events.
// Five functions: completion callback, voice removal, constructor,
// slot-1 advance/play, slot-2 advance/play.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_TELL.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// us-802abdf8 (__ct__802A96C0)
// Factory/constructor for CVS_THREAD_VISION_TELL. Takes two voice handles,
// allocates a throwaway 0x32-byte buffer and the object itself (0x28 bytes),
// runs the base constructor and sets the derived vtable and slot handles
// inside the try block, then copies the init-state triple outside it.
// The redundant `self != NULL` guard reproduces retail's `beq` re-check of
// CR0 guarding the EH region; the catch rethrows via __throw(0,0,0) so MWCC
// elides the __end__catch epilogue.
CVS_THREAD_VISION_TELL* CVS_THREAD_VISION_TELL::create(CVoiceHandle* h1, CVoiceHandle* h2) {
    // Allocate handle buffer (discarded).
    if (CCharVoiceMan_DispatchById(0x32, 1) == NULL) {
        return NULL;
    }

    // Allocate the actual CVS_THREAD_VISION_TELL object.
    CVS_THREAD_VISION_TELL* self = (CVS_THREAD_VISION_TELL*)CCharVoiceMan_AllocVoiceArena(0x28);
    if (self == NULL) {
        return NULL;
    }

    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);

            // Override the vtable at 0x1C with the derived one, then the slots.
            ((CVS_THREAD_VISION_TELL_raw*)self)->vtable = lbl_eu_80539DD4;
            self->field_0x20 = h1;
            self->field_0x24 = h2;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Install the initial voice-event handler as a real member-function
    // pointer; MWCC emits the {0, -1, handler} constant itself.
    self->mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_VISION_TELL::playSlot1);

    return self;
}

// us-802abed8 (VisionTell_PlaySlot1)
// Advance/play function for voice slot 1 (field_0x20).
void CVS_THREAD_VISION_TELL::playSlot1() {
    CVS_THREAD_VISION_TELL* self = this;
    mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_VISION_TELL::playSlot2);
    CVoiceHandle* handle20 = self->field_0x20;

    // Both slots must be populated
    if (handle20 == NULL) return;
    if (self->field_0x24 == NULL) return;

    // Check if slot 1 voice is still active (virtual dispatch -> r12 chain)
    if (((CVS_THREAD_VISION_TELL_Vtbl*)handle20)->isVoiceActive() != 0) return;

    // Get iterator from slot 2 handle
    if (HpVoice_ValidIter(HpVoice_MapSubState(self->field_0x24)) == 0) return;

    // Select voice ID based on iterator value
    int voiceId;
    switch (HpVoice_MapSubState(self->field_0x24)) {
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
        // Ternary arm order matters: retail emits li 0x76F; bne; li 0xCC.
        voiceId = (HpVoice_CheckPair26(self->field_0x20, self->field_0x24) == 0) ? 0xCC : 0x76F;
        break;
    case 7:
        voiceId = 0xCD;
        break;
    case 3:
        voiceId = 0xCE;
        break;
    case 1:
        voiceId = (HpVoice_CheckPairGate(self->field_0x20, self->field_0x24) == 0) ? 0xCF : 0x51A;
        break;
    default:
        self->PlayThreadVoice();
        return;
    }

    // Get voice pointer from slot 1 handle. Retail re-reads field_0x20 from
    // memory here (does not reuse the init-time handle cache).
    CCharVoice* voicePtr = (CCharVoice*)self->field_0x20;
    if (self->field_0x20 != NULL) {
        voicePtr = &self->field_0x20->voice;
    }

    // Try to play the selected voice
    if (TryRequestVoice(self, voicePtr, voiceId) == 0) {
        self->PlayThreadVoice();
    }
}

// us-802ac05c (VisionTell_PlaySlot2)
// Advance/play function for voice slot 2 (field_0x24). Retail re-reads
// self->field_0x24/field_0x20 from memory at each use (no long-lived caches).
void CVS_THREAD_VISION_TELL::playSlot2() {
    CVS_THREAD_VISION_TELL* self = this;
    if (IsThreadVoiceActive(self) != 0) {
        return;
    }

    mState = static_cast<void (CVS_THREAD::*)()>(
        &CVS_THREAD_VISION_TELL::notifyComplete);
    CVoiceHandle* handle24 = self->field_0x24;

    // Initial active-state gate on the slot-2 handle (read once here).
    if (handle24 == NULL) return;
    if (((CVS_THREAD_VISION_TELL_Vtbl*)handle24)->isVoiceActive() != 0) return;

    // Iterator active?
    if (HpVoice_ValidIter(HpVoice_MapSubState(self->field_0x24)) != 0) {
        // Slot-1 voice is in an active state (1): play the scripted voice.
        if (self->field_0x20 != NULL && HpVoice_MapSubState(self->field_0x20) == 1) {
            CCharVoice* voicePtr = (CCharVoice*)self->field_0x24;
            if (self->field_0x24 != NULL) {
                voicePtr = &self->field_0x24->voice;
            }
            if (TryRequestVoice(self, voicePtr, 0x12D) == 0) {
                self->PlayThreadVoice();
            }
            return;
        }

        // Random slot-2 voice from the first table.
        CCharVoice* voicePtr = (CCharVoice*)self->field_0x24;
        if (self->field_0x24 != NULL) {
            voicePtr = &self->field_0x24->voice;
        }
        if (TryRequestVoice(self, voicePtr, lbl_eu_80662D58[ml::math::mtRand(4)]) == 0) {
            self->PlayThreadVoice();
        }
        return;
    }

    // Iterator inactive: allow only slot-2 states 8 or 10. Retail re-reads
    // field_0x24 and re-calls HpVoice_MapSubState for EACH state test (two calls),
    // so use a short-circuit || (not a cached local) to reproduce the second
    // lwz + bl pair.
    if (HpVoice_MapSubState(self->field_0x24) == 8 ||
        HpVoice_MapSubState(self->field_0x24) == 10) {
        // Random slot-2 voice from the second table.
        CCharVoice* voicePtr = (CCharVoice*)self->field_0x24;
        if (self->field_0x24 != NULL) {
            voicePtr = &self->field_0x24->voice;
        }
        if (TryRequestVoice(self, voicePtr, lbl_eu_80662D60[ml::math::mtRand(3)]) == 0) {
            self->PlayThreadVoice();
        }
    }
}

// us-802ac1fc (VisionTell_NotifyComplete)
// Completion callback: if no active voice, call the playback-start virtual.
void CVS_THREAD_VISION_TELL::notifyComplete() {
    if (IsThreadVoiceActive(this) == 0) {
        PlayThreadVoice();
    }
}

// us-802ac244 (VisionTell_RemoveVoice)
// Remove a voice from the slots by matching its embedded CCharVoice pointer.
void VisionTell_RemoveVoice(CVS_THREAD_VISION_TELL* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

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

// Virtual method override: returns the buffer size for this thread type.
int CVS_THREAD_VISION_TELL::blank1() {
    return BUFFER_SIZE;
}