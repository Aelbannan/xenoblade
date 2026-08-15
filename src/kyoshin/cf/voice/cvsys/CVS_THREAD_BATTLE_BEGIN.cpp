// CVS_THREAD_BATTLE_BEGIN: voice thread for battle-start audio events.
// Minimal 30-byte thread buffer; walks a small slot array driving the
// battle-start voice lines.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"
#include <cstring>

// Instance-method pointer used for the vtable[0x108/4] size getter.
typedef int (*CVSProcFn)(CVoiceHandle*);

// ── Target 2: us-802b24f0 (__ct__802AFA80) ─────────────────────────────────
// Factory/constructor for the battle-begin voice thread.  Verifies the voice
// handle's embedded character matches player 0 (and the data source is
// ready), computes a phase count from the handle/data sizes (0, 1 or 2),
// then allocates and initialises a new CVS_THREAD_BATTLE_BEGIN object.
CVS_THREAD_BATTLE_BEGIN* __ct__802AFA80(CVoiceHandle* handle, CVoiceHandle* A) {
    CVoiceHandle* h = handle;
    // Bias the handle to its embedded character voice (or keep NULL).
    CCharVoice* biased = (CCharVoice*)h;
    if (h != NULL) {
        biased = &h->voice;
    }
    if (biased != (CCharVoice*)cf::CfGameManager::getPlayer(0)) {
        return NULL;
    }
    if (func_802AF388(A) != 0) {
        return NULL;
    }

    // Phase count from the size delta between the handle and the data source.
    // Retail re-computes both sizes for the second comparison (no caching).
    // MWCC evaluates comparison operands right-to-left, so the handle's call
    // is written as the right operand to land its +/-6 boundary between the
    // two virtual calls.
    s16 count;
    if (((CVoiceHandleSizeView*)A)->getSize() <= ((CVoiceHandleSizeView*)h)->getSize() - 6) {
        count = 0;
    } else {
        count = (s16)(2 - (((CVoiceHandleSizeView*)A)->getSize() < ((CVoiceHandleSizeView*)h)->getSize() + 6));
    }

    // Allocate the (discarded) handle buffer and the object itself.
    if (func_802A330C(0x1e, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_BATTLE_BEGIN* obj = (CVS_THREAD_BATTLE_BEGIN*)func_802A34E4(0x3c);
    if (obj == NULL) {
        return NULL;
    }

    // Construct the object (inside the exception handler, matching retail).
    try {
        __ct__cf_CVS_THREAD(obj);
        ((CVSThreadVtableView*)obj)->vtable = (u32*)lbl_eu_8053ACB8;
        obj->field_0x24 = A;
        obj->field_0x34 = count;
        memset(obj->field_0x28, 0, 0xc);
        obj->field_0x36 = 0;
        obj->field_0x38 = 0;
        obj->field_0x3a = (u8)(ml::math::mtRand(2) != 0);
    } catch (...) {
        throw;
    }

    // Final base-state triple (retail loads b,a then stores a,b, then c).
    const u32* fin = lbl_eu_8053AC88;
    u32 b = fin[1];
    u32 a = fin[0];
    obj->unk0 = (u32*)a;
    obj->unk4 = b;
    obj->unk8 = fin[2];
    return obj;
}

// ── Target 3: us-802b26c4 (func_802AFC54) ──────────────────────────────────
// Init: restore the base state triple, seed the slot array from the manager
// (capacity 3), then dispatch to the process virtual when the slot count is
// non-positive.
void func_802AFC54(CVS_THREAD_BATTLE_BEGIN* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    u32 v0;
    const u32* p = lbl_eu_8053AC94;
    v0 = *p++;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    int cnt = func_802A7870(&self->field_0x28[0], 3, 0);
    s16 scnt = (s16)cnt;
    self->field_0x38 = scnt;
    if (scnt > 0) return;
    self->func_802A3B50();
}

void func_802AFCC8(){}

// ── Target 5: us-802b2b5c (func_802B00EC) ──────────────────────────────────
// Advance the active-slot counter (up or down, per the direction flag).  When
// the counter wraps back to 0 the process virtual is dispatched again.
void func_802B00EC(CVS_THREAD_BATTLE_BEGIN* self) {
    if (func_802A3E88(self) != 0) return;

    u32 v0;
    const u32* p = lbl_eu_8053ACAC;
    v0 = *p++;
    self->unk4 = *p++;
    self->unk0 = (u32*)v0;
    self->unk8 = *p;

    if (self->field_0x3a == 0) {
        // forward direction: bump the counter, wrap to 0 at the limit
        int nv = self->field_0x36 + 1;
        self->field_0x36 = (s16)nv;
        if (self->field_0x38 <= (s16)nv) self->field_0x36 = 0;
    } else {
        // reverse direction: decrement, wrap to limit-1 on underflow
        s16 newv = (s16)(self->field_0x36 - 1);
        self->field_0x36 = newv;
        if (newv < 0) self->field_0x36 = (s16)(self->field_0x38 - 1);
    }

    if (self->field_0x36 != 0) return;
    self->func_802A3B50();
}

// ── Target 4: us-802b2c20 (func_802B01B0) ──────────────────────────────────
// Remove a released voice from the slot array (and the dedicated slot) by
// matching the embedded CCharVoice pointer against the one being freed.
void func_802B01B0(CVS_THREAD_BATTLE_BEGIN* self, CCharVoice* voicePtr) {
    func_802A3BEC(self, voicePtr);
    CVoiceHandle* handle;
    CCharVoice* vp;
    // Word-walk over the object so the slot loads fold the 0x28 field offset
    // (retail walks with base = self, load offset 0x28, step 4).
    u32* w = (u32*)self;
    int i = 0;
    while (i < self->field_0x38) {
        handle = *(CVoiceHandle**)(w + 0xA);
        vp = (CCharVoice*)handle;
        if (handle != NULL) {
            vp = &handle->voice;
        }
        if (vp == voicePtr) {
            *(CVoiceHandle**)(w + 0xA) = NULL;
        }
        w++;
        i++;
    }
    handle = self->field_0x24;
    vp = (CCharVoice*)handle;
    if (handle != NULL) {
        vp = &handle->voice;
    }
    if (vp == voicePtr) {
        self->field_0x24 = NULL;
    }
}

// ── Target 1: us-802b2cb8 (func_802B0248) ──────────────────────────────────
// Select a battle-start voice ID from the current handle playback state.
// Dispatches on func_802A77E8(handle); the first arg (self) is unused.
int func_802B0248(CVS_THREAD_BATTLE_BEGIN* self, CVoiceHandle* handle) {
    CVSHandleFieldView* hf = (CVSHandleFieldView*)handle;
    int r;
    switch (func_802A77E8(handle)) {
    case 1: {
        int x = (int)hf->field_0x3374;
        if ((x & 0x4000) != 0) {
            r = 1;
        } else {
            r = -(x >> 15 & 1) & 2;
        }
        break;
    }
    case 2:
        r = 3;
        break;
    case 3: {
        int y = hf->field_0x3F28;
        r = (((y - 8) == 0) ? 1 : 0) + 4;
        break;
    }
    case 4: {
        int x = (int)hf->field_0x3374;
        if ((x & 0x4000) != 0) {
            r = 7;
        } else {
            r = 6;
            if ((x & 0x8000) != 0) {
                r = 8;
            }
        }
        break;
    }
    case 5:
        r = 9;
        break;
    case 6:
        r = 10;
        break;
    case 7:
        r = 11;
        break;
    default:
        r = -1;
        break;
    }
    return r;
}

// Virtual override of blank1(): returns buffer size for BATTLE_BEGIN thread.
int CVS_THREAD_BATTLE_BEGIN::blank1() {
    return BUFFER_SIZE;
}

// ── Target 2: us-802b2db4 (func_802B0344) ──────────────────────────────────
// Play a battle-start line: when the manager flags are active, compute the
// embedded voice pointer and play the 0x1E-byte voice for the current ID.
int func_802B0344(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 0x04)) return 0;
    CVoiceHandle* h = self;
    CCharVoice* vp = (CCharVoice*)h;
    if (h != NULL) vp = &h->voice;
    func_802A3D54(vp, func_802AED0C(), 0x1e);
    return 0;
}

// ── Target 1: us-802b2e14 (func_802B03A4) ──────────────────────────────────
// Bit test on the shared voice-ID helper: report whether its second-lowest
// bit is set.
int func_802B03A4() {
    int x = func_802AED0C();
    return (int)((((u32)(-x) & (u32)(~x)) >> 31) & 1);
}