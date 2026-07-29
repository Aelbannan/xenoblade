#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "kyoshin/cf/object/CfObject.hpp"
#include "monolib/math/Random.hpp"

// ── Extern symbols ────────────────────────────────────────────────────────

extern "C" {
    extern u32 lbl_eu_80539910[];   // vtable for CVS_THREAD
    extern void func_800BE924(void* voice);
}

// func_800BE898 is declared void in CfObject.hpp (incomplete stub).
// Retail returns int; declare the mangled symbol so we can check the return.
extern "C" int func_800BE898__Q22cf8CfObjectFiUlff(cf::CfObject* self, int, u32, float, float);

// External functions used by func_802A3FD4 / func_802A4120.
extern "C" {
void* func_800B708C(int r3);
void* func_800BFC68__FPQ22cf12CfObjectMove(void* obj);
}

// External functions used by func_802A4120.
extern "C" {
u32 func_800822F4__Q22cf13CfGameManagerFv();
void* func_8009EC9C(u16 index);
int func_800A32BC(void* data);
void* getPlayer__Q22cf13CfGameManagerFi(int index);
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

// ── func_802A3C44 ─────────────────────────────────────────────────────────
// Voice dispatch helper: checks a CfObject flag (bit 2 of unk64),
// computes camera-distance volume parameters, then calls
// CfObject::func_800BE898.  On success stores the object and param
// in the CVS_THREAD.

int func_802A3C44(CVS_THREAD* self, cf::CfObject* obj, int param) {
    if (param < 0) {
        return 0;
    }
    int bufSize = self->blank1();
    if (param < 0) {
        return 0;
    }
    if (cf::CfGameManager::func_800829B8()) {
        return 0;
    }

    int result;
    if (obj->unk64 & 4) {
        // Branch A: alt volume curve (bit 2 set).
        UnkCamObj* camObj = func_8049603C(lbl_eu_80663E14);
        result = func_800BE898__Q22cf8CfObjectFiUlff(obj, param, (u32)bufSize,
            lbl_eu_80662CB0 * (lbl_eu_80668C88 - camObj->field_0xC),
            lbl_eu_80662CB8);
    } else {
        // Branch B: default volume curve.
        UnkCamObj* camObj = func_8049603C(lbl_eu_80663E14);
        result = func_800BE898__Q22cf8CfObjectFiUlff(obj, param, (u32)bufSize,
            lbl_eu_80662CB0 * (lbl_eu_80668C88 - camObj->field_0xC),
            lbl_eu_80662CB4);
    }

    if (result != 0) {
        self->unk10 = (u32)obj;
        self->unk14 = param;
    }

    return result;
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
// ── func_802A3FD4 ─────────────────────────────────────────────────────────
// First voice-ID probe.  Calls through handle vtable[169] to reach
// voice-manager data, validates battle state with func_80146148/
// func_80146300, then queries owner state via func_802A77E8.
// Returns a voice ID (0x8FD-0x8FF) or -1 on failure.

int func_802A3FD4(CVoiceHandle* handle) {
    // Call vtable method at index 169 to get the voice object.
    UnkVoiceObj* voiceObj = ((UnkVoiceObj* (*)(CVoiceHandle*))handle->vtable[0x2A4 / 4])(handle);

    // Resolve the CfObjectMove owner through the voice-manager chain.
    void* mgrData = func_800B708C(voiceObj->field_0x04);
    void* owner = func_800BFC68__FPQ22cf12CfObjectMove(mgrData);
    if (owner == NULL) {
        return -1;
    }

    // Re-obtain the voice object (call is not CSE'd).
    voiceObj = ((UnkVoiceObj* (*)(CVoiceHandle*))handle->vtable[0x2A4 / 4])(handle);
    UnkVoiceSub* sub = (UnkVoiceSub*)voiceObj->field_0x50;

    // Validate battle voice flags via the sub-object.
    // Short-circuit OR: try func_80146148 first, then func_80146300.
    int ok;
    if (sub == NULL) {
        ok = 0;
    } else {
        ok = (func_80146148(sub->field_0x48) || func_80146300(sub->field_0x48, 1)) ? 1 : 0;
    }
    if (!ok) {
        return -1;
    }

    // Random gate: 25% chance to proceed.
    if (ml::math::mtRand(100) >= 25) {
        return -1;
    }

    // Compare owner state vs handle state to pick voice ID.
    int ownerState = func_802A77E8(handle);
    int handleState = func_802A77E8((CVoiceHandle*)owner);

    if (ownerState == 1 && handleState == 3) {
        return 0x8fd;
    }
    if (ownerState == 1 && handleState == 2) {
        return 0x8fe;
    }
    if (ownerState == 1 && handleState == 4) {
        return 0x8ff;
    }
    return -1;
}
// ── func_802A4120 ─────────────────────────────────────────────────────────
// Second voice-ID probe.  Reads party-member voice data from the handle's
// field_0x3F60 chain, adjusts for battle state and player identity, then
// computes a voice ID based on the index and game conditions.
// Returns a voice ID (0xD9D-0xDD8 range) or -1 on failure.

int func_802A4120(CVoiceHandle* handle) {
    // Resolve voice index from handle->field_0x3F60 → inner → field_0x18.
    int voiceIndex;
    void* chainData = handle->field_0x3F60;
    if (chainData == NULL) {
        voiceIndex = -1;
    } else {
        Field3F60Inner* inner = (Field3F60Inner*)chainData;
        Field3F60Inner2* inner2 = (Field3F60Inner2*)inner->field_0x08;
        voiceIndex = inner2->field_0x18;
    }

    // Voice index must be in [16, 31].
    if (voiceIndex < 16) {
        return -1;
    }
    if (voiceIndex > 31) {
        return -1;
    }

    // If owner state is 3 and party count < 42, adjust voiceIndex via
    // the battle sub-object.
    int flag = 0;
    if (func_802A77E8(handle) == 3) {
        if (func_800822F4__Q22cf13CfGameManagerFv() < 42) {
            flag = 1;
        }
    }
    if (flag) {
        UnkVoiceObj* voiceObj = ((UnkVoiceObj* (*)(CVoiceHandle*))handle->vtable[0x2A4 / 4])(handle);
        UnkVoiceSub* sub = (UnkVoiceSub*)voiceObj->field_0x50;
        if (sub == NULL) {
            return -1;
        }
        voiceIndex = sub->field_0x77 + 16;
    }

    // If owner state is 10, also adjust via sub-object.
    if (func_802A77E8(handle) == 10) {
        UnkVoiceObj* voiceObj = ((UnkVoiceObj* (*)(CVoiceHandle*))handle->vtable[0x2A4 / 4])(handle);
        UnkVoiceSub* sub = (UnkVoiceSub*)voiceObj->field_0x50;
        if (sub == NULL) {
            return -1;
        }
        voiceIndex = sub->field_0x77 + 16;
    }

    // Determine if this handle belongs to the player character.
    // Get the embedded CCharVoice pointer (handle + 0x3E9C).
    void* voicePtr;
    if (handle == NULL) {
        voicePtr = handle;
    } else {
        voicePtr = &handle->voice;
    }
    int isPlayer = 1;
    void* player = getPlayer__Q22cf13CfGameManagerFi(0);
    if (voicePtr == player) {
        if (ml::math::mtRand(2) == 0) {
            isPlayer = 0;
        }
    }

    // If owner state is 1, override isPlayer for specific voice indices.
    if (func_802A77E8(handle) == 1) {
        if (voiceIndex == 17) {
            isPlayer = 1;
        }
        if (voiceIndex == 28) {
            isPlayer = 1;
        }
        if (voiceIndex == 29) {
            isPlayer = 1;
        }
    }

    int voiceId;
    if (isPlayer) {
        // Branch A: player character — base ID 0xDB1 + voiceIndex.
        voiceId = voiceIndex + 0xdb1;

        // If owner state == 4, check item condition.
        if (func_802A77E8(handle) == 4) {
            int cond = (func_800A32BC(func_8009EC9C(4)) == 1);
            if (cond) {
                if (voiceId == 0xdc1) {
                    voiceId = 0xdd5;
                } else if (voiceId == 0xdc2) {
                    voiceId = 0xdd6;
                }
            }
        }

        // If owner state == 3 and party count < 42, adjust voice ID.
        int flag2 = 0;
        if (func_802A77E8(handle) == 3) {
            if (func_800822F4__Q22cf13CfGameManagerFv() < 42) {
                flag2 = 1;
            }
        }
        if (flag2) {
            if (voiceId == 0xdc1) {
                voiceId = 0xdd5;
            } else if (voiceId == 0xdc2) {
                voiceId = 0xdd6;
            } else if (voiceId == 0xdc3) {
                voiceId = 0xdd7;
            } else if (voiceId == 0xdc4) {
                voiceId = 0xdd8;
            }
        }
    } else {
        // Branch B: non-player — base ID 0xD9D + voiceIndex.
        voiceId = voiceIndex + 0xd9d;

        // If owner state == 4, check item condition.
        if (func_802A77E8(handle) == 4) {
            int cond = (func_800A32BC(func_8009EC9C(4)) == 1);
            if (cond) {
                if (voiceId == 0xdad) {
                    voiceId = 0xdbc;
                } else if (voiceId == 0xdae) {
                    voiceId = 0xdb5;
                }
            }
        }

        // If owner state == 3 and party count < 42, adjust voice ID.
        int flag2 = 0;
        if (func_802A77E8(handle) == 3) {
            if (func_800822F4__Q22cf13CfGameManagerFv() < 42) {
                flag2 = 1;
            }
        }
        if (flag2) {
            if (voiceId == 0xdad) {
                voiceId = 0xdb5;
            } else if (voiceId == 0xdae) {
                voiceId = 0xdaf;
            } else if (voiceId == 0xdaf) {
                voiceId = 0xdb0;
            } else if (voiceId == 0xdb0) {
                voiceId = 0xdb2;
            }
        }
    }

    return voiceId;
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
