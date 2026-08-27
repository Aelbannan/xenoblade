// CVS_THREAD_BATTLE_END: voice thread for battle-end audio sequences.
// A voice-slot state machine that cycles through character-voice lines.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"
#include <cstring>

// us-802a74fc (func_802A4DC8)
// Remove a released voice from the slot array by matching the embedded
// CCharVoice pointer against the one being freed.
void func_802A4DC8(CVS_THREAD_BATTLE_END* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);
    CVoiceHandle* handle;
    CCharVoice* vp;
    CVS_THREAD_BATTLE_END* p = self;
    int i;
    for (i = 0; i < self->field_0x34; i++) {
        handle = p->field_0x20[0];
        vp = (CCharVoice*)handle;
        if (handle != NULL) {
            vp = &handle->voice;
        }
        if (vp == voicePtr) {
            p->field_0x20[0] = NULL;
        }
        // Walk one word per iteration (reproduces retail's cursor register).
        p = (CVS_THREAD_BATTLE_END*)((u8*)p + 4);
    }
}

// us-802a7438 (func_802A4D04)
// Advance the active-slot counter (up or down depending on field_0x38).
// When the counter reaches field_0x2c, fire the end virtual; otherwise the
// init triple is re-copied into the base fields.
void func_802A4D04(CVS_THREAD_BATTLE_END* self) {
    if (func_802A3E88(self) != 0) return;

    if (self->field_0x38 == 0) {
        // forward direction: bump the counter, wrap to 0 at the limit
        int old = self->field_0x30;
        int cnt = self->field_0x34;
        int newv = old + 1;
        self->field_0x30 = newv;
        if (cnt <= newv) self->field_0x30 = 0;
    } else {
        // reverse direction: decrement, wrap to limit-1 on underflow
        if (--self->field_0x30 < 0) self->field_0x30 = self->field_0x34 - 1;
    }

    if (self->field_0x30 != self->field_0x2c) {
        // Pointer increment reproduces the lwzu/spread load-with-update.
        // v0 declared first so MWCC colours the lwzu destination into r3
        // and the base pointer into r4, matching retail.
        u32 v0;
        CVS_THREAD_HEAD_VIEW* head = (CVS_THREAD_HEAD_VIEW*)self;
        const u32* p = lbl_eu_8053997C;
        v0 = *p++;
        head->word4 = *p++;
        head->word0 = v0;
        head->word8 = *p;
    } else {
        // Advance/process virtual (vtable offset 8 = func_802A3B50), not the
        // size-getter blank1 (offset 0x10).
        self->func_802A3B50();
    }
}

// Vtable shape for the CVoiceHandle polymorphic voice objects; method at
// offset 0x2BC reports whether the voice is currently playing.
struct VoiceVtbl {
    u8 pad[0x2BC];
    int (*isPlaying)(CVoiceHandle*);
};

// us-802a70b0 (func_802A497C)
// (Re)initialise the thread: seed the slot array, pick a random starting
// slot, and set the active flag based on whether any listed voice is live.
void func_802A497C(CVS_THREAD_BATTLE_END* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    u32 v0;
    CVS_THREAD_HEAD_VIEW* head = (CVS_THREAD_HEAD_VIEW*)self;
    const u32* p = lbl_eu_80539964;
    v0 = *p++;
    head->word4 = *p++;
    head->word0 = v0;
    head->word8 = *p;

    int cnt = func_802A7870(self->field_0x20, 3, 0);
    self->field_0x34 = cnt;
    if (cnt <= 0) {
        self->func_802A3B50();
        return;
    }

    int rnd = ml::math::mtRand(cnt);
    self->field_0x30 = rnd;
    self->field_0x2c = rnd;

    // Flag value never crosses a call site (constants assigned after the
    // calls), so MWCC keeps it in r0 and converges both paths onto one stb.
    // Scan written as the fall-through of a ==0 test to match retail layout.
    u8 live = self->field_0x3a;
    if (live == 0) {
        // No forced direction: scan the manager's voice list for a live one.
        ItemListManager* mgr = getListB28();
        ItemListNode* node = mgr->sentinel->next;
        while (node != mgr->sentinel) {
            // node->object points at the embedded CCharVoice (handle+0x3E9C);
            // recover the owning CVoiceHandle.
            CVoiceHandle* handle = (CVoiceHandle*)node->object;
            if (handle != NULL) {
                handle = (CVoiceHandle*)((u8*)handle - 0x3E9C);
            }
            // Vtable method at 0x2BC: "is voice playing".
            VoiceVtbl* vtbl = *(VoiceVtbl**)handle;
            if (vtbl->isPlaying(handle) != 0) {
                live = 1;
                goto store;
            }
            node = node->next;
        }
        live = 0;
    }
store:
    self->field_0x39 = live;
}

// us-802a6fa4 (__ct__802A4870)
// Placement factory for CVS_THREAD_BATTLE_END (retail keeps this as a free
// function, not an actual class __ct__).  Allocates a handle buffer (discarded) and the object,
// runs the base constructor, sets the subclass vtable, then initialises the
// slot array and state flags (including a random scan direction).
// The redundant `self != NULL` guard reproduces retail's re-test of the
// allocation result guarding the EH region; the catch rethrows via
// __throw(0,0,0) so MWCC elides the __end__catch epilogue.  The init-state
// triple is copied outside the try.
CVS_THREAD_BATTLE_END* __ct__802A4870(int owner) {
    // handleBuf is validated then discarded (retail leaks it here).
    if (func_802A330C(0x46, 1) == NULL) {
        return NULL;
    }

    CVS_THREAD_BATTLE_END* self = (CVS_THREAD_BATTLE_END*)func_802A34E4(0x3c);
    if (self == NULL) {
        return NULL;
    }

    if (self != NULL) {
        try {
            __ct__cf_CVS_THREAD(self);

            // Override the vtable at 0x1C with the derived one, then seed
            // the slot array and direction flag.
            ((CVS_THREAD_BATTLE_END_raw*)self)->vtable = lbl_eu_805399B8;
            self->field_0x3a = (u8)owner;
            memset(self->field_0x20, 0, 0xc);

            self->field_0x2c = 0;
            self->field_0x30 = 0;
            self->field_0x34 = 0;
            self->field_0x39 = 0;
            // Retail tests bit 0 only (rlwinm rotl-1 extract), not a full
            // zero comparison.
            self->field_0x38 = (u8)(ml::math::mtRand(2) & 1);
        } catch (...) {
            __throw(0, 0, 0);
        }
    }

    // Copy the init-state triple (outside try). Load base[1] before base[0]:
    // retail colors the first-loaded value r0 and the second r4, storing r4
    // to +0 and r0 to +4. Single pointer initialized directly from the
    // symbol keeps lis/addi materialized once (retail r5).
    u32 v1;
    u32* p0;
    CVS_THREAD_HEAD_VIEW* head = (CVS_THREAD_HEAD_VIEW*)self;
    v1 = (p0 = lbl_eu_80539958)[1];
    head->word0 = p0[0];
    head->word4 = v1;
    head->word8 = p0[2];
    return self;
}

// us-802a71ac (func_802A4A78)
// Play/advance the battle-end voice.  When the counter is not at the limit,
// pick a random ending line; otherwise gate on the thread sub-state (via
// func_802A77E8 mode) and play the appropriate variant ID.
void func_802A4A78(CVS_THREAD_BATTLE_END* self) {
    // Re-seed the state triple via the lwzu/spread load-with-update pattern
    // (v0 declared first so MWCC colours the lwzu destination low).
    u32 v0;
    CVS_THREAD_HEAD_VIEW* head = (CVS_THREAD_HEAD_VIEW*)self;
    const u32* p = lbl_eu_80539970;
    v0 = *p++;
    head->word4 = *p++;
    head->word0 = v0;
    head->word8 = *p;

    int idx = self->field_0x30;
    CVoiceHandle* handle = self->field_0x20[idx];
    if (handle == NULL) return;
    if (((int (*)(CVoiceHandle*))handle->vtable[0x2BC / 4])(handle) != 0) return;

    int voiceId;
    if (self->field_0x30 == self->field_0x2c) {
        // Counter at limit: choose a battle-end variant line by sub-state.
        CVoiceHandle* h = self->field_0x20[self->field_0x30];
        if (self->field_0x34 == 1 && func_802A77E8(h) == 1) {
            if (ml::math::mtRand(100) < 0x1e) {
                if (ml::math::mtRand(100) >= 0x32) voiceId = 0xb63;
                else voiceId = 0xb55;
            } else {
                voiceId = -1;
            }
        } else if (self->field_0x34 == 1 && func_802A77E8(h) == 3) {
            if (ml::math::mtRand(100) < 0x1e) {
                if (ml::math::mtRand(100) >= 0x32) voiceId = 0xb61;
                else voiceId = 0xb57;
            } else {
                voiceId = -1;
            }
        } else if (self->field_0x34 == 1 && func_802A77E8(h) == 7) {
            if (ml::math::mtRand(100) < 0x1e) {
                // Bool-to-int add reproduces retail's branchless subfc/subfe.
                voiceId = 0x5dd + (int)(ml::math::mtRand(100) >= 0x32);
            } else {
                voiceId = -1;
            }
        } else {
            if (self->field_0x39 != 0) voiceId = lbl_eu_80662CC8[ml::math::mtRand(2)];
            else voiceId = lbl_eu_80539988[ml::math::mtRand(7)];
        }
    } else {
        // Counter not at limit: simpler random pick.
        if (self->field_0x39 != 0) voiceId = lbl_eu_80662CD0[ml::math::mtRand(2)];
        else voiceId = lbl_eu_805399A8[ml::math::mtRand(4)];
    }

    CVoiceHandle* sl = self->field_0x20[self->field_0x30];
    CCharVoice* vp = (CCharVoice*)sl;
    if (sl != NULL) {
        vp = &sl->voice;
    }
    if (func_802A3C44(self, vp, voiceId) == 0) {
        self->blank1();
    }
}

// Virtual method: returns allocation size (0x46 bytes) for CVS_THREAD_BATTLE_END
// Caller passes this pointer in r3 which is unused by this override
int func_802A4E40() {
    return 70;
}