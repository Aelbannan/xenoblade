// CVS_THREAD_CHAIN: voice thread chain node.
// Six functions: busy-check clear, voice removal, voice play, chain actor
// voice trigger, factory constructor, and chain voice lifetime handler.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_CHAIN.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"

// Raw layout exposing the implicit vtable pointer at 0x1C so the factory
// can override it with the derived vtable (same idiom as CVS_THREAD_BATTLE_END).
struct CVS_THREAD_CHAIN_VtblView {
    u32 pad[7];   // 0x00..0x1B CVS_THREAD base words
    u32* vtable;  // 0x1C
};

// us-802a802c (chainVoiceClearIdleSlot)
// If the thread reports no active voice, clear the voice handle slot.
void chainVoiceClearIdleSlot(cf::CVS_THREAD_CHAIN* self) {
    if (IsThreadVoiceActive(self) == 0) {
        self->field_0x20 = nullptr;
    }
}

// us-802a8068 (chainVoiceRemoveByPtr)
// Remove a voice by matching its embedded CCharVoice against the slot.
// A CVoiceHandle stores the CCharVoice at offset 0x3E9C, so a non-null handle
// is biased by 0x3E9C before comparing against the incoming voice pointer.
void chainVoiceRemoveByPtr(cf::CVS_THREAD_CHAIN* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);

    CVoiceHandle* handle = self->field_0x20;
    CCharVoice* embedded = (CCharVoice*)handle;
    if (handle != nullptr) {
        embedded = &handle->voice;
    }
    if (embedded == voicePtr) {
        self->field_0x20 = nullptr;
    }
}

// us-802a80c0 (chainVoicePlayById)
// Prepare/play a voice: run the completion check (and playback-start helper
// if busy), store the inverse-biased handle into the slot, re-bias to the
// embedded CCharVoice and play the given voice ID.
int chainVoicePlayById(cf::CVS_THREAD_CHAIN* self, CCharVoice* voicePtr, int voiceId) {
    if (IsThreadVoiceActive(self) != 0) {
        StopThreadVoice(self);
    }

    // Convert CCharVoice* to the containing CVoiceHandle*.
    CVoiceHandle* handle = (CVoiceHandle*)voicePtr;
    if (voicePtr != nullptr) {
        handle = (CVoiceHandle*)((char*)voicePtr - 0x3E9C);
    }
    self->field_0x20 = handle;

    // Convert back to the embedded CCharVoice for the play call.
    CCharVoice* embedded = (CCharVoice*)handle;
    if (handle != nullptr) {
        embedded = &handle->voice;
    }
    return TryRequestVoice(self, embedded, voiceId);
}


// us-802a7f64 (__ct__802A5830)
// Factory for CVS_THREAD_CHAIN. Allocates the handle buffer ((1,0) - the
// handle is discarded), then the 0x24-byte thread object, constructs the
// base, sets vtable/owner fields and copies init data from lbl_eu_80539A30.
extern "C" cf::CVS_THREAD_CHAIN* __ct__802A5830() {
    CVoiceHandle* handleBuf = CCharVoiceMan_DispatchById(1, 0);
    if (handleBuf == nullptr) {
        return nullptr;
    }
    cf::CVS_THREAD_CHAIN* self = (cf::CVS_THREAD_CHAIN*)CCharVoiceMan_AllocVoiceArena(0x24);
    if (self == nullptr) {
        return nullptr;
    }

    // Construct the base (can throw -> EH guard), then set vtable + link slot.
    // The redundant `self != nullptr` re-check reproduces retail's `beq` guard on
    // the EH region (CR0 from the earlier cmpwi is reused). The catch rethrows
    // via the runtime __throw(0,0,0) (retail `li r3,0; li r4,0; li r5,0; bl
    // __throw`).
    if (self != nullptr) {
        try {
            __ct__cf_CVS_THREAD();

            // Set the vtable at offset 0x1C (right after the 7 CVS_THREAD base words).
            ((CVS_THREAD_CHAIN_VtblView*)self)->vtable = lbl_eu_80539A3C;
            self->field_0x20 = nullptr;
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple (bytes 0x00-0x08) as the base's ptmf
    // member (VoiceCb is 12 bytes; DOWN precedent: member-typed access
    // colors the pair r0/r4 with the single r5 base, matching retail).
    ((CVS_THREAD*)self)->unk0 = ((CVS_THREAD*)lbl_eu_80539A30)->unk0;

    return self;
}



// us-802a81dc (chainVoiceAlwaysOne)
// Always-one voice predicate (retail 0x8 bytes: li r3,1; blr). Flat C
// linkage keeps the emitted symbol unmangled like retail (FAINT's
// FaintVoice_GetBufSize precedent); referenced from the thread vtable
// (lbl_eu_80539A3C slot 4). The same-named inline member in the header
// still satisfies the class interface.
extern "C" int chainVoiceAlwaysOne() {
    return 1;
}



// us-802a8148 (chainVoiceLifetimeHandler)
// Chain voice lifetime handler. Validates the factory-active flag, that the
// voice is not already active (vtable[0x2BC/4]), and that the previous sound
// is not the chain-stop marker; then reserves a 0x12C buffer and plays either
// a random chain voice (flag==0) or a fixed chain voice (flag!=0). Returns 0.
extern "C" int chainVoiceLifetimeHandler(CVoiceHandle* self, int flag) {
    if ((self->field_0x3F00 & 2) == 0) {
        return 0;
    }
    // Idle-check via the handle's vtable slot at 0x2BC (real r12 chain).
    if (((CVoiceChainVTV*)self)->isActive() != 0) {
        return 0;
    }
    if (func_800BE8F4(&self->voice) == 0x12C) {
        return 0;
    }
    if (CCharVoiceMan_DispatchById(0x12C, 1) == nullptr) {
        return 0;
    }
    if (flag == 0) {
        CCharVoice* embedded = (CCharVoice*)self;
        if (self != nullptr) {
            embedded = &self->voice;
        }
        RequestVoicePlayback(embedded, ml::math::mtRand(2) + 0x2BD, 0x12C);
    } else {
        CCharVoice* embedded = (CCharVoice*)self;
        if (self != nullptr) {
            embedded = &self->voice;
        }
        RequestVoicePlayback(embedded, 0x2BF, 0x12C);
    }
    return 0;
}



// us-802a8238 (chainVoiceActorTrigger)
// Chain actor voice trigger. The actor handle must have its factory-active
// flag (bit 1 of the 0x3F00 manager word) set; a thread buffer (0xA) is
// reserved, then a random chain voice (mtRand(2) + 0x321) is played. Always
// returns 0.
extern "C" int chainVoiceActorTrigger(CVoiceHandle* self, int flag) {
    if ((self->field_0x3F00 & 2) == 0) {
        return 0;
    }
    if (CCharVoiceMan_DispatchById(0xA, 1) == nullptr) {
        return 0;
    }
    CCharVoice* embedded = (CCharVoice*)self;
    if (self != nullptr) {
        embedded = &self->voice;
    }
    RequestVoicePlayback(embedded, ml::math::mtRand(2) + 0x321, 0xA);
    return 0;
}


// Retail .data reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_CHAIN.txt): the init-state triple the
// factory copies into the base words 0x00-0x08 ({0, -1, callback}), the
// 7-word thread vtable ({typeinfo, 0, play, remove, alwaysOne, alwaysTrue,
// playById}), and the trailing typeinfo-base word plus padding. Global-
// scope data symbols: MWCC does not mangle them, so these definitions
// carry the retail linker names.
//
// External words (out-of-unit retail labels, referenced not defined):
// __RTTI__Q22cf16CVS_THREAD_CHAIN is the CHAIN typeinfo object defined by DOWN's .sdata,
// __RTTI__Q22cf10CVS_THREAD is the base typeinfo word in CCharVoiceMan's .sdata.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern u32 __RTTI__Q22cf16CVS_THREAD_CHAIN[2];
extern u32 __RTTI__Q22cf10CVS_THREAD[2];

u32 lbl_eu_80539A30[3] = {
    0,
    0xFFFFFFFF,
    (u32)chainVoiceClearIdleSlot,
};

u32 lbl_eu_80539A3C[7] = {
    (u32)__RTTI__Q22cf16CVS_THREAD_CHAIN,
    0,
    (u32)PlayThreadVoice,
    (u32)chainVoiceRemoveByPtr,
    (u32)chainVoiceAlwaysOne,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)chainVoicePlayById,
};

u32 lbl_eu_80539A58[4] = {
    (u32)__RTTI__Q22cf10CVS_THREAD,
    0,
    0,
    0,
};

