// CVS_THREAD_BATTLE_END: voice thread for battle-end audio sequences.
// A voice-slot state machine that cycles through character-voice lines.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_END.hpp"
#include "kyoshin/harness_catalog.hpp"
#include "monolib/math/Random.hpp"
#include <cstring>

// us-802a74fc (EndVoice_RemoveVoice)
// Remove a released voice from the slot array by matching the embedded
// CCharVoice pointer against the one being freed.
void EndVoice_RemoveVoice(CVS_THREAD_BATTLE_END* self, CCharVoice* voicePtr) {
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

// us-802a7438 (EndVoice_AdvanceSlot)
// Advance the active-slot counter (up or down depending on field_0x38).
// When the counter reaches field_0x2c, fire the end virtual; otherwise the
// init triple is re-copied into the base fields.
void EndVoice_AdvanceSlot(CVS_THREAD_BATTLE_END* self) {
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
// Cast-only virtual iface: real member call so MWCC emits lwz r12,0(r3)
// dispatch (a VoiceVtbl* FP temp colors the vptr load as r4). With -RTTI,
// declared virtual #k lands at byte (k+2)*4; 0x2BC/4 = 175 -> k = 173.
class VoicePlayingIf {
public:
    /* 173 unique fillers + isPlaying => vtable +0x2BC with RTTI. */
    virtual void vf000() = 0;
    virtual void vf001() = 0;
    virtual void vf002() = 0;
    virtual void vf003() = 0;
    virtual void vf004() = 0;
    virtual void vf005() = 0;
    virtual void vf006() = 0;
    virtual void vf007() = 0;
    virtual void vf008() = 0;
    virtual void vf009() = 0;
    virtual void vf010() = 0;
    virtual void vf011() = 0;
    virtual void vf012() = 0;
    virtual void vf013() = 0;
    virtual void vf014() = 0;
    virtual void vf015() = 0;
    virtual void vf016() = 0;
    virtual void vf017() = 0;
    virtual void vf018() = 0;
    virtual void vf019() = 0;
    virtual void vf020() = 0;
    virtual void vf021() = 0;
    virtual void vf022() = 0;
    virtual void vf023() = 0;
    virtual void vf024() = 0;
    virtual void vf025() = 0;
    virtual void vf026() = 0;
    virtual void vf027() = 0;
    virtual void vf028() = 0;
    virtual void vf029() = 0;
    virtual void vf030() = 0;
    virtual void vf031() = 0;
    virtual void vf032() = 0;
    virtual void vf033() = 0;
    virtual void vf034() = 0;
    virtual void vf035() = 0;
    virtual void vf036() = 0;
    virtual void vf037() = 0;
    virtual void vf038() = 0;
    virtual void vf039() = 0;
    virtual void vf040() = 0;
    virtual void vf041() = 0;
    virtual void vf042() = 0;
    virtual void vf043() = 0;
    virtual void vf044() = 0;
    virtual void vf045() = 0;
    virtual void vf046() = 0;
    virtual void vf047() = 0;
    virtual void vf048() = 0;
    virtual void vf049() = 0;
    virtual void vf050() = 0;
    virtual void vf051() = 0;
    virtual void vf052() = 0;
    virtual void vf053() = 0;
    virtual void vf054() = 0;
    virtual void vf055() = 0;
    virtual void vf056() = 0;
    virtual void vf057() = 0;
    virtual void vf058() = 0;
    virtual void vf059() = 0;
    virtual void vf060() = 0;
    virtual void vf061() = 0;
    virtual void vf062() = 0;
    virtual void vf063() = 0;
    virtual void vf064() = 0;
    virtual void vf065() = 0;
    virtual void vf066() = 0;
    virtual void vf067() = 0;
    virtual void vf068() = 0;
    virtual void vf069() = 0;
    virtual void vf070() = 0;
    virtual void vf071() = 0;
    virtual void vf072() = 0;
    virtual void vf073() = 0;
    virtual void vf074() = 0;
    virtual void vf075() = 0;
    virtual void vf076() = 0;
    virtual void vf077() = 0;
    virtual void vf078() = 0;
    virtual void vf079() = 0;
    virtual void vf080() = 0;
    virtual void vf081() = 0;
    virtual void vf082() = 0;
    virtual void vf083() = 0;
    virtual void vf084() = 0;
    virtual void vf085() = 0;
    virtual void vf086() = 0;
    virtual void vf087() = 0;
    virtual void vf088() = 0;
    virtual void vf089() = 0;
    virtual void vf090() = 0;
    virtual void vf091() = 0;
    virtual void vf092() = 0;
    virtual void vf093() = 0;
    virtual void vf094() = 0;
    virtual void vf095() = 0;
    virtual void vf096() = 0;
    virtual void vf097() = 0;
    virtual void vf098() = 0;
    virtual void vf099() = 0;
    virtual void vf100() = 0;
    virtual void vf101() = 0;
    virtual void vf102() = 0;
    virtual void vf103() = 0;
    virtual void vf104() = 0;
    virtual void vf105() = 0;
    virtual void vf106() = 0;
    virtual void vf107() = 0;
    virtual void vf108() = 0;
    virtual void vf109() = 0;
    virtual void vf110() = 0;
    virtual void vf111() = 0;
    virtual void vf112() = 0;
    virtual void vf113() = 0;
    virtual void vf114() = 0;
    virtual void vf115() = 0;
    virtual void vf116() = 0;
    virtual void vf117() = 0;
    virtual void vf118() = 0;
    virtual void vf119() = 0;
    virtual void vf120() = 0;
    virtual void vf121() = 0;
    virtual void vf122() = 0;
    virtual void vf123() = 0;
    virtual void vf124() = 0;
    virtual void vf125() = 0;
    virtual void vf126() = 0;
    virtual void vf127() = 0;
    virtual void vf128() = 0;
    virtual void vf129() = 0;
    virtual void vf130() = 0;
    virtual void vf131() = 0;
    virtual void vf132() = 0;
    virtual void vf133() = 0;
    virtual void vf134() = 0;
    virtual void vf135() = 0;
    virtual void vf136() = 0;
    virtual void vf137() = 0;
    virtual void vf138() = 0;
    virtual void vf139() = 0;
    virtual void vf140() = 0;
    virtual void vf141() = 0;
    virtual void vf142() = 0;
    virtual void vf143() = 0;
    virtual void vf144() = 0;
    virtual void vf145() = 0;
    virtual void vf146() = 0;
    virtual void vf147() = 0;
    virtual void vf148() = 0;
    virtual void vf149() = 0;
    virtual void vf150() = 0;
    virtual void vf151() = 0;
    virtual void vf152() = 0;
    virtual void vf153() = 0;
    virtual void vf154() = 0;
    virtual void vf155() = 0;
    virtual void vf156() = 0;
    virtual void vf157() = 0;
    virtual void vf158() = 0;
    virtual void vf159() = 0;
    virtual void vf160() = 0;
    virtual void vf161() = 0;
    virtual void vf162() = 0;
    virtual void vf163() = 0;
    virtual void vf164() = 0;
    virtual void vf165() = 0;
    virtual void vf166() = 0;
    virtual void vf167() = 0;
    virtual void vf168() = 0;
    virtual void vf169() = 0;
    virtual void vf170() = 0;
    virtual void vf171() = 0;
    virtual void vf172() = 0;
    virtual int isPlaying() = 0;
};;

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
            // Real virtual call -> lwz r12,0(r3) dispatch (not r4 FP temp).
            if (reinterpret_cast<VoicePlayingIf*>(handle)->isPlaying() != 0) {
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

    CVS_THREAD_BATTLE_END* self = (CVS_THREAD_BATTLE_END*)CCharVoiceMan_AllocVoiceArena(0x3c);
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
int EndVoice_AllocSize() {
    return 70;
}