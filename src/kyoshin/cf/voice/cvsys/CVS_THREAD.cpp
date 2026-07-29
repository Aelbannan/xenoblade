#include "kyoshin/cf/voice/cvsys/CVS_THREAD.hpp"
#include "kyoshin/cf/CfGameManager.hpp"
#include "monolib/math/Random.hpp"

// ── Extern symbols ────────────────────────────────────────────────────────

extern "C" {
    extern u32 lbl_eu_80539910[];   // vtable for CVS_THREAD
    extern void func_800BE924(void* voice);
    extern u8* lbl_eu_80664A58;     // voice manager global pointer
    extern int func_802A77E8(CVoiceHandle* handle);
    extern int func_802A7850(int iter);
    extern int func_80148778(u32* obj, int arg);
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

void func_802A3D54(){
}

void func_802A3E28(){
}

void func_802A3E88(){}
void func_802A3EF0(){}
void func_802A3FD4(){}
void func_802A4120(){}
// ── func_802A4430 ─────────────────────────────────────────────────────────
// Voice-ID selector for battle-voice context.  Dispatches on
// voice-thread state (1–7) and character type to pick the appropriate
// voice line for tension, arts, chain-attack, revive, etc.

int func_802A4430(CVoiceHandle* self) {
    int isLeader = 1;

    // Leader check: bias handle to voice area at +0x3E9C, compare with
    // the player object.  If this voice belongs to the player, there is
    // a 1-in-3 chance of treating it as a non-leader voice.
    {
        u8* vp = (u8*)self;
        if (self != NULL) {
            vp = (u8*)self + 0x3E9C;
        }
        if (vp == (u8*)cf::CfGameManager::getPlayer(0)) {
            if (ml::math::mtRand(2) == 0) {
                isLeader = 0;
            }
        }
    }

    // Extract character type from pointer chain (declare before state
    // so that MWCC assigns this to r31, matching the retail register).
    int charType;

    // Get voice-thread state (declare after charType so MWCC assigns
    // this to r29, matching the retail register).
    int state = func_802A77E8(self);

    {
        UnkTarget* target = self->unkTarget;
        if (target != NULL) {
            charType = target->field_0x08->field_0x18;
        } else {
            charType = -1;
        }
    }

    // Character type must be in [7, 14].
    if (charType < 7) goto invalid_char;
    if (charType - 15 < 0) goto char_ok;
invalid_char:
    return -1;
char_ok:

    // Validate iterator.
    if (func_802A7850(state) == 0) {
        return -1;
    }

    // Dispatch on voice-thread state.
    switch (state) {
    case 1:
        if (charType == 7) {
            // One-shot flag toggle on voice manager at offset 0x222.
            int flag;
            if (lbl_eu_80664A58[0x222] != 0) {
                lbl_eu_80664A58[0x222] = 0;
                flag = 1;
            } else {
                flag = 0;
            }
            if (flag != 0) {
                return 0xBBB;
            }
            if (isLeader == 0) {
                return 0xDD2;
            }
            return 0xDBD;
        }
        if (charType == 8) {
            if (isLeader == 0) {
                return 0xDB3;
            }
            return 0xDD1;
        }
        goto fail;

    case 2:
        if (charType != 7) goto fail;
        // Check arts/skill availability via field at offset 0x8.
        if (func_80148778(&self->unk8, 0xED) != 0) {
            if (isLeader == 0) {
                return 0xDBD;
            }
            return 0xDD2;
        } else {
            if (isLeader == 0) {
                return 0xDBD;
            }
            return 0xDD1;
        }

    case 3: {
        // Party gauge check.
        int gaugeActive = 0;
        if (func_802A77E8(self) == 3) {
            if ((u32)cf::CfGameManager::func_800822F4() < 42) {
                gaugeActive = 1;
            }
        }
        if (gaugeActive != 0) {
            if (isLeader == 0) {
                return 0xDAD;
            }
            return 0xDD9;
        }

        // No gauge: dispatch on sub-state from equipment data.
        {
            UnkWorkObj* workObj;
            UnkEquipData* equipData;
            u16 subState;

            workObj = ((UnkWorkObj*(*)(CVoiceHandle*))self->vtable[0xA9])(self);
            equipData = workObj->field_0x50;
            if (equipData == NULL) goto fail;
            subState = equipData->subState;

            // Jump-table dispatch (9 entries).  The unsigned bounds check
            // via cmplwi/bgt is generated by the switch itself.
            switch (subState - 0x77) {
            case 0: return isLeader ? 0xDD1 : 0xDBF;
            case 1: return isLeader ? 0xDD2 : 0xDBD;
            case 2: return isLeader ? 0xDD3 : 0xDAD;
            case 3: return isLeader ? 0xDD4 : 0xDBE;
            case 4: return isLeader ? 0xDD1 : 0xDBF;
            case 5: return isLeader ? 0xDD2 : 0xDBD;
            case 6: return isLeader ? 0xDD3 : 0xDAD;
            case 7: return isLeader ? 0xDD4 : 0xDBE;
            case 8: return isLeader ? 0xDD3 : 0xDAD;
            default: goto fail;
            }
        }
        goto fail;
    }

    case 4:
        // Character types 7–10 map linearly to voice IDs.
        if (charType < 7) goto fail;
        if (charType > 10) goto fail;
        return charType + 0xDCA;

    case 5:
        // Only valid for character types 7 and 8.
        if (charType - 7 > 1) goto fail;
        if (isLeader == 0) {
            return 0xDBD;
        }
        return 0xDD1;

    case 6:
        if (charType != 7) goto fail;
        return 0xDD1;

    case 7:
        // Character types 8–13 map to dynamic voice IDs.
        if (charType < 8) goto fail;
        if (charType > 13) goto fail;
        if (isLeader == 0) {
            return 0xDBE;
        }
        return charType + 0xDCC;
    }

fail:
    return -1;
}
void func_802A4798(){}
