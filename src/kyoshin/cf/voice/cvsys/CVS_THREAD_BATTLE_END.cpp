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
    CCharVoice* vp;
    CVoiceHandle* handle;
    for (int i = 0; i < self->field_0x34; i++) {
        handle = self->field_0x20[i];
        vp = (CCharVoice*)handle;
        if (handle != NULL) {
            vp = &handle->voice;
        }
        if (vp == voicePtr) {
            self->field_0x20[i] = NULL;
        }
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
        const u32* p = lbl_eu_8053997C;
        v0 = *p++;
        self->unk4 = *p++;
        self->unk0 = (u32*)v0;
        self->unk8 = *p;
    } else {
        // Advance/process virtual (vtable offset 8 = func_802A3B50), not the
        // size-getter blank1 (offset 0x10).
        self->func_802A3B50();
    }
}

// us-802a70b0 (func_802A497C)
// (Re)initialise the thread: seed the slot array, pick a random starting
// slot, and set the active flag based on whether any listed voice is live.
void func_802A497C(CVS_THREAD_BATTLE_END* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    u32 v0;
    const u32* p = lbl_eu_80539964;
    v0 = *p++;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    int cnt = func_802A7870(self->field_0x20, 3, 0);
    self->field_0x34 = cnt;
    if (cnt <= 0) {
        self->func_802A3B50();
        return;
    }

    int rnd = ml::math::mtRand(cnt);
    self->field_0x30 = rnd;
    self->field_0x2c = rnd;

    if (self->field_0x3a != 0) {
        self->field_0x39 = self->field_0x3a;
        return;
    }
    // No forced direction: scan the manager's voice list for a live one.
    {
        ItemListManager* mgr = func_800B6BA4();
        ItemListNode* node = mgr->sentinel->next;
        while (node != mgr->sentinel) {
            u8* obj = node->object;
            CVoiceHandle* handle = (CVoiceHandle*)obj;
            if (obj != NULL) {
                handle = (CVoiceHandle*)((u8*)obj - 0x3E9C);
            }
            if (((int (*)(CVoiceHandle*))handle->vtable[0x2BC / 4])(handle) != 0) {
                self->field_0x39 = 1;
                return;
            }
            node = node->next;
        }
    }
    self->field_0x39 = 0;
}

// us-802a6fa4 (__ct__802A4870)
// Factory/constructor.  Allocates a handle buffer (discarded) and the object,
// runs the base constructor, sets the subclass vtable, then initialises the
// slot array and state flags (including a random scan direction).
CVS_THREAD_BATTLE_END* __ct__802A4870(int owner) try {
    CVoiceHandle* handleBuf = func_802A330C(0x46, 1);
    if (handleBuf == NULL) return NULL;

    CVS_THREAD_BATTLE_END* self = (CVS_THREAD_BATTLE_END*)func_802A34E4(0x3c);
    if (self == NULL) return NULL;

    __ct__cf_CVS_THREAD();
    ((void**)self)[0x1C / 4] = (void*)lbl_eu_805399B8;
    self->field_0x3a = (u8)owner;
    memset(self->field_0x20, 0, 0xc);
    self->field_0x2c = 0;
    self->field_0x30 = 0;
    self->field_0x34 = 0;
    self->field_0x39 = 0;
    self->field_0x38 = (u8)(ml::math::mtRand(2) != 0);

    const u32* base = lbl_eu_80539958;
    self->unk0 = (u32*)base[0];
    self->unk4 = base[1];
    self->unk8 = base[2];
    return self;
} catch (...) {
    throw;
}

// us-802a71ac (func_802A4A78)
// Play/advance the battle-end voice.  When the counter is not at the limit,
// pick a random ending line; otherwise gate on the thread sub-state (via
// func_802A77E8 mode) and play the appropriate variant ID.
void func_802A4A78(CVS_THREAD_BATTLE_END* self) {
    u32* base = (u32*)lbl_eu_80539970;
    u32 v0 = base[0];
    self->unk4 = base[1];
    self->unk0 = (u32*)v0;
    self->unk8 = base[2];

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
                int x = ml::math::mtRand(100);
                if (x >= 0x32) voiceId = 0x5de;
                else voiceId = 0x5dd;
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