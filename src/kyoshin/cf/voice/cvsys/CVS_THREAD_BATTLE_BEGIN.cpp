// CVS_THREAD_BATTLE_BEGIN: voice thread for battle-start audio events.
// Minimal 30-byte thread buffer; walks a small slot array driving the
// battle-start voice lines.

#include "kyoshin/cf/voice/cvsys/CVS_THREAD_BATTLE_BEGIN.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"
#include <cstring>

// us-802b24f0 (__ct__802AFA80)
// Factory/constructor for the battle-begin voice thread.  Verifies the voice
// handle's embedded character matches player 0 (and the data source is
// ready), computes a phase count from the handle/data sizes (0, 1 or 2),
// then allocates and initialises a new CVS_THREAD_BATTLE_BEGIN object.
extern "C" CVS_THREAD_BATTLE_BEGIN* __ct__802AFA80(CVoiceHandle* handle, CVoiceHandle* A) {
    CVoiceHandle* h = handle;
    // Bias the handle to its embedded character voice (or keep NULL).
    CCharVoice* biased = (CCharVoice*)h;
    if (h != NULL) {
        biased = &h->voice;
    }
    if (biased != (CCharVoice*)cf::CfGameManager::getPlayer(0)) {
        return NULL;
    }
    if (BmVoice_HasEntry(A) != 0) {
        return NULL;
    }

    // Phase count from the size delta between the handle and the data source.
    // Retail re-computes both sizes for the second comparison (no caching).
    // MWCC evaluates comparison operands right-to-left, so the handle's call
    // is written as the right operand to land its +/-6 boundary between the
    // two virtual calls.
    int count;
    if (((CVoiceHandleSizeView*)A)->getSize() <= ((CVoiceHandleSizeView*)h)->getSize() - 6)
        count = 0;
    else
        count = (int)(((CVoiceHandleSizeView*)A)->getSize() < ((CVoiceHandleSizeView*)h)->getSize() + 6 ? 1 : 2);

    // Allocate the (discarded) handle buffer and the object itself.
    if (CCharVoiceMan_DispatchById(0x1e, 1) == NULL) {
        return NULL;
    }
    CVS_THREAD_BATTLE_BEGIN* obj = (CVS_THREAD_BATTLE_BEGIN*)CCharVoiceMan_AllocVoiceArena(0x3c);
    if (obj == NULL) {
        return NULL;
    }

    // Construct the object (inside the exception handler, matching retail).
    // The redundant `obj != NULL` re-check reproduces retail's `beq` guard on
    // the EH region (CHAIN precedent); the catch rethrows via the runtime
    // __throw(0,0,0) so MWCC elides the __end__catch epilogue.
    if (obj != NULL) {
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
        __throw(0, 0, 0);
    }
    }

    // Final base-state triple:
    // MWCC emits the retail load-descending/store-ascending word-move
    // pattern (DOWN precedent).
    ((CVS_THREAD*)obj)->unk0 = ((CVS_THREAD*)lbl_eu_8053AC88)->unk0;
    return obj;
}

// us-802b26c4 (BtlBeg_InitSlots_FC54)
// Init: restore the base state triple, seed the slot array from the manager
// (capacity 3), then dispatch to the process virtual when the slot count is
// non-positive.
void BtlBeg_InitSlots_FC54(CVS_THREAD_BATTLE_BEGIN* self) {
    // Restore the base state triple via the lwzu/spread load-with-update
    // pattern (v0 declared first so the lwzu destination colours low).
    u32 v0;
    const u32* p = lbl_eu_8053AC94;
    v0 = *p++;
    ((CVSThreadStateTriple*)self)->b = *p++;
    ((CVSThreadStateTriple*)self)->a = v0;
    ((CVSThreadStateTriple*)self)->c = *p;

    int cnt = HpVoice_CollectIdle(&self->field_0x28[0], 3, 0);
    s16 scnt = (s16)cnt;
    self->field_0x38 = scnt;
    if (scnt > 0) return;
    self->PlayThreadVoice();
}

void BtlBeg_ReinitSlots_FCC8(){}

// us-802b2b5c (BtlBeg_AdvanceSlot_00EC)
// Advance the active-slot counter (up or down, per the direction flag).  When
// the counter wraps back to 0 the process virtual is dispatched again.
void BtlBeg_AdvanceSlot_00EC(CVS_THREAD_BATTLE_BEGIN* self) {
    if (IsThreadVoiceActive(self) != 0) return;

    u32 v0;
    const u32* p = lbl_eu_8053ACAC;
    v0 = *p++;
    ((CVSThreadStateTriple*)self)->b = *p++;
    ((CVSThreadStateTriple*)self)->a = v0;
    ((CVSThreadStateTriple*)self)->c = *p;

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
    self->PlayThreadVoice();
}

// us-802b2c20 (BtlBeg_RemoveVoice_01B0)
// Remove a released voice from the slot array (and the dedicated slot) by
// matching the embedded CCharVoice pointer against the one being freed.
void BtlBeg_RemoveVoice_01B0(CVS_THREAD_BATTLE_BEGIN* self, CCharVoice* voicePtr) {
    DetachThreadVoice(self, voicePtr);
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

// us-802b2cb8 (BtlBeg_SelectVoiceId_0248)
// Select a battle-start voice ID from the current handle playback state.
// Dispatches on HpVoice_MapSubState(handle); the first arg (self) is unused.
int BtlBeg_SelectVoiceId_0248(CVS_THREAD_BATTLE_BEGIN* self, CVoiceHandle* handle) {
    CVSHandleFieldView* hf = (CVSHandleFieldView*)handle;
    int r;
    switch (HpVoice_MapSubState(handle)) {
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

// (No blank1() override defined here: the 0x1E const-return body is already
// provided as BtlBeg_PlayGate_033C below under its retail spelling, and
// defining a virtual would make MWCC emit a vtable in this TU (CVS_THREAD_HP
// precedent). The blank1() member decl in the header still satisfies the
// class interface; the thread vtable is the retail lbl_eu_8053ACB8 data.)

// us-802b2dac (BtlBeg_PlayGate_033C)
// Flat-named copy of the blank1 body: retail emits the 0x1E const-return
// (li r3,0x1E; blr, 0x8 bytes) under the flat map name and references it
// from the thread vtable (lbl_eu_8053ACB8 slot 4). C linkage keeps the
// emitted symbol unmangled like retail.
extern "C" int BtlBeg_PlayGate_033C() {
    return CVS_THREAD_BATTLE_BEGIN::BUFFER_SIZE;
}

// us-802b2db4 (BtlBeg_PlayLine_0344)
// Play a battle-start line: when the manager flags are active, compute the
// embedded voice pointer and play the 0x1E-byte voice for the current ID.
extern "C" int BtlBeg_PlayLine_0344(CVoiceHandle* self) {
    if (!(self->field_0x3F00 & 0x04)) return 0;
    CVoiceHandle* h = self;
    CCharVoice* vp = (CCharVoice*)h;
    if (h != NULL) vp = &h->voice;
    RequestVoicePlayback(vp, BmVoice_FindPoolA(), 0x1e);
    return 0;
}

// us-802b2e14 (BtlBeg_IsVoiceBit_03A4)
// Bit test on the shared voice-ID helper: report whether its second-lowest
// bit is set.
extern "C" int BtlBeg_IsVoiceBit_03A4() {
    int x = BmVoice_FindPoolA();
    return (int)((((u32)(-x) & (u32)(~x)) >> 31) & 1);
}

// Retail section reconstruction (measured in
// .scratch/dlabels/a2/CVS_THREAD_BATTLE_BEGIN.txt): three .rodata typeinfo
// names (BATTLE_BEGIN exact-fit; the two LIB names sized to absorb the
// retail pad), four .data init-state triples ({0, -1, callback}), the
// 8-word thread vtable ({typeinfo, 0, play, remove, playGate, alwaysTrue,
// nullDispatch, 0}), two trailing mixed pointer/constant blocks, and three
// .sdata typeinfo objects ({name, trailing-or-0}). Global-scope data
// symbols: MWCC does not mangle them.
//
// NOTE (frozen-rule residual): this TU carries a UNIT_RULES
// extern_data_sections=(".data", ".rodata", ".sdata") entry that
// zeroes these sections in the link-time reloc.o. The retail split object
// genuinely carries them (0x80/0x48/0x18, reconstructed byte-identical
// below), so that rule entry is stale and should be removed; until then
// the link keeps resolving these labels as UNDEF. lbl_eu_80662CC0 lives in
// the CCharVoiceMan range and is referenced, not defined, here.
extern "C" void PlayThreadVoice();
extern "C" bool CCharVoiceMan_AlwaysTrue();
extern "C" int CCharVoiceMan_NullVoiceDispatch(void* self);
extern u32 lbl_eu_80662CC0[2];
extern u32 lbl_eu_8053ACA0[3];
extern u32 lbl_eu_8053ACD8[8];
extern u32 lbl_eu_8053ACF8[4];
extern u32 lbl_eu_80662E10[2];
extern u32 lbl_eu_80662E18[2];
extern u32 lbl_eu_80662E20[2];

extern const char lbl_eu_805134A8[] = "cf::CVS_THREAD_BATTLE_BEGIN";
extern const char lbl_eu_805134C4[0x14] = "cf::EVS_THREAD_LIB";
extern const char lbl_eu_805134D8[0x18] = "cf::CVS_THREAD_LIB";

u32 lbl_eu_8053AC88[3] = {
    0,
    0xFFFFFFFF,
    (u32)BtlBeg_InitSlots_FC54,
};

u32 lbl_eu_8053AC94[3] = {
    0,
    0xFFFFFFFF,
    (u32)BtlBeg_ReinitSlots_FCC8,
};

u32 lbl_eu_8053ACA0[3] = {
    0,
    0xFFFFFFFF,
    (u32)BtlBeg_AdvanceSlot_00EC,
};

u32 lbl_eu_8053ACAC[3] = {
    0,
    0xFFFFFFFF,
    (u32)BtlBeg_ReinitSlots_FCC8,
};

u32 lbl_eu_8053ACB8[8] = {
    (u32)lbl_eu_80662E10,
    0,
    (u32)PlayThreadVoice,
    (u32)BtlBeg_RemoveVoice_01B0,
    (u32)BtlBeg_PlayGate_033C,
    (u32)CCharVoiceMan_AlwaysTrue,
    (u32)CCharVoiceMan_NullVoiceDispatch,
    0,
};

u32 lbl_eu_8053ACD8[8] = {
    (u32)lbl_eu_80662CC0,
    0,
    (u32)lbl_eu_80662E18,
    0x80000021,
    1,
    (u32)lbl_eu_80662E20,
    0x21,
    0,
};

u32 lbl_eu_8053ACF8[4] = {
    (u32)lbl_eu_80662E20,
    0,
    0,
    0,
};

__declspec(section ".sdata") extern u32 lbl_eu_80662E10[2] = {
    (u32)lbl_eu_805134A8,
    (u32)lbl_eu_8053ACD8,
};

__declspec(section ".sdata") extern u32 lbl_eu_80662E18[2] = {
    (u32)lbl_eu_805134C4,
    (u32)lbl_eu_8053ACF8,
};

__declspec(section ".sdata") extern u32 lbl_eu_80662E20[2] = {
    (u32)lbl_eu_805134D8,
    0,
};


