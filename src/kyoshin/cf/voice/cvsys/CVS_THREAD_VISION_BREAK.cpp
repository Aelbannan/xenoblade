// CVS_THREAD_VISION_BREAK: Voice thread for vision-break status effect.
// Five matched functions: completion callback, voice removal, voice selector,
// factory constructor, and the playback/iteration dispatcher.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_BREAK.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/reslist.hpp"

// Returns a reslist of CfObject pointers from the global object manager.
// C++ linkage (retail mangled name func_800B6BA4__Fv).
extern reslist<cf::CfObject*>* func_800B6BA4();

// Sibling-voice imports (func_802A3E88, func_802A3BEC, func_802A3C44,
// func_802A3D54, func_802A330C, func_802A34E4, func_802A7A54,
// func_8016FE34, func_800BE924) and the base constructor
// __ct__cf_CVS_THREAD are declared in CVS_THREAD_VISION_BREAK.hpp.

// Active-check function type: vtable[0x2BC/4] on a CVoiceHandle.
typedef int (*IsActiveFunc)(CVoiceHandle*);

// ── Target 1: us-802abc94 (func_802A955C) ──────────────────────────────────
// Completion callback: if no active voice is playing, invoke the playback
// virtual (CVS_THREAD::func_802A3B50, vtable slot 1).
void func_802A955C(CVS_THREAD_VISION_BREAK* self) {
    if (func_802A3E88(self) == 0) {
        self->func_802A3B50();
    }
}

// ── Target 2: us-802abcdc (func_802A95A4) ──────────────────────────────────
// Remove a voice from the field_0x20 slot by matching its embedded CCharVoice.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void func_802A95A4(CVS_THREAD_VISION_BREAK* self, CCharVoice* voicePtr) {
    // Direct call to the free function at 0x802A3BEC.
    func_802A3BEC(self, voicePtr);

    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }
}

// ── Target 3: us-802abd3c (func_802A9604) ──────────────────────────────────
// Standalone voice selector for vision-break. Finds a free voice handle via
// func_802A7A54(1), allocates a 0x32-byte buffer, and plays a voice ID chosen
// based on the current game-manager frame count. Always returns 0.
int func_802A9604() {
    CVoiceHandle* handle = func_802A7A54(1);
    if (handle == NULL) {
        return 0;
    }

    // Allocate a 0x32-byte playback buffer.
    if (func_802A330C(0x32, 1) == NULL) {
        return 0;
    }

    // Select a voice ID based on the game-manager frame counter.
    // Retail calls func_800822F4() for EACH branch condition (not once).
    // Cast to u32 ensures unsigned compare (cmpli) matches retail.
    int voiceId;
    if ((u32)cf::CfGameManager::func_800822F4() < 0x38u) {
        // Early phase: voice IDs 0x65-0x66.
        voiceId = ml::math::mtRand(2) + 0x65;
    } else if ((u32)cf::CfGameManager::func_800822F4() < 0x62u) {
        // Mid phase: voice IDs 0x67-0x68.
        voiceId = ml::math::mtRand(2) + 0x67;
    } else {
        // Late phase: voice IDs 0x69-0x6A.
        voiceId = ml::math::mtRand(2) + 0x69;
    }

    // Bias the handle to its embedded CCharVoice.
    CCharVoice* voicePtr = (CCharVoice*)handle;
    if (handle != NULL) {
        voicePtr = &handle->voice;
    }

    func_802A3D54(voicePtr, voiceId, 0x32);
    return 0;
}

// ── Target 4: us-802aba10 (__ct__802A92D8) ─────────────────────────────────
// Factory constructor for CVS_THREAD_VISION_BREAK. Takes a CVoiceHandle
// (manager) and a parameter; validates both, allocates a new 0x28-byte
// object, initialises it, and returns it. Returns NULL on failure.
CVS_THREAD_VISION_BREAK* __ct__802A92D8(CVoiceHandle* handle, int param) try {
    // Parameter must be positive.
    if (param <= 0) {
        return NULL;
    }

    // The handle's +0x3F00 flag (bit 30 = value bit 1) must be set.
    if ((handle->field_0x3F00 & 2) == 0) {
        return NULL;
    }

    // Check that the handle is not already active (vtable[0x2BC/4]).
    if (((IsActiveFunc)handle->vtable[0x2BC / 4])(handle) != 0) {
        return NULL;
    }

    // Allocate a temporary 0xF-byte buffer (voice handle reservation).
    if (func_802A330C(0xF, 1) == NULL) {
        return NULL;
    }

    // Allocate the 0x28-byte CVS_THREAD_VISION_BREAK object.
    CVS_THREAD_VISION_BREAK* obj = (CVS_THREAD_VISION_BREAK*)func_802A34E4(0x28);
    if (obj == NULL) {
        return NULL;
    }

    // Call the base CVS_THREAD constructor.
    __ct__cf_CVS_THREAD(obj);

    // Override the vtable (at 0x1C, index 7) with the VISION_BREAK vtable.
    ((void**)obj)[7] = (void**)lbl_eu_80539D88;
    obj->field_0x20 = handle;
    obj->field_0x24 = param;

    // Copy the initial-state triple (lbl_eu_80539D70) to obj's base fields.
    const u32* base = lbl_eu_80539D70;
    obj->unk0 = (u32*)base[0];
    obj->unk4 = base[1];
    obj->unk8 = base[2];

    return obj;
} catch (...) {
    throw;
}

// ── Target 5: us-802abb34 (func_802A93FC) ──────────────────────────────────
// Playback/iteration dispatcher. Resets the state triple, checks if the
// handle is inactive, iterates the global object list to process active
// voice handles, then selects a voice ID from one of two tables (based on
// field_0x24 threshold) and plays it. Falls back to vtable[2] if play fails.
void func_802A93FC(CVS_THREAD_VISION_BREAK* self) {
    // Reset the state triple from lbl_eu_80539D7C (consecutive reads so
    // MWCC keeps the base register and emits the lwzu form).
    self->unk0 = (u32*)lbl_eu_80539D7C[0];
    self->unk4 = lbl_eu_80539D7C[1];
    self->unk8 = lbl_eu_80539D7C[2];

    CVoiceHandle* handle = self->field_0x20;
    if (handle == NULL) {
        return;
    }

    // Check if the handle is already active.
    if (((IsActiveFunc)handle->vtable[0x2BC / 4])(handle) != 0) {
        return;
    }

    // Iterate through the global object list, processing voice handles.
    reslist<cf::CfObject*>* list = func_800B6BA4();
    for (reslist<cf::CfObject*>::iterator it = list->begin(); it != list->end(); ++it) {
        cf::CfObject* obj = *it;
        // func_8016FE34 returns a CVoiceHandle*, and the CCharVoice is embedded at +0x3E9C.
        CVoiceHandle* resultHandle = (CVoiceHandle*)func_8016FE34(obj);
        if (resultHandle != NULL) {
            func_800BE924(&resultHandle->voice);
        }
    }

    // Select a voice ID from the appropriate phase table based on field_0x24.
    CCharVoice* voicePtr;
    int voiceId;
    if (self->field_0x24 >= 3) {
        // Late phase: handle + voice IDs from lbl_eu_80662D40.
        CVoiceHandle* h = self->field_0x20;
        voicePtr = (CCharVoice*)h;
        if (h != NULL) {
            voicePtr = &h->voice;
        }
        voiceId = lbl_eu_80662D40[ml::math::mtRand(3)];
        if (func_802A3C44(self, voicePtr, voiceId) == 0) {
            // Play failed -- end the sequence via the blank1() virtual.
            static_cast<CVS_THREAD*>(self)->blank1();
        }
    } else {
        // Early phase: handle + voice IDs from lbl_eu_80662D48.
        CVoiceHandle* h = self->field_0x20;
        voicePtr = (CCharVoice*)h;
        if (h != NULL) {
            voicePtr = &h->voice;
        }
        voiceId = lbl_eu_80662D48[ml::math::mtRand(3)];
        if (func_802A3C44(self, voicePtr, voiceId) == 0) {
            // Play failed -- end the sequence via the blank1() virtual.
            static_cast<CVS_THREAD*>(self)->blank1();
        }
    }
}