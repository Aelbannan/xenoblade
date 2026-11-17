// CVS_THREAD_VISION_BREAK: Voice thread for vision-break status effect.
// Five matched functions: completion callback, voice removal, voice selector,
// factory constructor, and the playback/iteration dispatcher.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_VISION_BREAK.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/Random.hpp"
#include "monolib/util/reslist.hpp"

// All sibling-voice imports (IsThreadVoiceActive, DetachThreadVoice, TryRequestVoice,
// RequestVoicePlayback, CCharVoiceMan_DispatchById, CCharVoiceMan_AllocVoiceArena, HpVoice_FindBySubState,
// MapFx_ResolveOwner, CfObjectMove_releaseVoiceHandle), the base constructor __ct__cf_CVS_THREAD,
// and getListB28 are declared in CVS_THREAD_VISION_BREAK.hpp.

// Sibling-voice imports (IsThreadVoiceActive, DetachThreadVoice, TryRequestVoice,
// RequestVoicePlayback, CCharVoiceMan_DispatchById, CCharVoiceMan_AllocVoiceArena, HpVoice_FindBySubState,
// MapFx_ResolveOwner, CfObjectMove_releaseVoiceHandle) and the base constructor
// __ct__cf_CVS_THREAD are declared in CVS_THREAD_VISION_BREAK.hpp.

// us-802aba10 (__ct__802A92D8)
// Factory constructor for CVS_THREAD_VISION_BREAK. Validates the parameter,
// manager-flag and active-check gates, allocates the object, runs the base
// constructor and derived-vtable/slot stores inside an EH region (retail
// guards them with a try block whose catch rethrows via __throw(0,0,0)),
// then copies the init-state triple outside the try. Returns NULL on failure.
extern "C" CVS_THREAD_VISION_BREAK* __ct__802A92D8(CVoiceHandle* handle, int param) {
    // Parameter must be positive.
    if (param <= 0) {
        return NULL;
    }

    // The handle's +0x3F00 flag (bit 1) must be set.
    if ((handle->field_0x3F00 & 2) == 0) {
        return NULL;
    }

    // Check that the handle is not already active (vtable slot 173 = 0x2BC).
    // Cast the handle itself: the phantom's implicit vtable slot is at 0x0.
    if (((CVoiceHandleVTable*)handle)->isVoiceActive() != 0) {
        return NULL;
    }

    // Allocate a temporary 0xF-byte buffer (voice handle reservation).
    if (CCharVoiceMan_DispatchById(0xF, 1) == NULL) {
        return NULL;
    }

    // Allocate the 0x28-byte CVS_THREAD_VISION_BREAK object.
    CVS_THREAD_VISION_BREAK* obj = (CVS_THREAD_VISION_BREAK*)CCharVoiceMan_AllocVoiceArena(0x28);
    if (obj == NULL) {
        return NULL;
    }

    // Redundant null re-check reproduces retail's `beq` re-use of CR0
    // guarding the EH region.
    if (obj != NULL) {
        try {
            // Base CVS_THREAD constructor, then override the vtable at 0x1C
            // with the VISION_BREAK vtable, then the slots.
            __ct__cf_CVS_THREAD(obj);
            ((CVS_THREAD_VISION_BREAK_raw*)obj)->vtable = lbl_eu_80539D88;
            obj->field_0x20 = handle;
            obj->field_0x24 = param;
        } catch (...) {
            // Retail landing pad: __throw(0,0,0).
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple (lbl_eu_80539D70). Word 1 is read first per
    // retail load order; the single pointer keeps the label materialized
    // once via lis+addi (retail r5).
    u32 v1;
    u32* p0;
    v1 = (p0 = lbl_eu_80539D70)[1];
    ((CVS_THREAD_VISION_BREAK_raw*)obj)->state0 = (u32*)p0[0];
    ((CVS_THREAD_VISION_BREAK_raw*)obj)->state1 = v1;
    ((CVS_THREAD_VISION_BREAK_raw*)obj)->state2 = p0[2];

    return obj;
}

// us-802abb34 (VisBreak_DispatchPlayback)
// Playback/iteration dispatcher. Resets the state triple, checks if the
// handle is inactive, iterates the global object list to process active
// voice handles, then selects a voice ID from one of two tables (based on
// field_0x24 threshold) and plays it. Falls back to vtable[2] if play fails.
void VisBreak_DispatchPlayback(CVS_THREAD_VISION_BREAK* self) {
    // Declared first so MWCC colours the object-list walk into r31/r30,
    // leaving r29 for `self` (retail's allocation).
    reslist<cf::CfObject*>* list;
    // Reset the state triple from lbl_eu_80539D7C. Pointer increment
    // reproduces the lwzu/spread load-with-update fold (same shape as the
    // CVS_THREAD_BATTLE_END triple copy); v0 declared first so MWCC colours
    // the lwzu destination low.
    u32 v0;
    const u32* reset = lbl_eu_80539D7C;
    v0 = *reset++;
    CVoiceHandle* handle = self->field_0x20;
    CVS_THREAD_VISION_BREAK_raw* raw = (CVS_THREAD_VISION_BREAK_raw*)self;
    raw->state1 = *reset++;
    raw->state0 = (u32*)v0;
    raw->state2 = *reset;

    if (handle == NULL) {
        return;
    }

    // Check if the handle is already active. Phantom-vtable view so the
    // dispatch emits the retail lwz r12 chain (slot 173 = offset 0x2BC).
    if (((CVoiceHandleVTable*)handle)->isVoiceActive() != 0) {
        return;
    }

    // Iterate through the global object list, processing voice handles.
    list = getListB28();
    for (reslist<cf::CfObject*>::iterator it = list->begin(); it != list->end(); ++it) {
        cf::CfObject* obj = *it;
        // MapFx_ResolveOwner returns a CVoiceHandle*, and the CCharVoice is embedded at +0x3E9C.
        CVoiceHandle* resultHandle = (CVoiceHandle*)MapFx_ResolveOwner(obj);
        if (resultHandle != NULL) {
            CfObjectMove_releaseVoiceHandle(&resultHandle->voice);
        }
    }

    // Select a voice ID from the appropriate phase table based on field_0x24.
    // h declared once ahead of the branch so its web claims a fresh saved
    // register (retail r31) rather than reusing the dead iterator register.
    CVoiceHandle* h;
    CCharVoice* voicePtr;
    int voiceId;
    if (self->field_0x24 >= 3) {
        // Late phase: handle + voice IDs from lbl_eu_80662D40.
        h = self->field_0x20;
        voicePtr = (CCharVoice*)h;
        if (self->field_0x20 != NULL) {
            voicePtr = &h->voice;
        }
        voiceId = lbl_eu_80662D40[ml::math::mtRand(3)];
        if (TryRequestVoice(self, voicePtr, voiceId) == 0) {
            // Play failed -- fire the advance/process virtual (vtable offset
            // 8 = PlayThreadVoice), not the size-getter blank1 (offset 0x10).
            self->PlayThreadVoice();
        }
    } else {
        // Early phase: handle + voice IDs from lbl_eu_80662D48.
        h = self->field_0x20;
        voicePtr = (CCharVoice*)h;
        if (self->field_0x20 != NULL) {
            voicePtr = &h->voice;
        }
        voiceId = lbl_eu_80662D48[ml::math::mtRand(3)];
        if (TryRequestVoice(self, voicePtr, voiceId) == 0) {
            // Play failed -- fire the advance/process virtual (vtable offset
            // 8 = PlayThreadVoice), not the size-getter blank1 (offset 0x10).
            self->PlayThreadVoice();
        }
    }
}


// us-802abc94 (VisBreak_TryRunSlotAction)
// Completion callback: if no active voice is playing, invoke the playback
// virtual (CVS_THREAD::PlayThreadVoice, vtable slot 1).
void VisBreak_TryRunSlotAction(CVS_THREAD_VISION_BREAK* self) {
    if (IsThreadVoiceActive(self) == 0) {
        self->PlayThreadVoice();
    }
}



// us-802abcdc (VisBreak_RemoveVoiceSlot)
// Remove a voice from the field_0x20 slot by matching its embedded CCharVoice.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void VisBreak_RemoveVoiceSlot(CVS_THREAD_VISION_BREAK* self, CCharVoice* voicePtr) {
    // Direct call to the free function at 0x802A3BEC.
    DetachThreadVoice(self, voicePtr);

    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* biased = (CCharVoice*)handle;
    if (handle != NULL) {
        biased = &handle->voice;
    }
    if (biased == voicePtr) {
        self->field_0x20 = NULL;
    }
}


// us-802abd3c (VisBreak_AllocVoiceHandle)
// Standalone voice selector for vision-break. Finds a free voice handle via
// HpVoice_FindBySubState(1), allocates a 0x32-byte buffer, and plays a voice ID chosen
// based on the current game-manager frame count. Always returns 0.
extern "C" int VisBreak_AllocVoiceHandle() {
    CVoiceHandle* handle = HpVoice_FindBySubState(1);
    if (handle == NULL) {
        return 0;
    }

    // Allocate a 0x32-byte playback buffer.
    if (CCharVoiceMan_DispatchById(0x32, 1) == NULL) {
        return 0;
    }

    // Select a voice ID based on the game-manager frame counter.
    // Retail calls getQueuedFileEventCount() for EACH branch condition (not once).
    // Cast to u32 ensures unsigned compare (cmpli) matches retail.
    int voiceId;
    if ((u32)cf::CfGameManager::getQueuedFileEventCount() < 0x38u) {
        // Early phase: voice IDs 0x65-0x66.
        voiceId = ml::math::mtRand(2) + 0x65;
    } else if ((u32)cf::CfGameManager::getQueuedFileEventCount() < 0x62u) {
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

    RequestVoicePlayback(voicePtr, voiceId, 0x32);
    return 0;
}
// Buffer-size getter recorded under the flat retail map name VisBreak_GetSizeConstF:
// returns 0xF (the thread's buffer size). Retail body: li r3,0xF; blr.
u32 VisBreak_GetSizeConstF(CVS_THREAD_VISION_BREAK* self) { return 0xF; }

// Retail section reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_VISION_BREAK.txt): the .rodata typeinfo
// name (sized to absorb the retail pad), two .data init-state triples
// ({0, -1, callback}), the 7-word thread vtable ({typeinfo, 0, play,
// remove, sizeConstF, alwaysTrue, nullDispatch}), the trailing
// typeinfo-base word plus padding, and three .sdata objects: two voice-ID
// quads the code reads via @sda21 and the typeinfo object ({name,
// trailing}). Global-scope data symbols: MWCC does not mangle them.
// __RTTI__Q22cf10CVS_THREAD lives in the CCharVoiceMan range and is referenced, not
// defined, here.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern u32 __RTTI__Q22cf10CVS_THREAD[2];
extern u32 __RTTI__Q22cf23CVS_THREAD_VISION_BREAK[2];

extern const char lbl_eu_80510838[0x20] = "cf::CVS_THREAD_VISION_BREAK";

u32 lbl_eu_80539D70[3] = {
    0,
    0xFFFFFFFF,
    (u32)VisBreak_DispatchPlayback,
};

u32 lbl_eu_80539D7C[3] = {
    0,
    0xFFFFFFFF,
    (u32)VisBreak_TryRunSlotAction,
};

u32 lbl_eu_80539D88[7] = {
    (u32)__RTTI__Q22cf23CVS_THREAD_VISION_BREAK,
    0,
    (u32)PlayThreadVoice,
    (u32)VisBreak_RemoveVoiceSlot,
    (u32)VisBreak_GetSizeConstF,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)CCharVoiceMan_NullVoiceDispatch,
};

u32 lbl_eu_80539DA4[3] = {
    (u32)__RTTI__Q22cf10CVS_THREAD,
    0,
    0,
};

__declspec(section ".sdata") extern s16 lbl_eu_80662D40[4] = {
    0x192,
    0x195,
    0x196,
    0,
};

__declspec(section ".sdata") extern s16 lbl_eu_80662D48[4] = {
    0x191,
    0x193,
    0x194,
    0,
};

__declspec(section ".sdata") extern u32 __RTTI__Q22cf23CVS_THREAD_VISION_BREAK[2] = {
    (u32)lbl_eu_80510838,
    (u32)lbl_eu_80539DA4,
};
