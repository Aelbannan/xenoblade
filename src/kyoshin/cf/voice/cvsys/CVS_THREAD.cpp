#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/Random.hpp"

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

void CVS_THREAD::func_802A3BEC(){

}

void func_802A3C44(){

}

// ── func_802A3D54 ─────────────────────────────────────────────────────────
// Voice playback dispatcher.  Receives a CCharVoice* that is embedded inside
// a CVoiceHandle at offset 0x3E9C.  The field at +0x64 is the handle's
// field_0x3F00 flag word; bit 2 selects an alternate volume curve.

int func_802A3D54(CCharVoice* voicePtr, int voiceId, int arg) {
    if (voiceId < 0) {
        return 0;
    }
    if (cf::CfGameManager::func_800829B8()) {
        return 0;
    }

    // Recover the containing CVoiceHandle to read field_0x3F00 flags.
    CVoiceHandle* handle = (CVoiceHandle*)((u8*)voicePtr - offsetof(CVoiceHandle, voice));
    bool useAltCurve = (handle->field_0x3F00 & 0x4) != 0;

    // Read a camera parameter from the global scene.
    UnkCamObj* camObj = func_8049603C(lbl_eu_80663E14);
    float camVal = camObj->field_0xC;

    float dist = lbl_eu_80668C88 - camVal;
    float f1 = lbl_eu_80662CB0 * dist;
    float f2 = useAltCurve ? lbl_eu_80662CB8 : lbl_eu_80662CB4;

    // voicePtr is also a CfObject* for this non-virtual call.
    ((cf::CfObject*)voicePtr)->func_800BE898(voiceId, (u32)arg, f1, f2);
    return 0;
}

int func_802A3E28(){
    return 0;
}

void func_802A3E88(){}

// ── func_802A3EF0 ─────────────────────────────────────────────────────────
// Voice-play entry point.  Checks the ready flag, then probes three
// voice-ID sources (func_802A3FD4 / func_802A4120 / func_802A4430) in
// sequence.  On success allocates a 0xA0-byte buffer and dispatches to
// func_802A3D54 with the selected voice ID.

int func_802A3EF0(CVoiceHandle* handle) {
    if (!(handle->field_0x3F00 & 0x2)) {
        return 0;
    }

    // Check if the handle is already active (vtable slot 0x2BC).
    typedef int (*IsActiveFunc)(CVoiceHandle*);
    IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
    if (isActive(handle) != 0) {
        return 0;
    }

    // Try three voice-ID sources in priority order.
    int voiceId = func_802A3FD4(handle);
    if (voiceId == -1) {
        voiceId = func_802A4120(handle);
    }
    if (voiceId == -1) {
        voiceId = func_802A4430(handle);
    }

    if (voiceId <= 0) {
        return 0;
    }

    // Reserve a 0xA0-byte voice buffer.
    if (func_802A330C(0xA0, 1) == NULL) {
        return 0;
    }

    // Bias to the embedded CCharVoice at +0x3E9C.
    CCharVoice* voicePtr = NULL;
    if (handle != NULL) {
        voicePtr = &handle->voice;
    }

    func_802A3D54(voicePtr, voiceId, 0xA0);
    return 0;
}
int func_802A3FD4(CVoiceHandle* handle) {
    return -1;
}
int func_802A4120(CVoiceHandle* handle) {
    return -1;
}
int func_802A4430(CVoiceHandle* handle) {
    return -1;
}
// ── func_802A4798 ─────────────────────────────────────────────────────────
// Battle-end voice play.  Validates the ready flag and active state,
// checks that the party-member voice data is in range [1,6], allocates
// a 0x122-byte buffer, picks a random voice ID (0xD49+D52), and plays.

int func_802A4798(CVoiceHandle* handle) {
    if (!(handle->field_0x3F00 & 0x2)) {
        return 0;
    }

    // Check if the handle is already active (vtable slot 0x2BC).
    typedef int (*IsActiveFunc)(CVoiceHandle*);
    IsActiveFunc isActive = (IsActiveFunc)handle->vtable[0x2BC / 4];
    if (isActive(handle) != 0) {
        return 0;
    }

    // Validate party-member voice data range.
    Field3F60Inner* inner = (Field3F60Inner*)handle->field_0x3F60;
    if (inner == NULL) {
        return 0;
    }
    Field3F60Inner2* inner2 = (Field3F60Inner2*)inner->field_0x08;
    if (inner2->field_0x18 < 1 || inner2->field_0x18 > 6) {
        return 0;
    }

    // Reserve a 0x122-byte voice buffer.
    if (func_802A330C(0x122, 1) == NULL) {
        return 0;
    }

    // Bias to the embedded CCharVoice at +0x3E9C.
    CCharVoice* voicePtr = NULL;
    if (handle != NULL) {
        voicePtr = &handle->voice;
    }

    int voiceId = ml::math::mtRand(3) + 0xD49;
    func_802A3D54(voicePtr, voiceId, 0x122);
    return 0;
}
