#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"

// ── Extern symbols ────────────────────────────────────────────────────────

extern "C" {
    extern u32 lbl_eu_80539910[];   // vtable for CVS_THREAD
    extern void func_800BE924(void* voice);
}

// ── func_802A3E74 ─────────────────────────────────────────────────────────
// If the thread has a non-null voice handle at unk10, stop it via the sound
// system call func_800BE924.

void func_802A3E74(CVS_THREAD* thread) {
    if (thread->unk10 != 0) {
        func_800BE924((void*)thread->unk10);
    }
}

// ── __ct__cf_CVS_THREAD (constructor) ─────────────────────────────────────
// Initialises a CVS_THREAD: stores the vtable at offset 0x1C, zeros unkC
// and unk10, calls func_802A35A0(this) to obtain an ID stored in unk18,
// and returns this.

extern "C" CVS_THREAD* __ct__cf_CVS_THREAD(CVS_THREAD* self) {
    // Store the vtable pointer at offset 0x1C.
    *(u32*)((u8*)self + 0x1C) = (u32)&lbl_eu_80539910;
    self->unkC = 0;
    self->unk10 = 0;
    self->unk18 = func_802A35A0((unsigned int)self);
    return self;
}

void func_802A3ACC(){

}

void CVS_THREAD::func_802A3B50(){

}

// ── func_802A3BEC (vmethod[1]) ────────────────────────────────────────────
// Checks whether the active voice handle at unk10 matches voicePtr.
// If so, stops the voice via func_800BE924 and clears both unk10
// and unk14.  The redundant inner voice != 0 guard mirrors the
// retail MWCC codegen pattern for the && short-circuit.

void CVS_THREAD::func_802A3BEC(CCharVoice* voicePtr) {
    u32 voice = unk10;
    if (voice != 0 && voice == (unsigned int)voicePtr) {
        if (voice != 0) {
            func_800BE924((void*)voice);
        }
        unk10 = 0;
        unk14 = -1;
    }
}

void func_802A3C44(){

}

void func_802A3D54(){
}

// ── func_802A3E28 ─────────────────────────────────────────────────────────
// Stops any active voice at unk10, then clears unk10 to 0 and sets
// unk14 to -1 unconditionally.

void CVS_THREAD::func_802A3E28() {
    if (unk10 != 0) {
        func_800BE924((void*)unk10);
    }
    unk10 = 0;
    unk14 = (u32)-1;
}

void func_802A3E88(){}
void func_802A3EF0(){}
void func_802A3FD4(){}
void func_802A4120(){}
void func_802A4430(){}
void func_802A4798(){}
