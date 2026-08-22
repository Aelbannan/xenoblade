// Decompilation of kyoshin/help/CHelp_Talk

#include "kyoshin/help/CHelp_Talk.hpp"
#include "kyoshin/cf/CfGameManager.hpp"

// C++ linkage at global scope -> MWCC emits retail symbol func_800B708C__Fi.
void* func_800B708C(int id);

namespace cf {

// Reset the talk help: enable the switch flag and clear the state byte.
void CHelp_Talk::func_802B86BC() {
    CHelpSwitch::func_802B7CB0();
    field_0x16 = 0;
}

// Evaluate whether the talk help condition is satisfied.
bool CHelp_Talk::func_802B86F0() {
    // Get the current voice action for player 0.
    bool voiceActive;
    CVoiceRec* voice = static_cast<CVoiceRec*>(func_8016FE34(CfGameManager::getPlayer(0)));
    voiceActive = false;

    if (voice != nullptr) {
        u32* ret = voice->field_04->vf30();
        u32 localVal = *ret;

        if (func_80174C98(voice, &localVal, 1) != 0) {
            voiceActive = true;
        }
    }

    // Dispatch to interface-table slot 0x20 with the voice-active flag.
    // Control flow mirrors retail: both failure exits share the function tail.
    if (!reinterpret_cast<CHelpTalkVtblView*>(this)->vf20(voiceActive)) {
        goto retFalse;
    }

    // If no voice is active, return the current state byte as-is.
    if (!voiceActive) {
        goto retState;
    }

    field_0x16 = 1;

    // If field_0x10 is set, validate it against the talk target's id (+0x8C).
    if (field_0x10 != 0) {
        CPlayerTalkRec* targetResult = static_cast<CPlayerTalkRec*>(
            func_8016FE34(CfGameManager::getPlayer(0)));
        // Sub-object at +0x3E9C: vtable 0x4C yields the actor id.
        CTalkActor* targetObj = targetResult != nullptr
            ? static_cast<CTalkActor*>(func_800BF324(func_800B708C(
                reinterpret_cast<int>(targetResult->field_3E9C.vf4C()))))
            : nullptr;

        field_0x16 = (targetObj != nullptr && field_0x10 == targetObj->field_8C) ? 1 : 0;
    }

    // If state is still set and field_0x14 is set, check a global flag.
    if (field_0x16 != 0 && field_0x14 != 0) {
        field_0x16 = (func_8009CF8C(0x7D0) != 0) ? 1 : 0;
    }

    // If state is still set and field_0x15 is set, check the actor's state id.
    if (field_0x16 != 0 && field_0x15 != 0) {
        CPlayerTalkRec* actorResult = static_cast<CPlayerTalkRec*>(
            func_8016FE34(CfGameManager::getPlayer(0)));
        // vtable 0x228: current state id; success when it equals 3.
        bool stateOk = actorResult != nullptr
            ? static_cast<CTalkActor*>(func_800BF324(func_800B708C(
                reinterpret_cast<int>(actorResult->field_3E9C.vf4C()))))->vf228() == 3
            : false;
        field_0x16 = stateOk ? 1 : 0;
    }

goto retFalse;

retState:
    return field_0x16;

retFalse:
    return false;
}

} // namespace cf
